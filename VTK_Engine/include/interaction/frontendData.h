#ifndef FRONTENDDATA_H
#define FRONTENDDATA_H

#include <string>
#include <vector>

namespace FrontendData {
	struct OpacityControlPoint {
		double value;
		double opacity;
		bool operator==(const OpacityControlPoint& other) const;
	};

	struct ColorGradientStopPoint {
		double position;
		std::string color;
		bool operator==(const ColorGradientStopPoint& other) const;
	};

	struct BackgroundColor {
		std::string color;
		bool operator==(const BackgroundColor& other) const;
	};

	// DEFAULTS, TODO: REMOVE HARDCODING

	static std::vector<OpacityControlPoint> defaultOpacityPoints = {
		{0.0, 0.0},
		{30.767, 0.0},
		{96.0473, 0.475},
		{74.6273, 0.0},
		{148.067, 0.655 },
		{255.0, 0.02} 
	};

	static std::vector<ColorGradientStopPoint> defaultColorStops = {
		{0.0, "#FF0000"},
		{255.0, "#0000FF"},
		{100.12728, "#009DFF"},
		{254.65728, "#080075"},
		{139.397279, "#C8FF00"}
	};

	static std::string defaultBackgroundColor = "#000000";
	static bool defaultShading = true;
	static std::string defaultInterpolationType = "Linear";
	static bool defaultAutoSampling = true;
	static float defaultSampleDistance = 0.5f;
	static std::string defaultBlendMode = "Composite";
	static float defaultIsosurfaceValue = (255.0 / 2.0);
	static int defaultInteractionPanelWidth = 300;
	static int defaultInteractionPanelHeight = 300;

	std::vector<float> getColor(const std::string& color);
}

#endif