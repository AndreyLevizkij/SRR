#include<iostream>
#include<Windows.h>
#include <algorithm> 
#include <queue> 

using namespace std;

struct process {
	int num;
	int priority = 0;
	int arrival_time = 0;
	int burst_time = 0;
	int total_time = 0;
	bool is_accepted = false;
};

bool compare1(process p1, process p2)
{
	return p1.arrival_time < p2.arrival_time;
}

void main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int n;
	int tq;
	int a;
	int b;

	do {
		cout << "Enter the number of processes: ";
		cin >> n;
	} while (n < 1);
	do {
		cout << "Enter the quantum size: ";
		cin >> tq;
	} while (tq < 1);
	do {
		cout << "Enter the rate of increasing the priority of the new process: ";
		cin >> a;
	} while (a < 0);
	do {
		cout << "Enter the rate of increasing the priority of the accepted process: ";
		cin >> b;
	} while (b < 0);

	process* p = new process[n];
	int* burst_remaining = new int[n];

	for (int i = 0; i < n; i++) {
		cout << "Enter the time the process appears " << i + 1 << ": ";
		cin >> p[i].arrival_time;
		cout << "Enter the process execution time " << i + 1 << ": ";
		cin >> p[i].burst_time;
		burst_remaining[i] = p[i].burst_time;
		p[i].num = i + 1;
		cout << endl;
	}

	sort(p, p + n, compare1);

	int m = 0;
	for (int i = 0; i < n; i++) {
		m += p[i].burst_time;
	}
	int* make = new int[m];
	m = 0;

	queue<int> q;

	int current_time = 0; // Current time
	int completed = 0; // Number of executed processes
	int max_priority = 0; // Maximum priority
	int min_priority_accepted = 0; // Minimum priority
	int idx = -1;

	while (completed != n) {
		max_priority = 0;
		min_priority_accepted = 0;

		idx = -1;

		if (!q.empty()) { idx = q.front(); q.pop(); }

		if (idx >= 0 && current_time > 0) {
			if (p[idx].arrival_time <= current_time) {
				if (burst_remaining[idx] - tq > 0) {
					make[m] = p[idx].num;
					m++;
					burst_remaining[idx] -= tq;
				}
				else {
					make[m] = p[idx].num;
					m++;
					burst_remaining[idx] = 0;
					completed++;
				}
			}
		}

		for (int i = 0; i < n; i++) {
			if (burst_remaining[i] > 0 && p[i].arrival_time <= current_time) {
				p[i].total_time += tq;
				if (p[i].is_accepted == false) { p[i].priority += a; }
				if (p[i].is_accepted == true) { p[i].priority += b; }
			}
		}

		for (int i = 0; i < n; i++) {
			if (burst_remaining[i] > 0 && p[i].arrival_time <= current_time) {
				if (p[i].priority > max_priority) {
					max_priority = p[i].priority;
					if (p[i].is_accepted == true) {
						min_priority_accepted = max_priority;
					}
				}
			}
		}

		for (int i = 0; i < n; i++) {
			if (burst_remaining[i] > 0 && p[i].arrival_time <= current_time && p[i].is_accepted == true) {
				if (p[i].priority < min_priority_accepted) { min_priority_accepted = p[i].priority; }
			}
		}

		for (int i = 0; i < n; i++) {
			if (burst_remaining[i] > 0 && p[i].arrival_time <= current_time && p[i].is_accepted == false) {
				if (p[i].priority >= min_priority_accepted) {
					p[i].is_accepted = true;
				}
			}
		}

		for (int i = 0; i < n; i++) {
			if (burst_remaining[i] > 0 && p[i].arrival_time <= current_time) {
				if (p[i].priority >= min_priority_accepted && p[i].is_accepted == false) {
					p[i].is_accepted = true;
				}
			}
		}

		bool flag = false;

		for (int i = 0; i < n; i++) {
			if (burst_remaining[i] > 0 && p[i].arrival_time <= current_time && p[i].is_accepted == true) {
				if (idx >= 0) {
					if (i == idx + 1) {
						q.push(i);
						flag = true;
						break;
					}
				}
			}
		}
		if (flag == false) {
			for (int i = 0; i < n; i++) {
				if (burst_remaining[i] > 0 && p[i].arrival_time <= current_time && p[i].is_accepted == true) {
					q.push(i);
					flag = true;
					break;
				}
			}
		}

		int k = 0;
		int ik = -1;
		for (int i = 0; i < n; i++) {
			if (burst_remaining[i] > 0 && p[i].arrival_time <= current_time && p[i].priority >= max_priority && p[i].is_accepted == true) {
				k++;
				ik = i;
			}
		}
		if (k == 1) { while (!q.empty()) { q.pop(); } q.push(ik); }

		current_time += tq;
	}

	for (int i = 0; i < m; i++) {
		cout << make[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < n; i++) {
		cout << "Total time in the system process " << i + 1 << ": " << p[i].total_time << endl;
	}
}
