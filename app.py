from flask import Flask, render_template, request, jsonify
from flask_cors import CORS
import numpy as np

app = Flask(__name__)
CORS(app)

class Solution:
    def minCashFlow(self, graph, currency, names):
        n = len(graph)
        amount = [0] * n

        for i in range(n):
            for j in range(n):
                amount[i] += graph[j][i] - graph[i][j]

        minQ = []
        maxQ = []
        transactions = []

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
            owed_amount = min(-maxDebitEntry[1], maxCreditEntry[1])

            transactions.append(f"{names[debtor]} pays {owed_amount:.2f} {currency} to {names[creditor]}")

            if transaction_val == 0:
                minQ.remove(maxDebitEntry)
                maxQ.remove(maxCreditEntry)
            elif transaction_val < 0:
                minQ.remove(maxDebitEntry)
                minQ.append((debtor, transaction_val))
                maxQ.remove(maxCreditEntry)
            else:
                maxQ.remove(maxCreditEntry)
                maxQ.append((creditor, transaction_val))
                minQ.remove(maxDebitEntry)

        return transactions

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/calculate', methods=['POST'])
def calculate():
    try:
        data = request.json
        n = int(data['n'])
        names = data['names']
        currency = data['currency']
        graph = np.array(data['graph'])

        if n <= 0:
            return jsonify({"error": "Invalid number of people."}), 400
        if len(names) != n:
            return jsonify({"error": "Names count does not match the number of people."}), 400
        if graph.shape != (n, n):
            return jsonify({"error": "Graph must be a square matrix."}), 400

        solution = Solution()
        transactions = solution.minCashFlow(graph, currency, names)
        return jsonify(transactions)

    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True)
