const sliderWidth = 500;
const sliderHeight = 30;
const colorSlider = d3.select("#colorEditor")
    .style("width", `${sliderWidth}px`)
    .style("height", `${sliderHeight}px`)
    .style("background", "linear-gradient(to right, red, blue)")
    .style("position", "relative");  // Make colorSlider the reference for positioning

let selectedLine = null;  // Track the currently selected dividing line
let lines = [];           // Array to hold all dividing line elements

// Add a dividing line at the center when the button is clicked
d3.select("#addDividerButton").on("click", () => {
    const initialX = sliderWidth / 2;
    addDividingLine(initialX);
});

// Delete the selected line when the delete button is clicked
d3.select("#deleteDividerButton").on("click", () => {
    if (selectedLine) {
        selectedLine.remove();  // Remove the line from the DOM
        lines = lines.filter(line => line !== selectedLine);  // Remove from array
        selectedLine = null;
        toggleDeleteButton(false);  // Hide the delete button
    }
});

// Function to add a new dividing line at a specified position
function addDividingLine(xPosition) {
    // Create a new line element
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
            selectLine(line);  // Select this line
        })
        .call(d3.drag()
            .on("drag", (event) => {
                const newX = Math.max(0, Math.min(sliderWidth, d3.pointer(event, colorSlider.node())[0]));
                line.style("left", `${newX}px`);  // Update line position while dragging
            })
        );

    // Automatically select the newly created line
    selectLine(line);

    // Store the new line in the lines array
    lines.push(line);
}

// Function to handle selecting a line
function selectLine(line) {
    // Deselect any previously selected line
    if (selectedLine) {
        selectedLine.style("border", "none");
    }

    // Select the new line
    selectedLine = line;
    selectedLine.style("border", "1px solid white");  // Highlight selected line
    toggleDeleteButton(true);  // Show the delete button
}

// Function to show or hide the delete button
function toggleDeleteButton(show) {
    d3.select("#deleteDividerButton").style("display", show ? "inline-block" : "none");
}

// Deselect line on clicking outside the color slider or any line
document.addEventListener("click", (event) => {
    if (!event.target.closest("#colorEditor")) {
        if (selectedLine) {
            selectedLine.style("border", "none");  // Reset color of selected line
            selectedLine = null;
            toggleDeleteButton(false);  // Hide the delete button
        }
    }
});
