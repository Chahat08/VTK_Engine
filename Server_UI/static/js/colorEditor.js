const sliderWidth = 500;
const sliderHeight = 30;
const colorSlider = d3.select("#colorEditor")
    .style("width", `${sliderWidth}px`)
    .style("height", `${sliderHeight}px`)
    .style("position", "relative")
    .style("border", "1px solid black")
    .style("touch-action", "none");

let selectedLine = null;
let lineColors = window.lineColors; 

function updateGradient() {
    const gradientStops = lineColors.map(({ line, color }) => {
        const percentage = (parseFloat(line.style("left")) / sliderWidth) * 100;
        return `${color} ${percentage}%`;
    }).sort((a, b) => parseFloat(a.split(' ')[1]) - parseFloat(b.split(' ')[1])).join(", ");
    colorSlider.style("background", `linear-gradient(to right, ${gradientStops})`);
}

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
            .on("start", function () {
                selectLine(line);
            })
            .on("drag", function (event) {
                let pointerX;

                if (event.sourceEvent.touches) {
                    pointerX = event.sourceEvent.touches[0].clientX - colorSlider.node().getBoundingClientRect().left;
                } else {
                    pointerX = d3.pointer(event, colorSlider.node())[0];
                }

                const newX = Math.max(0, Math.min(sliderWidth, pointerX));

                line.style("left", `${newX}px`);

                updateGradient();
                window.sendColorStops();
            })
        );

    lineColors.push({ line, color });
    selectLine(line);
    updateGradient();
    window.sendColorStops();
}

function selectLine(line) {
    if (selectedLine) {
        selectedLine.style("border", "none");
    }
    selectedLine = line;
    selectedLine.style("border", "1px solid white");
    showColorPicker();
}

function showColorPicker() {
    const lineColor = lineColors.find(lc => lc.line === selectedLine);
    if (!lineColor) return;

    const colorPicker = d3.select("#colorPicker")
        .style("display", "block")
        .property("value", lineColor.color);

    colorPicker.on("input", null).on("change", null);

    colorPicker
        .on("input", function () {
            lineColor.color = this.value;
            updateGradient();
            window.sendColorStops();
        })
        .on("change", function () {
            lineColor.color = this.value;
            updateGradient();
            window.sendColorStops();
        });
}

addDividingLine(0, "#FF0000");
addDividingLine(sliderWidth, "#0000FF");

colorSlider.on("click", function (event) {
    if (event.target === this) {  
        const [x] = d3.pointer(event);
        const leftColor = lineColors.reduce((prev, curr) =>
            parseFloat(curr.line.style("left")) < x && parseFloat(curr.line.style("left")) > parseFloat(prev.line.style("left")) ? curr : prev
        ).color;
        addDividingLine(x, leftColor);
    }
});

d3.select("#deleteDividerButton").on("click", () => {
    if (selectedLine && lineColors.length > 2) {  
        const index = lineColors.findIndex(lc => lc.line === selectedLine);
        if (index !== -1) {
            selectedLine.remove();
            lineColors.splice(index, 1);
            selectedLine = null;
            d3.select("#colorPicker").style("display", "none");
            updateGradient();
            window.sendColorStops();
        }
    }
});

d3.select("#removeAllButton").on("click", () => {
    lineColors.slice(1, -1).forEach(({ line }) => line.remove());
    lineColors = [lineColors[0], lineColors[lineColors.length - 1]];
    selectedLine = null;
    d3.select("#colorPicker").style("display", "none");
    updateGradient();
    window.sendColorStops();
});

document.addEventListener("click", (event) => {
    if (!event.target.closest("#colorEditor") && !event.target.closest("#colorPicker")) {
        if (selectedLine) {
            selectedLine.style("border", "none");
            selectedLine = null;
            d3.select("#colorPicker").style("display", "none");
        }
    }
});

function addColorStopFromIntensity(intensity) {
    const position = (intensity - intensityRange[0]) / (intensityRange[1] - intensityRange[0]) * sliderWidth;
    addDividingLine(position, "#CCCCCC"); 
}