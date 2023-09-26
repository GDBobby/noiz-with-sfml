#pragma once

#include <SFML/Graphics.hpp>
#include <noiz/include/noiz/noise2.hpp>

#include "TerrainGUI.h"

#include <vector>
#include <format>

class TerrainGeneration {
public:
	explicit TerrainGeneration(sf::RenderWindow& window, sf::Font& font) : image_size{ 256 }, renderWindow { window }, font{ font }, terrainGUI{ 256, font} {
		terrainGUI.octave.value.setText(std::to_string(octave));
		terrainGUI.water.value.setText(std::to_string(water_level));

		std::string persistance_string = std::to_string(persistance);
		terrainGUI.persistance.value.setText(std::format("{:.1f}", persistance));
		terrainGUI.lacunarity.value.setText(std::to_string(lacunarity));
		generateImage();
	}
	void draw() {
		renderWindow.draw(sprite);
		terrainGUI.draw(renderWindow);
	}

	void processEvent(sf::Event& event);
	TerrainGUI terrainGUI;

	sf::RenderWindow& renderWindow;

	sf::Font& font;
	noiz::Noise2f noise;
	std::vector<sf::Uint8> pixels;

	uint16_t image_size;
	uint8_t octave{ 8 };
	float step = 0.1f;
	float persistance = 1.f;
	float lacunarity = 2.f;

	uint8_t water_level = 50;
	uint8_t beach_level = 55;
	uint8_t grass_level = 60;

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
private:
	void generateImage();
	void regenerateImage();
	void regenerateSeed();
	sf::Color grass_color{ sf::Color::Green };
	sf::Color water_color{ sf::Color::Blue };
	sf::Color beach_color{ sf::Color::Yellow };

};
