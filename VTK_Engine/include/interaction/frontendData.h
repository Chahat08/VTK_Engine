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

	static std::vector<OpacityControlPoint> opacityPoints = {
		{0.0, 0.0},
		{4095.0, 1.0} // TODO: REMOVE HARDCODING
	};

	static std::vector<ColorGradientStopPoint> colorStops = {
		{0.0, "#FF0000"},
		{4095.0, "#0000FF"}
	};

	std::vector<int> getColor(const std::string& color);
}

#endif