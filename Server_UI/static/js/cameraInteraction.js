const cameraInteractionBox = document.getElementById("cameraInteraction");

cameraInteractionBox.style.touchAction = "none";

let isDragging = false;
let lastX = 0;
let lastY = 0;

cameraInteractionBox.addEventListener("wheel", (event) => {
    const zoomAmount = event.deltaY < 0 ? 1 : -1;
    window.sendCameraZoom(-zoomAmount);
    event.preventDefault();
});

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

        window.sendCameraDrag(deltaX, deltaY);
    }
});

cameraInteractionBox.addEventListener("mouseup", () => {
    isDragging = false;
    cameraInteractionBox.style.cursor = "grab";
});

cameraInteractionBox.addEventListener("mouseleave", () => {
    if (isDragging) {
        isDragging = false;
        cameraInteractionBox.style.cursor = "grab";
    }
});

cameraInteractionBox.addEventListener("touchstart", (event) => {
    if (event.touches.length === 1) {
        isDragging = true;
        lastX = event.touches[0].clientX;
        lastY = event.touches[0].clientY;
        cameraInteractionBox.style.cursor = "grabbing";
    } else if (event.touches.length === 2) {
        
        initialPinchDistance = getPinchDistance(event.touches);
    }
});

cameraInteractionBox.addEventListener("touchmove", (event) => {
    if (event.touches.length === 1 && isDragging) {
        
        const currentX = event.touches[0].clientX;
        const currentY = event.touches[0].clientY;
        const deltaX = currentX - lastX;
        const deltaY = currentY - lastY;

        lastX = currentX;
        lastY = currentY;

        window.sendCameraDrag(deltaX, deltaY);
        event.preventDefault(); 
    } else if (event.touches.length === 2 && initialPinchDistance != null) {
        
        const currentDistance = getPinchDistance(event.touches);
        const pinchFactor = currentDistance > initialPinchDistance ? 1 : -1;

        window.sendCameraZoom(pinchFactor);
        initialPinchDistance = currentDistance;
        event.preventDefault(); 
    }
});

cameraInteractionBox.addEventListener("touchend", (event) => {
    if (event.touches.length === 0) {
        isDragging = false;
        cameraInteractionBox.style.cursor = "grab";
    }
    if (event.touches.length < 2) {
        initialPinchDistance = null;
    }
});

function getPinchDistance(touches) {
    const dx = touches[0].clientX - touches[1].clientX;
    const dy = touches[0].clientY - touches[1].clientY;
    return Math.sqrt(dx * dx + dy * dy);
}
