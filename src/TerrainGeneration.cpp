#include "TerrainGeneration.h"

namespace TerrainGeneration {
	void generateImage() {
		pixels.reserve(image_size * image_size * 4);

		for (int y = 0; y < image_size; y++) {
			for (int x = 0; x < image_size; x++) {
				auto point = noiz::Vec2f{};
				point.x = static_cast<float>(x) * step;
				point.y = static_cast<float>(y) * step;
				const float adjusted_noise = noise.at_with_octave(point, 2, 1.f, 2.f);


				if (adjusted_noise > 0.3f) {
					pixels.emplace_back(grass_color.r);
					pixels.emplace_back(grass_color.g);
					pixels.emplace_back(grass_color.b);
					pixels.emplace_back(255);
				}
				else if (adjusted_noise > 0.2f) {
					pixels.emplace_back(beach_color.r);
					pixels.emplace_back(beach_color.g);
					pixels.emplace_back(beach_color.b);
					pixels.emplace_back(255);
				}
				else {
					pixels.emplace_back(water_color.r);
					pixels.emplace_back(water_color.g);
					pixels.emplace_back(water_color.b);
					pixels.emplace_back(255);
				}
			}
		}

		image.create(256, 256, pixels.data());
	}
}