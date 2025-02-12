document.getElementById('cashFlowForm').addEventListener('submit', function(event) {
    event.preventDefault();

    const n = document.getElementById('n').value;
    const names = document.getElementById('names').value.split(',').map(name => name.trim());
    const currency = document.getElementById('currency').value;
    const graphInput = document.getElementById('graph').value.split('\n');
    const graph = graphInput.map(row => row.split(',').map(Number));

    fetch('/calculate', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ n, names, currency, graph })
    })
    .then(response => response.json())
    .then(data => {
        const resultDiv = document.getElementById('result');
        resultDiv.innerHTML = '<h2>Transactions:</h2>';
        if (Array.isArray(data)) {
            data.forEach(transaction => {
                resultDiv.innerHTML += `<p>${transaction}</p>`;
            });
        } else if (data.error) {
            resultDiv.innerHTML += `<p>Error: ${data.error}</p>`;
        } else {
            resultDiv.innerHTML += `<p>Error: Unknown error occurred</p>`;
        }
    })
    .catch(error => {
        console.error('Error:', error);
        const resultDiv = document.getElementById('result');
        resultDiv.innerHTML = `<p>Error: ${error.message}</p>`;
    });
});
