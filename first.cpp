#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

void minCashFlowRec(vector<int>& amount, const vector<string>& names) {
 
    int maxCredit = max_element(amount.begin(), amount.end()) - amount.begin();
    int maxDebit = min_element(amount.begin(), amount.end()) - amount.begin();

    if (amount[maxCredit] == 0 && amount[maxDebit] == 0)
        return;

    int settle = min(-amount[maxDebit], amount[maxCredit]);
    amount[maxCredit] -= settle;
    amount[maxDebit] += settle;

    cout << names[maxDebit] << " pays " << settle << " to " << names[maxCredit] << endl;

    minCashFlowRec(amount, names);
}

void minimizeCashFlow(const vector<vector<int>>& graph, const vector<string>& names) {
    int n = graph.size();
    vector<int> amount(n, 0);

    for (int p = 0; p < n; ++p) {
        for (int i = 0; i < n; ++i) {
            amount[p] += (graph[i][p] - graph[p][i]);
        }
    }

    minCashFlowRec(amount, names);
}

int main() {
    int n;
    cout << "Enter number of people: ";
    cin >> n;
    cin.ignore();

    vector<string> names(n);
    cout << "Enter names (space separated): ";
    for (int i = 0; i < n; ++i) {
        cin >> names[i];
    }

    cout << "Enter the transaction matrix (" << n << " rows, each with " << n << " values):\n";
    vector<vector<int>> graph(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> graph[i][j];
        }
    }

    cout << "\nMinimal Transactions:\n";
    minimizeCashFlow(graph, names);

    return 0;
}
