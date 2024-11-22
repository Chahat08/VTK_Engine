class Joystick {
    constructor(containerId, handleId) {
        this.joystick = document.getElementById(containerId);
        this.handle = document.getElementById(handleId);
        this.isDragging = false;
        this.startX = 0;
        this.startY = 0;
        this.currentX = 0;
        this.currentY = 0;
        this.updateInterval = null; 

        this.handleStart = this.handleStart.bind(this);
        this.handleMove = this.handleMove.bind(this);
        this.handleEnd = this.handleEnd.bind(this);
        this.sendJoystickData = this.sendJoystickData.bind(this);

        this.joystick.addEventListener('mousedown', this.handleStart);
        this.joystick.addEventListener('touchstart', this.handleStart);

        document.addEventListener('mousemove', this.handleMove);
        document.addEventListener('touchmove', this.handleMove);

        document.addEventListener('mouseup', this.handleEnd);
        document.addEventListener('touchend', this.handleEnd);
    }

    handleStart(e) {
        this.isDragging = true;
        this.startX = e.clientX || e.touches[0].clientX;
        this.startY = e.clientY || e.touches[0].clientY;

        this.updateInterval = setInterval(this.sendJoystickData, 50); 
    }

    handleMove(e) {
        if (!this.isDragging) return;

        const clientX = e.clientX || e.touches[0].clientX;
        const clientY = e.clientY || e.touches[0].clientY;

        const deltaX = clientX - this.startX;
        const deltaY = clientY - this.startY;

        const joystickRect = this.joystick.getBoundingClientRect();
        const maxDistance = joystickRect.width / 2 - this.handle.offsetWidth / 2;

        const distance = Math.min(Math.sqrt(deltaX * deltaX + deltaY * deltaY), maxDistance);
        const angle = Math.atan2(deltaY, deltaX);

        this.currentX = Math.round(Math.cos(angle) * distance);
        this.currentY = Math.round(Math.sin(angle) * distance);

        this.handle.style.transform = `translate(calc(-50% + ${this.currentX}px), calc(-50% + ${this.currentY}px))`;
    }

    handleEnd() {
        this.isDragging = false;

        this.handle.style.transform = 'translate(-50%, -50%)';

        clearInterval(this.updateInterval);
        this.updateInterval = null;

        socket.send(JSON.stringify({ joystick: this.joystick.id, x: 0, y: 0 }));
    }

    sendJoystickData() {
        if (!this.isDragging) return;

        const joystickRect = this.joystick.getBoundingClientRect();
        const maxDistance = joystickRect.width / 2 - this.handle.offsetWidth / 2;

        const normalizedX = this.currentX / maxDistance;
        const normalizedY = -this.currentY / maxDistance;

        socket.send(JSON.stringify({ joystick: this.joystick.id, x: normalizedX, y: normalizedY }));
    }
}

document.addEventListener('DOMContentLoaded', () => {
    const leftJoystick = new Joystick('left-joystick', 'left-handle');
    //const rightJoystick = new Joystick('right-joystick', 'right-handle');
});

console.log("Joystick script loaded successfully");
