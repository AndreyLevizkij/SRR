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
		cout << "Ââåäèòå êîëè÷åñòâî ïðîöåññîâ: ";
		cin >> n;
	} while (n < 1);
	do {
		cout << "Ââåäèòå ðàçìåð êâàíòà: ";
		cin >> tq;
	} while (tq < 1);
	do {
		cout << "Ââåäèòå ñêîðîñòü ïîâûøåíèÿ ïðèîðèòåòà íîâîãî ïðîöåññà: ";
		cin >> a;
	} while (a < 0);
	do {
		cout << "Ââåäèòå ñêîðîñòü ïîâûøåíèÿ ïðèîðèòåòà âûáðàííîãî ïðîöåññà: ";
		cin >> b;
	} while (b < 0);

	process* p = new process[n];
	int* burst_remaining = new int[n];

	for (int i = 0; i < n; i++) {
		cout << "Ââåäèòå âðåìÿ ïîÿâëåíèÿ ïðîöåññà " << i + 1 << ": ";
		cin >> p[i].arrival_time;
		cout << "Ââåäèòå âðåìÿ âûïîëíåíèÿ ïðîöåññà " << i + 1 << ": ";
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
	q.push(0);
	int current_time = 0; // Òåêóùåå âðåìÿ
	int completed = 0; // Êîëè÷åñòâî âûïîëíåííûõ ïðîöåññîâ
	int max_priority = 0; // Ìàêñèìàëüíûé ïðèîðèòåò
	int min_priority_accepted = 0; // Ìèíèìàëüíûé ïðèîðèòåò
	int idx = -1;

	if (a != 0) { 
		if (b == 0 || b / a >= 1) { p[0].is_accepted = true; }
	}

	while (completed != n) {

		if (!q.empty()) { idx = q.front(); q.pop();}
		
		max_priority = 0;
		min_priority_accepted = 0;

		bool flag = false;
		if (idx >= 0) {
			if (p[idx].arrival_time <= current_time) {
				if (burst_remaining[idx] - tq > 0) {
					make[m] = p[idx].num;
					m++;
					burst_remaining[idx] -= tq;
					current_time += tq;
					flag = true;
				}
				else {
					if (idx == 0 && p[idx].arrival_time == 0) { p[idx].total_time += tq; }
					current_time += burst_remaining[idx];
					make[m] = p[idx].num;
					m++;
					burst_remaining[idx] = 0;
					completed++;
					flag = true;
				}
			}
		}
		if (flag == false) { current_time++; }

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
			if (burst_remaining[i] > 0 && p[i].arrival_time <= current_time) {
				if (p[i].priority >= min_priority_accepted && p[i].is_accepted == false) {
					p[i].is_accepted = true;
					while (!q.empty()){ q.pop(); }
				}
			}
		}		

		int k = 0;
		int ik = -1;
		idx = -1;
		for (int i = 0; i < n; i++) {
			if (burst_remaining[i] > 0 && p[i].arrival_time <= current_time && p[i].priority >= max_priority && p[i].is_accepted == true) {
				k++;
				ik = i;
			}
		}
		if (k == 1) { idx = ik; }
		else if (q.empty() && k > 1) {
			for (int i = 0; i < n; i++) {
				if (burst_remaining[i] > 0 && p[i].arrival_time <= current_time && p[i].priority >= max_priority && p[i].is_accepted == true) {
					q.push(i);
				}
			}
		}
	}
	for (int i = 0; i < m; i++) {
		cout << make[i] << " "; 
	}
	cout << endl;
	for (int i = 0; i < n; i++) {
		cout << "Îáùåå âðåìÿ â ñèñòåìå ïðîöåññà " << i + 1 << ": " << p[i].total_time << endl;
	}
}
