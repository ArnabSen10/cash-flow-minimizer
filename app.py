from flask import Flask, render_template, request, jsonify
import numpy as np

app = Flask(__name__)

class Solution:
    def minCashFlow(self, graph, currency):
        print("Calculating cash flow...")

        n = graph.shape[0]
        amount = [0] * n
        for i in range(n):
            for j in range(n):
                amount[i] += graph[j][i] - graph[i][j]

        print("\nNet Balances Before Transactions:")
        for i in range(n):
            print(f"{names[i]}: {amount[i]} {currency}")

        minQ = []
        maxQ = []
        for i in range(n):
            if amount[i] > 0:
                maxQ.append((i, amount[i]))
            elif amount[i] < 0:
                minQ.append((i, amount[i]))

        while minQ and maxQ:
            maxCreditEntry = max(maxQ, key=lambda x: x[1])
            maxDebitEntry = min(minQ, key=lambda x: x[1])

            transaction_val = maxCreditEntry[1] + maxDebitEntry[1]
            debtor = maxDebitEntry[0]
            creditor = maxCreditEntry[0]
            owed_amount = 0

            if transaction_val == 0:
                owed_amount = maxCreditEntry[1]
            elif transaction_val < 0:
                owed_amount = maxCreditEntry[1]
                minQ.remove(maxDebitEntry)
                minQ.append((debtor, transaction_val))
            else:
                owed_amount = -maxDebitEntry[1]
                maxQ.remove(maxCreditEntry)
                maxQ.append((creditor, transaction_val))

            print(f"{names[debtor]} pays {owed_amount} {currency} to {names[creditor]}")

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/calculate', methods=['POST'])
def calculate():
    try:
        data = request.json
        n = data['n']
        names = data['names']
        currency = data['currency']
        graph = np.array(data['graph'])

        if not isinstance(n, int) or n <= 0:
            return jsonify({"error": "Invalid number of people."}), 400
        if len(names) != n:
            return jsonify({"error": "Names count does not match the number of people."}), 400
        if graph.shape != (n, n):
            return jsonify({"error": "Graph must be a square matrix."}), 400

        solution = Solution()
        transactions = solution.minCashFlow(graph, currency)
        print("Transactions:", transactions)
        return jsonify(transactions)

    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True)
