const upButton = document.getElementById("upButton");
const downButton = document.getElementById("downButton");

let intervalId = null;

const startSending = (direction) => {
    if (intervalId !== null) return;
    intervalId = setInterval(() => {
        socket.send(JSON.stringify({ direction }));
    }, 100);
};

const stopSending = () => {
    clearInterval(intervalId);
    intervalId = null;
};

const disableContextMenu = (event) => event.preventDefault();

upButton.addEventListener("mousedown", () => startSending("up"));
upButton.addEventListener("mouseup", stopSending);
upButton.addEventListener("mouseleave", stopSending);
upButton.addEventListener("touchstart", (event) => {
    disableContextMenu(event);
    startSending("up");
}, { passive: false });
upButton.addEventListener("touchend", stopSending);
upButton.addEventListener("touchcancel", stopSending);

downButton.addEventListener("mousedown", () => startSending("down"));
downButton.addEventListener("mouseup", stopSending);
downButton.addEventListener("mouseleave", stopSending);
downButton.addEventListener("touchstart", (event) => {
    disableContextMenu(event);
    startSending("down");
}, { passive: false });
downButton.addEventListener("touchend", stopSending);
downButton.addEventListener("touchcancel", stopSending);