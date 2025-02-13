const flex12 = document.getElementById("flex12Slider");
const flex23 = document.getElementById("flex23Slider");
const flex34 = document.getElementById("flex34Slider");
const flex45 = document.getElementById("flex45Slider");

flex12.addEventListener("input", (event) => {
    const angle = parseFloat(event.target.value);
    socket.send(JSON.stringify({ flexAngle: angle, col: 1 }));
});

flex23.addEventListener("input", (event) => {
    const angle = parseFloat(event.target.value);
    socket.send(JSON.stringify({ flexAngle: angle, col: 2 }));
});

flex34.addEventListener("input", (event) => {
    const angle = parseFloat(event.target.value);
    socket.send(JSON.stringify({ flexAngle: angle, col: 4 }));
});

flex45.addEventListener("input", (event) => {
    const angle = parseFloat(event.target.value);
    socket.send(JSON.stringify({ flexAngle: angle, col: 5 }));
});

