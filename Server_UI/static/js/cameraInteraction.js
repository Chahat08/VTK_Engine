const cameraInteractionBox = document.getElementById("cameraInteraction");

let isDragging = false;
let lastX = 0;
let lastY = 0;

// Capture mouse wheel for zooming
cameraInteractionBox.addEventListener("wheel", (event) => {
    const zoomAmount = event.deltaY < 0 ? 1 : -1; // Positive for zoom in, negative for zoom out
    window.sendCameraZoom(-zoomAmount);
    event.preventDefault();
});

// Capture mouse down, move, and up for dragging
cameraInteractionBox.addEventListener("mousedown", (event) => {
    isDragging = true;
    lastX = event.clientX;
    lastY = event.clientY;
    cameraInteractionBox.style.cursor = "grabbing";
});

cameraInteractionBox.addEventListener("mousemove", (event) => {
    if (isDragging) {
        const deltaX = event.clientX - lastX;
        const deltaY = event.clientY - lastY;
        lastX = event.clientX;
        lastY = event.clientY;

        // Send drag offsets to server
        window.sendCameraDrag(deltaX, deltaY);
    }
});

cameraInteractionBox.addEventListener("mouseup", () => {
    isDragging = false;
    cameraInteractionBox.style.cursor = "grab";
});

// Handle mouse leaving the box while dragging
cameraInteractionBox.addEventListener("mouseleave", () => {
    if (isDragging) {
        isDragging = false;
        cameraInteractionBox.style.cursor = "grab";
    }
});

// Touch events for mobile pinch zoom
let initialPinchDistance = null;

cameraInteractionBox.addEventListener("touchstart", (event) => {
    if (event.touches.length === 2) {
        initialPinchDistance = getPinchDistance(event.touches);
    }
});

cameraInteractionBox.addEventListener("touchmove", (event) => {
    if (event.touches.length === 2 && initialPinchDistance != null) {
        const currentDistance = getPinchDistance(event.touches);
        const pinchFactor = currentDistance > initialPinchDistance ? 1 : -1;
        window.sendCameraZoom(pinchFactor);

        initialPinchDistance = currentDistance;
        event.preventDefault();
    }
});

cameraInteractionBox.addEventListener("touchend", (event) => {
    if (event.touches.length < 2) {
        initialPinchDistance = null;
    }
});

function getPinchDistance(touches) {
    const dx = touches[0].clientX - touches[1].clientX;
    const dy = touches[0].clientY - touches[1].clientY;
    return Math.sqrt(dx * dx + dy * dy);
}
