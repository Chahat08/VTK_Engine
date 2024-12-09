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
		{500.0, 0.3},
		{1150.0, 0.6},
		{4095.0, 1.0} 
	};

	static std::vector<ColorGradientStopPoint> defaultColorStops = {
		{0.0, "#FA8072"},
		{550.0, "#FA8072"},
		{1000.0, "#FFFFF0"},
		{1150.0, "#FFFFF0"},
		{4095.0, "#FFFFF0"}
	};

	static std::string defaultBackgroundColor = "#6495ED";
	static bool defaultShading = true;
	static std::string defaultInterpolationType = "Linear";
	static bool defaultAutoSampling = true;
	static float defaultSampleDistance = 0.5f;
	static std::string defaultBlendMode = "Isosurface";
	static float defaultIsosurfaceValue = (4095.0 / 2.0);
	static int defaultInteractionPanelWidth = 300;
	static int defaultInteractionPanelHeight = 300;

	std::vector<float> getColor(const std::string& color);
}

#endif