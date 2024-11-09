const sliderWidth = 500;
const sliderHeight = 30;
const colorSlider = d3.select("#colorEditor")
    .style("width", `${sliderWidth}px`)
    .style("height", `${sliderHeight}px`)
    .style("position", "relative")
    .style("border", "1px solid black");

let selectedLine = null;
let lineColors = []; // Array to store line and color pairs

// Function to update the gradient
function updateGradient() {
    const gradientStops = lineColors.map(({ line, color }) => {
        const percentage = (parseFloat(line.style("left")) / sliderWidth) * 100;
        return `${color} ${percentage}%`;
    }).sort((a, b) => parseFloat(a.split(' ')[1]) - parseFloat(b.split(' ')[1])).join(", ");
    colorSlider.style("background", `linear-gradient(to right, ${gradientStops})`);
}

// Function to add a new dividing line at a specified position
function addDividingLine(xPosition, color) {
    const line = colorSlider.append("div")
        .style("position", "absolute")
        .style("left", `${xPosition}px`)
        .style("top", "0")
        .style("height", `${sliderHeight}px`)
        .style("width", "2px")
        .style("background-color", "black")
        .style("cursor", "ew-resize")
        .classed("divider-line", true)
        .on("click", function (event) {
            event.stopPropagation();
            selectLine(line);
        })
        .call(d3.drag()
            .on("drag", (event) => {
                const newX = Math.max(0, Math.min(sliderWidth, d3.pointer(event, colorSlider.node())[0]));
                line.style("left", `${newX}px`);
                updateGradient();
            })
        );

    lineColors.push({ line, color });
    selectLine(line);
    updateGradient();
}

// Function to handle selecting a line
function selectLine(line) {
    if (selectedLine) {
        selectedLine.style("border", "none");
    }
    selectedLine = line;
    selectedLine.style("border", "1px solid white");
    showColorPicker();
}

// Function to show color picker
function showColorPicker() {
    const lineColor = lineColors.find(lc => lc.line === selectedLine);
    if (!lineColor) return;

    const colorPicker = d3.select("#colorPicker")
        .style("display", "block")
        .property("value", lineColor.color);

    // Remove any existing event listeners
    colorPicker.on("input", null).on("change", null);

    // Add new event listeners
    colorPicker
        .on("input", function () {
            // Update color immediately when picking
            lineColor.color = this.value;
            updateGradient();
        })
        .on("change", function () {
            // Ensure color is updated when the picker is closed
            lineColor.color = this.value;
            updateGradient();
        });
}

// Add initial red and blue sliders
addDividingLine(0, "#FF0000");
addDividingLine(sliderWidth, "#0000FF");

// Add a dividing line at the center when the button is clicked
d3.select("#addDividerButton").on("click", () => {
    const initialX = sliderWidth / 2;
    const leftColor = lineColors.reduce((prev, curr) =>
        parseFloat(curr.line.style("left")) < initialX && parseFloat(curr.line.style("left")) > parseFloat(prev.line.style("left")) ? curr : prev
    ).color;
    addDividingLine(initialX, leftColor);
});

// Delete the selected line when the delete button is clicked
d3.select("#deleteDividerButton").on("click", () => {
    if (selectedLine && lineColors.length > 2) {  // Prevent deleting if only 2 lines remain
        const index = lineColors.findIndex(lc => lc.line === selectedLine);
        if (index !== -1) {
            selectedLine.remove();
            lineColors.splice(index, 1);
            selectedLine = null;
            d3.select("#colorPicker").style("display", "none");
            updateGradient();
        }
    }
});

// Deselect line on clicking outside
document.addEventListener("click", (event) => {
    if (!event.target.closest("#colorEditor") && !event.target.closest("#colorPicker")) {
        if (selectedLine) {
            selectedLine.style("border", "none");
            selectedLine = null;
            d3.select("#colorPicker").style("display", "none");
        }
    }
});