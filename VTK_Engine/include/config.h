#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <string>
#include <vector>

// TODO: put all the configurable values on the controller frontend

namespace Config {
	static std::unordered_map<std::string, std::string> readerConfig = {
		{"fileName", "FullHead.mhd"},
		{"fileType", "MetaImage"}
	};

	static std::unordered_map<std::string, std::string> mapperConfig = {
		{"AutoAdjustSampleDistances", "off"},
		{"SampleDistance", "0.5" },
		{"BlendMode", "IsoSurface"}
	};

	struct IsoValue {
		float value;
		std::vector<float> color;
		float opacity;
	};

	static std::vector<IsoValue> isoValues = {
		{ 500.0, {255, 255, 240}, 0.3 },
		{ 1150.0, {255, 125, 64}, 0.6 },
	};

	static std::unordered_map<std::string, std::string> volumeProperties = {
		{"Shade", "On"},
		{"InterpolationType", "Linear" }
	};
}

#endif