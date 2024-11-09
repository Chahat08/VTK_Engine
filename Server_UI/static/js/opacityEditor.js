const width = 500;
const height = 200;

const intensityRange = [0, 255];
const svg = d3.select("#opacityEditor")
    .attr("width", width)
    .attr("height", height)
    .style("border", "1px solid black");

const xScale = d3.scaleLinear().domain(intensityRange).range([0, width]);
const yScale = d3.scaleLinear().domain([0, 1]).range([height, 0]);

let controlPoints = [
    { id: 1, x: intensityRange[0], y: 0 },
    { id: 2, x: intensityRange[1], y: 1 }
];

let selectedPointId = null;

const line = d3.line()
    .x(d => xScale(d.x))
    .y(d => yScale(d.y));

const path = svg.append("path")
    .datum([...controlPoints].sort((a, b) => a.x - b.x))
    .attr("fill", "none")
    .attr("stroke", "blue")
    .attr("stroke-width", 2)
    .attr("d", line);

const label = svg.append("text")
    .attr("class", "label")
    .attr("x", 10)
    .attr("y", 20)
    .attr("fill", "black")
    .style("display", "none");

const updateControlPoints = () => {
    svg.selectAll(".control-point")
        .data(controlPoints, d => d.id)
        .join("circle")
        .attr("class", "control-point")
        .attr("r", 5)
        .attr("cx", d => xScale(d.x))
        .attr("cy", d => yScale(d.y))
        .attr("fill", d => (d.id === selectedPointId) ? "green" : "red")
        .attr("stroke", d => (d.id === selectedPointId) ? "black" : "none")
        .on("click", function (event, d) {
            event.stopPropagation();
            selectControlPoint(d.id);
        })
        .call(d3.drag()
            .on("start", dragStarted)
            .on("drag", dragged)
            .on("end", dragEnded)
        );
};

const updateLine = () => {
    const sortedPoints = [...controlPoints].sort((a, b) => a.x - b.x);
    path.datum(sortedPoints)
        .attr("d", line);
};

const selectControlPoint = (id) => {
    selectedPointId = id;
    updateLabel();
    updateControlPoints();
};

const updateLabel = () => {
    const selectedPoint = controlPoints.find(d => d.id === selectedPointId);
    if (selectedPoint) {
        label.style("display", "block")
            .attr("x", xScale(selectedPoint.x) + 10)
            .attr("y", yScale(selectedPoint.y) - 10)
            .text(`${selectedPoint.x.toFixed(1)}, ${selectedPoint.y.toFixed(2)}`);
    } else {
        label.style("display", "none");
    }
};

svg.on("click", (event) => {
    event.stopPropagation();
    const [mouseX, mouseY] = d3.pointer(event, svg.node());
    const newPoint = {
        id: Date.now(),
        x: xScale.invert(mouseX),
        y: yScale.invert(mouseY)
    };
    controlPoints.push(newPoint);
    selectControlPoint(newPoint.id);
    updateControlPoints();
    updateLine();
});

function dragStarted(event, d) {
    selectControlPoint(d.id);
    d3.select(this)
        .raise()
        .attr("stroke", "black")
        .attr("fill", "green");
} 

function dragged(event, d) {
    const [mouseX, mouseY] = d3.pointer(event, svg.node());
    d.x = Math.max(intensityRange[0], Math.min(intensityRange[1], xScale.invert(mouseX)));
    d.y = Math.max(0, Math.min(1, yScale.invert(mouseY)));

    d3.select(this)
        .attr("cx", xScale(d.x))
        .attr("cy", yScale(d.y))
        .attr("fill", "green")
        .attr("stroke", "black");

    updateLine();
    updateLabel();
}

function dragEnded(event, d) {
    d3.select(this)
        .attr("stroke", "none")
        .attr("fill", "green");
    updateLabel();
}

document.addEventListener("click", () => {
    selectedPointId = null;
    updateLabel();
    updateControlPoints();
});

updateControlPoints();
updateLine();


///*
//TODO: 
//1. Add a button to remove the selected control point.
//2. Add range sliders to adjust the intensity range + button to reset to original values.
//3. Pass data from the volume.
// - intensity range, histogram.
//4. Color from the color editor showing under curve.
//5. Pass the opacity transfer function to the volume.
// */


