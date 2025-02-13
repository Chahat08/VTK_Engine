const resetPositionButton = document.getElementById("resetPositionButton");
const resetSpeedButton = document.getElementById("resetSpeedButton");

resetPositionButton.addEventListener("click", () => {
    const command = { resetCamera: true };
    socket.send(JSON.stringify(command));
});

resetSpeedButton.addEventListener("click", () => {
    cameraSpeedSlider.value = 2;
    cameraSpeedValue.textContent = "2"; 

    arcballSpeedSlider.value = 1;
    arcballSpeedValue.textContent = "1";

    socket.send(JSON.stringify({ freeCameraSpeed: 2 }));
    socket.send(JSON.stringify({ arcballSpeed: 1 }));
});

const cameraSpeedSlider = document.getElementById("cameraSpeedSlider");
const cameraSpeedValue = document.getElementById("cameraSpeedValue");

cameraSpeedSlider.addEventListener("input", (event) => {
    const speed = parseFloat(event.target.value);
    cameraSpeedValue.textContent = speed.toFixed(1);

    socket.send(JSON.stringify({ freeCameraSpeed: speed }));
});

const arcballSpeedSlider = document.getElementById("arcballSpeedSlider");
const arcballSpeedValue = document.getElementById("arcballSpeedValue");

arcballSpeedSlider.addEventListener("input", (event) => {
    const speed = parseFloat(event.target.value);
    arcballSpeedValue.textContent = speed.toFixed(1);

    socket.send(JSON.stringify({ arcballSpeed: speed }));
});