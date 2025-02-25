const intensityRange = [0, 65280];
const defaultIsoValue = (intensityRange[0] + intensityRange[1]) / 2;

window.controlPoints = [
    { id: 1, x: intensityRange[0], y: 0 },
    { id: 2, x: intensityRange[1], y: 1 }
];
window.lineColors = [];

function scalePositionToIntensity(position) {
    const intensityMin = intensityRange[0];
    const intensityMax = intensityRange[1];
    return (position / sliderWidth) * (intensityMax - intensityMin) + intensityMin;
}

function sendControlPoints() {
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify(
            {
                controlPoints: window.controlPoints.map(p => ({ value: p.x, opacity: p.y }))
            }
        ));
    }
}

function sendColorStops() {
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify(
            {
                colorStops: window.lineColors.map(({ line, color }) => ({
                    position: scalePositionToIntensity(parseFloat(line.style("left"))),
                    color: color
                }))
            }
        ));
    }
}

function sendCameraZoom(zoomAmount) {
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify({ cameraZoom: zoomAmount }));
    }
}

function sendCameraDrag(deltaX, deltaY) {
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify({ cameraDrag: { deltaX, deltaY } }));
    }
}


window.sendColorStops = sendColorStops;
window.sendControlPoints = sendControlPoints;
window.sendCameraZoom = sendCameraZoom;
window.sendCameraDrag = sendCameraDrag;

const bgColorPicker = document.getElementById("bgColorPicker");
bgColorPicker.addEventListener("input", (event) => {
    const color = event.target.value;
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify({ bgColor: color }));
    }
});

const outlineCheckbox = document.getElementById("outlineCheckbox");
outlineCheckbox.addEventListener("change", () => {
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify({ outline: outlineCheckbox.checked }));
    }
});

const shadingCheckbox = document.getElementById("shadingCheckbox");
shadingCheckbox.addEventListener("change", () => {
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify({ shading: shadingCheckbox.checked }));
    }
});

const interpolationTypeDropdown = document.getElementById("interpolationType");
interpolationTypeDropdown.addEventListener("change", () => {
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify({ interpolationType: interpolationTypeDropdown.value }));
    }
});

const autosampleCheckbox = document.getElementById("autosampleCheckbox");
const sampleDistanceContainer = document.getElementById("sampleDistanceContainer");
autosampleCheckbox.addEventListener("change", () => {
    sampleDistanceContainer.style.display = autosampleCheckbox.checked ? "none" : "block";
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify({ autosampleDistances: autosampleCheckbox.checked }));
    }
});

const sampleDistanceInput = document.getElementById("sampleDistance");
sampleDistanceInput.addEventListener("input", () => {
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify({ sampleDistance: parseFloat(sampleDistanceInput.value) }));
    }
});

const blendModeDropdown = document.getElementById("blendMode");
const isosurfaceValuesContainer = document.getElementById("isosurfaceValuesContainer");
const isosurfaceInputs = document.getElementById("isosurfaceInputs");
const addIsoValueButton = document.getElementById("addIsoValueButton");

blendModeDropdown.addEventListener("change", () => {
    const blendMode = blendModeDropdown.value;
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify({ blendMode }));
    }

    if (blendMode === "Isosurface") {
        isosurfaceValuesContainer.style.display = "block";
        if (isosurfaceInputs.children.length === 0) {
            addIsosurfaceInput(defaultIsoValue);
        }
    } else {
        isosurfaceValuesContainer.style.display = "none";
    }
});

function addIsosurfaceInput(value = defaultIsoValue) {
    const input = document.createElement("input");
    input.type = "number";
    input.className = "isoValueInput";
    input.value = value;
    input.step = "0.1";
    input.min = "0";
    input.addEventListener("input", () => sendIsosurfaceValues());
    isosurfaceInputs.appendChild(input);
    sendIsosurfaceValues();
}

addIsoValueButton.addEventListener("click", () => {
    addIsosurfaceInput();
});

function sendIsosurfaceValues() {
    const isoValues = Array.from(document.getElementsByClassName("isoValueInput")).map(input => parseFloat(input.value));
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify({ isosurfaceValues: isoValues }));
    }
}