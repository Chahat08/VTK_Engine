const width = 500;
const height = 200;

const svg = d3.select("#opacityEditor")
    .attr("width", width)
    .attr("height", height)
    .style("border", "1px solid black");

const xScale = d3.scaleLinear().domain(intensityRange).range([0, width]);
const yScale = d3.scaleLinear().domain([0, 1]).range([height, 0]);

let controlPoints = window.controlPoints;

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

const label = svg.append("g")
    .attr("class", "label-group")
    .style("display", "none");

label.append("rect")
    .attr("class", "label-background")
    .attr("rx", 3)
    .attr("ry", 3)
    .attr("fill", "white")
    .attr("fill-opacity", 0.7);

label.append("text")
    .attr("class", "label-text")
    .attr("fill", "black")
    .attr("font-size", "10px")
    .attr("dy", ".35em");

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
        const labelText = `${selectedPoint.x.toFixed(1)}, ${selectedPoint.y.toFixed(2)}`;
        const labelPadding = 4;

        label.style("display", "block")
            .attr("transform", `translate(${xScale(selectedPoint.x) + 10}, ${yScale(selectedPoint.y) - 10})`);

        label.select("text")
            .text(labelText);

        const textBBox = label.select("text").node().getBBox();

        label.select("rect")
            .attr("x", -labelPadding)
            .attr("y", -textBBox.height / 2 - labelPadding)
            .attr("width", textBBox.width + 2 * labelPadding)
            .attr("height", textBBox.height + 2 * labelPadding);
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
    window.sendControlPoints();
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
    if (d.x === intensityRange[0] || d.x === intensityRange[1]) {
        // For end points, only allow vertical movement
        d.y = Math.max(0, Math.min(1, yScale.invert(mouseY)));
    } else {
        d.x = Math.max(intensityRange[0], Math.min(intensityRange[1], xScale.invert(mouseX)));
        d.y = Math.max(0, Math.min(1, yScale.invert(mouseY)));
    }

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
    window.sendControlPoints();
}

document.addEventListener("click", () => {
    selectedPointId = null;
    updateLabel();
    updateControlPoints();
});

// Remove selected control point
d3.select("#opacityMinusButton").on("click", () => {
    if (selectedPointId && controlPoints.length > 2) {
        const selectedPoint = controlPoints.find(d => d.id === selectedPointId);
        if (selectedPoint.x !== intensityRange[0] && selectedPoint.x !== intensityRange[1]) {
            controlPoints = controlPoints.filter(d => d.id !== selectedPointId);
            selectedPointId = null;
            updateControlPoints();
            updateLine();
            updateLabel();
            window.sendControlPoints();
        }
    }
});

// Remove all control points except the end points
d3.select("#opacityRemoveAllButton").on("click", () => {
    controlPoints = controlPoints.filter(d => d.x === intensityRange[0] || d.x === intensityRange[1]);
    selectedPointId = null;
    updateControlPoints();
    updateLine();
    updateLabel();
    window.sendControlPoints();
});

updateControlPoints();
updateLine();


///*
//TODO: 
//2. Add range sliders to adjust the intensity range + button to reset to original values.
//3. Pass data from the volume.
// - intensity range, histogram.
//4. Color from the color editor showing under curve.
//5. Pass the opacity transfer function to the volume.
// */


