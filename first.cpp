#include <iostream>
#include <queue>
#include <vector>
#include <limits>
#include <iomanip>
#include <map>
#include <string>

using namespace std;

struct AscCmp {
    bool operator()(const pair<int, int>& p1, const pair<int, int>& p2) {
        return p1.second < p2.second;
    }
};

struct DscCmp {
    bool operator()(const pair<int, int>& p1, const pair<int, int>& p2) {
        return p1.second > p2.second;
    }
};

class Solution {
private:
    priority_queue<pair<int, int>, vector<pair<int, int>>, DscCmp> minQ;
    priority_queue<pair<int, int>, vector<pair<int, int>>, AscCmp> maxQ;
    vector<vector<string>> transactionHistory;
    map<string, double> exchangeRates;
    vector<string> names; // Store names instead of indices

    void constructMinMaxQ(const vector<int>& amount) {
        for (int i = 0; i < amount.size(); ++i) {
            if (amount[i] == 0) continue;
            if (amount[i] > 0) {
                maxQ.emplace(i, amount[i]);
            } else {
                minQ.emplace(i, amount[i]);
            }
        }
    }

    void solveTransaction(const string& currency) {
        while (!minQ.empty() && !maxQ.empty()) {
            pair<int, int> maxCreditEntry = maxQ.top();
            maxQ.pop();

            pair<int, int> maxDebitEntry = minQ.top();
            minQ.pop();

            int transaction_val = maxCreditEntry.second + maxDebitEntry.second;
            int debtor = maxDebitEntry.first, creditor = maxCreditEntry.first, owed_amount = 0;

            if (transaction_val == 0) {
                owed_amount = maxCreditEntry.second;
            } else if (transaction_val < 0) {
                owed_amount = maxCreditEntry.second;
                minQ.emplace(maxDebitEntry.first, transaction_val);
            } else {
                owed_amount = -(maxDebitEntry.second);
                maxQ.emplace(maxCreditEntry.first, transaction_val);
            }

            cout << names[debtor] << " pays " << owed_amount << " " << currency << " to " << names[creditor] << endl;
            transactionHistory.push_back({names[debtor], names[creditor], to_string(owed_amount), currency});
        }
    }

public:
    Solution() {
        exchangeRates["USD"] = 1.0;
        exchangeRates["EUR"] = 0.85;
        exchangeRates["GBP"] = 0.73;
        exchangeRates["JPY"] = 110.0;
    }

    void minCashFlow(const vector<vector<int>>& graph, const string& currency) {
        int n = graph.size();
        vector<int> amount(n, 0);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int diff = graph[j][i] - graph[i][j];
                amount[i] += diff;
            }
        }

        // Display net balances before minimizing transactions
        cout << "\nNet Balances Before Transactions:\n";
        for (int i = 0; i < n; ++i) {
            cout << names[i] << ": " << amount[i] << " " << currency;
            if (amount[i] > 0)
                cout << " (to be received)";
            else if (amount[i] < 0)
                cout << " (to be paid)";
            cout << endl;
        }
        cout << endl;

        constructMinMaxQ(amount);
        solveTransaction(currency);
    }

    bool validateGraph(const vector<vector<int>>& graph) {
        int n = graph.size();
        for (int i = 0; i < n; ++i) {
            if (graph[i].size() != n) {
                cout << "Error: Graph is not square." << endl;
                return false;
            }
            for (int j = 0; j < n; ++j) {
                if (graph[i][j] < 0) {
                    cout << "Error: Negative values are not allowed." << endl;
                    return false;
                }
                if (i == j && graph[i][j] != 0) {
                    cout << "Error: Diagonal elements should be zero." << endl;
                    return false;
                }
            }
        }
        return true;
    }

    void setNames(const vector<string>& inputNames) {
        names = inputNames;
    }

    void printTransactionHistory() {
        cout << "\nTransaction History:\n";
        cout << setw(15) << "From" << setw(15) << "To" << setw(10) << "Amount" << setw(10) << "Currency\n";
        for (const auto& transaction : transactionHistory) {
            cout << setw(15) << transaction[0] << setw(15) << transaction[1] 
                 << setw(10) << transaction[2] << setw(10) << transaction[3] << "\n";
        }
    }
};

vector<vector<int>> getUserInput(int& n, string& currency, vector<string>& names) {
    cout << "Enter the number of people: ";
    cin >> n;

    names.resize(n);
    cout << "Enter names of people: ";
    for (int i = 0; i < n; ++i) {
        cin >> names[i];
    }

    vector<vector<int>> graph(n, vector<int>(n, 0));

    cout << "Enter the currency (USD/EUR/GBP/JPY): ";
    cin >> currency;

    cout << "Enter the cash flow matrix:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                cout << "Amount " << names[i] << " owes to " << names[j] << " (" << currency << "): ";
                cin >> graph[i][j];
            }
        }
    }

    return graph;
}

int main() {
    Solution S;
    int choice;
    string currency;

    do {
        cout << "\n1. Use predefined test case" << endl;
        cout << "2. Enter custom cash flow" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            vector<vector<int>> graph = {
                { 0, 1000, 2000 },
                { 0, 0, 5000 },
                { 0, 0, 0 },
            };
            vector<string> testNames = {"Alice", "Bob", "Charlie"};
            currency = "USD";

            cout << "Solution for predefined test case (USD):" << endl;
            S.setNames(testNames);
            if (S.validateGraph(graph)) {
                S.minCashFlow(graph, currency);
                S.printTransactionHistory();
            }
            break;
        }
        case 2: {
            int n;
            vector<string> names;
            vector<vector<int>> userGraph = getUserInput(n, currency, names);
            S.setNames(names);
            if (S.validateGraph(userGraph)) {
                cout << "Solution for user input:" << endl;
                S.minCashFlow(userGraph, currency);
                S.printTransactionHistory();
            }
            break;
        }
        case 3:
            cout << "Exiting program." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);

    return 0;
}
