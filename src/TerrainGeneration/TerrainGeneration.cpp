#include "TerrainGeneration.h"

void TerrainGeneration::processEvent(sf::Event& event) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed: {
		std::cout << "mouse click location - " << event.mouseButton.x << ", " << event.mouseButton.y << std::endl;
		if (event.mouseButton.button == sf::Mouse::Left) {

			switch (terrainGUI.octave.click(renderWindow)) {
				case 0: {
					uint8_t& octave = noise_structs[currently_selected_texture].noise_processor.octave;
					if (octave > 1) {
						octave--;
						terrainGUI.octave.value.setText(std::to_string(octave));
						regenerateImage();
					}
					break;
				}				
				case 1: {
					uint8_t& octave = noise_structs[currently_selected_texture].noise_processor.octave;
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
						beach_level--;
						grass_level--;
						terrainGUI.water.value.setText(std::to_string(water_level));
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

					float& persistence = noise_structs[currently_selected_texture].noise_processor.persistence;
					persistence /= 2.f;
					terrainGUI.persistance.value.setText(std::format("{:.2f}", persistence));
					regenerateImage();
					break;
				}
				case 1: {
					float& persistence = noise_structs[currently_selected_texture].noise_processor.persistence;
					persistence *= 2.f;
					terrainGUI.persistance.value.setText(std::format("{:.2f}", persistence));
					regenerateImage();
					break;
				}
			}
			switch (terrainGUI.lacunarity.click(renderWindow)) {
				case 0: {
					float& lacunarity = noise_structs[currently_selected_texture].noise_processor.lacunarity;
					if (lacunarity > 0.1f) {
						lacunarity -= 0.1f;
						terrainGUI.lacunarity.value.setText(std::format("{:.1f}", lacunarity));
						regenerateImage();
					}
					break;
				}
				case 1: {
					float& lacunarity = noise_structs[currently_selected_texture].noise_processor.lacunarity;
					lacunarity += 0.1f;
					terrainGUI.lacunarity.value.setText(std::format("{:.1f}", lacunarity));
					regenerateImage();
				
					break;
				}
			}
			if (terrainGUI.regenerateSeed.isMouseOver(renderWindow)) {
				regenerateSeed();
			}
			switch (terrainGUI.generationType.isMouseOver(renderWindow)) {
				case 0: {
					std::cout << "setting regeneration to basic" << std::endl;
					noisePointer = &TerrainGeneration::basicNoise;
					break;
				}
				case 1: {
					std::cout << "setting regeneration to turbulent" << std::endl;
					noisePointer = &TerrainGeneration::turbulenceNoise;
					break;
				}
				case 2: {
					std::cout << "setting regeneration to billowy" << std::endl;
					noisePointer = &TerrainGeneration::billowyNoise;
					break;
				}
				case 3: {
					std::cout << "setting regeneration to rigid" << std::endl;
					noisePointer = &TerrainGeneration::rigidNoise;
					break;
				}
				case 4: {
					std::cout << "setting regeneration to hybrid multifractal" << std::endl;
					noisePointer = &TerrainGeneration::hybridMultifractalNoise;
					break;
				}
			}
			

		}
		break;
	}
	}

}

uint8_t TerrainGeneration::basicNoise(noiz::Vec2f const& point) {
	return static_cast<uint8_t>((noise_structs[currently_selected_texture].noise_processor.basic_processing(point) + 1.f) * 100.f);
}

uint8_t TerrainGeneration::turbulenceNoise(noiz::Vec2f const& point) {
	return static_cast<uint8_t>(noise_structs[currently_selected_texture].noise_processor.turbulence_processing(point) + 3.f * 16.6666f);
}

uint8_t TerrainGeneration::billowyNoise(noiz::Vec2f const& point) {
	return static_cast<uint8_t>(noise_structs[currently_selected_texture].noise_processor.billowy_processing(point) * 100.f);
}

uint8_t TerrainGeneration::rigidNoise(noiz::Vec2f const& point) {
	return static_cast<uint8_t>(noise_structs[currently_selected_texture].noise_processor.rigid_processing(point) * 100.f);
}
uint8_t TerrainGeneration::hybridMultifractalNoise(noiz::Vec2f const& point) {
	//https://www.classes.cs.uchicago.edu/archive/2015/fall/23700-1/final-project/MusgraveTerrain00.pdf
	
	
	return static_cast<uint8_t>(noise_structs[currently_selected_texture].noise_processor.hybrid_multi_fractal_processing(point) * 25.f);

}

void TerrainGeneration::generateImage() {
	pixels.reserve(image_size * image_size * 4);

	for (int y = 0; y < image_size; y++) {
		for (int x = 0; x < image_size; x++) {
			auto point = noiz::Vec2f{};
			point.x = static_cast<float>(x);
			point.y = static_cast<float>(y);
			const uint8_t adjusted_noise = basicNoise(point);

			add_noise_to_pixels(adjusted_noise, x, y);

		}
	}

	image.create(image_size, image_size, pixels.data());
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sf::Vector2f scale{ 2.f, 2.f };
	sprite.setScale(scale);
}

void TerrainGeneration::regenerateImage() {
	for (int y = 0; y < image_size; y++) {
		for (int x = 0; x < image_size; x++) {
			auto point = noiz::Vec2f{};
			point.x = static_cast<float>(x);
			point.y = static_cast<float>(y);

			change_pixel_to_noise((this->*noisePointer)(point), x, y);
		}
	}
	image.create(image_size, image_size, pixels.data());
	texture.update(image);
	sprite.setTexture(texture);
	sf::Vector2f scale{ 2.f, 2.f };
	sprite.setScale(scale);
}

void TerrainGeneration::regenerateSeed() {
	noise_structs[currently_selected_texture].noise = noiz::Noise2f{};
	regenerateImage();
}

void TerrainGeneration::add_noise_to_pixels(const uint8_t noise, int const& x, int const& y) {
	if (noise > grass_level) {
		pixels.emplace_back(grass_color.r);
		pixels.emplace_back(grass_color.g);
		pixels.emplace_back(grass_color.b);
		pixels.emplace_back(255);
	}
	else if (noise > beach_level) {
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

void TerrainGeneration::change_pixel_to_noise(uint8_t const& noise, int const& x, int const& y) {
	if (noise > grass_level) {
		pixels.at((x + y * image_size) * 4) = (grass_color.r);
		pixels.at((x + y * image_size) * 4 + 1) = (grass_color.g);
		pixels.at((x + y * image_size) * 4 + 2) = (grass_color.b);
		pixels.at((x + y * image_size) * 4 + 3) = (255);
	}
	else if (noise > beach_level) {
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


/*
* DEPRECATED
* 

void TerrainGeneration::generateTurbulentImage() {
	pixels.reserve(image_size * image_size * 4);

	for (int y = 0; y < image_size; y++) {
		for (int x = 0; x < image_size; x++) {
			auto point = noiz::Vec2f{};
			point.x = static_cast<float>(x) * step;
			point.y = static_cast<float>(y) * step;
			const uint8_t adjusted_noise = static_cast<uint8_t>((
				turbulenceNoise(point)
				+ 3.f) * 16.6666f);

			add_noise_to_pixels(adjusted_noise, x, y);

		}
	}

	image.create(image_size, image_size, pixels.data());
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sf::Vector2f scale{ 2.f, 2.f };
	sprite.setScale(scale);
}

* DEPRECATED
void TerrainGeneration::regenerateTurbulentImage() {
	for (int y = 0; y < image_size; y++) {
		for (int x = 0; x < image_size; x++) {
			auto point = noiz::Vec2f{};
			point.x = static_cast<float>(x) * step;
			point.y = static_cast<float>(y) * step;

			//regenerateImage();
			change_pixel_to_noise((this->*noisePointer)(point), x, y);
		}
	}
	image.create(image_size, image_size, pixels.data());
	texture.update(image);
	sprite.setTexture(texture);
	sf::Vector2f scale{ 2.f, 2.f };
	sprite.setScale(scale);
}

*/