class Joystick {
    constructor(containerId, handleId) {
        this.joystick = document.getElementById(containerId);
        this.handle = document.getElementById(handleId);
        this.isDragging = false;
        this.startX = 0;
        this.startY = 0;

        this.handleStart = this.handleStart.bind(this);
        this.handleMove = this.handleMove.bind(this);
        this.handleEnd = this.handleEnd.bind(this);

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

        const x = Math.round(Math.cos(angle) * distance);
        const y = Math.round(Math.sin(angle) * distance);

        this.handle.style.transform = `translate(calc(-50% + ${x}px), calc(-50% + ${y}px))`;

        // Normalize x and y values to range -1 to 1
        const normalizedX = x / maxDistance;
        const normalizedY = -y / maxDistance;

        socket.send(JSON.stringify({ joystick: this.joystick.id, x: normalizedX, y: normalizedY }));
    }

    handleEnd() {
        this.isDragging = false;
        this.handle.style.transform = 'translate(-50%, -50%)';

        socket.send(JSON.stringify({ joystick: this.joystick.id, x: 0, y: 0 }));
    }
}

// Initialize joysticks
document.addEventListener('DOMContentLoaded', () => {
    const leftJoystick = new Joystick('left-joystick', 'left-handle');
    const rightJoystick = new Joystick('right-joystick', 'right-handle');
});

console.log("Joystick script loaded successfully");