const clientListContainer = document.getElementById("clientListContainer");
const terminateAllButton = document.getElementById("terminateAllButton");

terminateAllButton.addEventListener("click", () => {
    socket.send(JSON.stringify({ "terminate_all":true }));    
});

function updateClientListUI(clients) {
    const sortedClients = clients
        .filter(clientId => clientId !== "Frontend_Client")
        .sort((a, b) => {
            const [nodeA, instanceA] = a.split("_").map(Number);
            const [nodeB, instanceB] = b.split("_").map(Number);
            return nodeA === nodeB ? instanceA - instanceB : nodeA - nodeB;
        });

    clientListContainer.innerHTML = ""; 

    sortedClients.forEach(clientId => {
        const [node, instance] = clientId.split("_");

        const clientBox = document.createElement("div");
        clientBox.classList.add("client-box");

        const clientInfo = document.createElement("div");
        clientInfo.classList.add("client-info");
        clientInfo.textContent = `Node: ${node}, Instance: ${instance}`;
        clientBox.appendChild(clientInfo);

        const terminateButton = document.createElement("button");
        terminateButton.classList.add("terminate-button");
        terminateButton.textContent = "X";
        terminateButton.addEventListener("click", () => {
            socket.send(JSON.stringify({ "terminate_client":clientId }));
        });
        clientBox.appendChild(terminateButton);

        clientListContainer.appendChild(clientBox);
    });
}
