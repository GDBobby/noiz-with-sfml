#include "TerrainGeneration.h"

void TerrainGeneration::processEvent(sf::Event& event) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed: {
		std::cout << "mouse click location - " << event.mouseButton.x << ", " << event.mouseButton.y << std::endl;
		if (event.mouseButton.button == sf::Mouse::Left) {

			switch (terrainGUI.octave.click(renderWindow)) {
				case 0: {
					if (octave > 1) {
						octave--;
						terrainGUI.octave.value.setText(std::to_string(octave));
						regenerateImage();
					}
					break;
				}				
				case 1: {
					if (octave < 128) {
						octave++;
						terrainGUI.octave.value.setText(std::to_string(octave));
						regenerateImage();
					}
					break;
				}
			}
			switch (terrainGUI.water.click(renderWindow)) {
				case 0: {
					if (water_level >= 1) {
						water_level--;
						terrainGUI.water.value.setText(std::to_string(water_level));
						beach_level--;
						grass_level--;
						regenerateImage();
					}
					break;
				}
				case 1: {
					if (water_level < 99) {
						water_level++;
						beach_level++;
						grass_level++;
						terrainGUI.water.value.setText(std::to_string(water_level));
						regenerateImage();
					}
					break;
				}
			}
			switch (terrainGUI.persistance.click(renderWindow)) {
				case 0: {
					persistance /= 2.f;
					terrainGUI.persistance.value.setText(std::format("{:.2f}", persistance));
					regenerateImage();
					break;
				}
				case 1: {
					persistance *= 2.f;
					terrainGUI.persistance.value.setText(std::format("{:.2f}", persistance));
					regenerateImage();
					break;
				}
			}
			switch (terrainGUI.lacunarity.click(renderWindow)) {
				case 0: {
					if (lacunarity > 0.1f) {
						lacunarity -= 0.1f;
						terrainGUI.lacunarity.value.setText(std::format("{:.1f}", lacunarity));
						regenerateImage();
					}
					break;
				}
				case 1: {
					lacunarity += 0.1f;
					terrainGUI.lacunarity.value.setText(std::format("{:.1f}", lacunarity));
					regenerateImage();
				
					break;
				}
			}
			if (terrainGUI.regenerateSeed.isMouseOver(renderWindow)) {
				regenerateSeed();
			}
		}
		break;
	}
	}

}

void TerrainGeneration::generateImage() {
	pixels.reserve(image_size * image_size * 4);

	for (int y = 0; y < image_size; y++) {
		for (int x = 0; x < image_size; x++) {
			auto point = noiz::Vec2f{};
			point.x = static_cast<float>(x) * step;
			point.y = static_cast<float>(y) * step;
			const uint8_t adjusted_noise = static_cast<uint8_t>((noise.at_with_octave(point, octave, persistance, lacunarity) + 1.f) * 50.f);


			if (adjusted_noise > grass_level) {
				pixels.emplace_back(grass_color.r);
				pixels.emplace_back(grass_color.g);
				pixels.emplace_back(grass_color.b);
				pixels.emplace_back(255);
			}
			else if (adjusted_noise > beach_level) {
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

	image.create(image_size, image_size, pixels.data());
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sf::Vector2f scale{ 2.f, 2.f };
	sprite.setScale(scale);
}

void TerrainGeneration::regenerateSeed() {
	noise = noiz::Noise2f{};
	regenerateImage();
}

void TerrainGeneration::regenerateImage() {
	for (int y = 0; y < image_size; y++) {
		for (int x = 0; x < image_size; x++) {
			auto point = noiz::Vec2f{};
			point.x = static_cast<float>(x) * step;
			point.y = static_cast<float>(y) * step;
			const uint8_t adjusted_noise = static_cast<uint8_t>((noise.at_with_octave(point, octave, persistance, lacunarity) + 1.f) * 50.f);


			if (adjusted_noise > grass_level) {
				pixels.at((x + y * image_size) * 4) = (grass_color.r);
				pixels.at((x + y * image_size) * 4 + 1) = (grass_color.g);
				pixels.at((x + y * image_size) * 4 + 2) = (grass_color.b);
				pixels.at((x + y * image_size) * 4 + 3) = (255);
			}
			if (adjusted_noise > beach_level) {
				pixels.at((x + y * image_size) * 4) = (beach_color.r);
				pixels.at((x + y * image_size) * 4 + 1) = (beach_color.g);
				pixels.at((x + y * image_size) * 4 + 2) = (beach_color.b);
				pixels.at((x + y * image_size) * 4 + 3) = (255);
			}
			else {
				pixels.at((x + y * image_size) * 4) = (water_color.r);
				pixels.at((x + y * image_size) * 4 + 1) = (water_color.g);
				pixels.at((x + y * image_size) * 4 + 2) = (water_color.b);
				pixels.at((x + y * image_size) * 4 + 3) = (255);
			}
		}
	}
	image.create(image_size, image_size, pixels.data());
	texture.update(image);
	sprite.setTexture(texture);
	sf::Vector2f scale{ 2.f, 2.f };
	sprite.setScale(scale);
}
