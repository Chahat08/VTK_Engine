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

	// DEFAULTS

	static std::vector<OpacityControlPoint> defaultOpacityPoints = {
		{0.0, 0.0},
		{4095.0, 1.0} // TODO: REMOVE HARDCODING
	};

	static std::vector<ColorGradientStopPoint> defaultColorStops = {
		{0.0, "#FF0000"},
		{4095.0, "#0000FF"}
	};

	static std::string defaultBackgroundColor = "#000000";
	static bool defaultShading = true;
	static std::string defaultInterpolationType = "Linear";

	std::vector<float> getColor(const std::string& color);
}

#endif