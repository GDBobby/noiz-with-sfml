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
		terrainGUI.persistance.value.setText(std::format("{:.2f}", persistance));
		terrainGUI.lacunarity.value.setText(std::format("{:.1f}", lacunarity));
		generateImage();

		noisePointer = &TerrainGeneration::basicNoise;
	}
	void draw() {
		renderWindow.draw(sprite);
		terrainGUI.draw(renderWindow);
	}

	void processEvent(sf::Event& event);
	TerrainGUI terrainGUI;

private:
	void generateImage();
	void regenerateImage();

	uint8_t (TerrainGeneration::*noisePointer)(noiz::Vec2f const&);

	void generateTurbulentImage();

	void add_noise_to_pixels(const uint8_t noise, int const& x, int const& y);
	void change_pixel_to_noise(uint8_t const& noise, int const& x, int const& y);


	void regenerateSeed();

	uint8_t basicNoise(noiz::Vec2f const& point);
	uint8_t turbulenceNoise(noiz::Vec2f const& point);
	uint8_t billowyNoise(noiz::Vec2f const& point);
	uint8_t rigidNoise(noiz::Vec2f const& point);
	uint8_t hybridMultifractalNoise(noiz::Vec2f const& point);
	uint8_t MultiLayerBasicNoise(noiz::Vec2f const& point);

	sf::Color grass_color{ sf::Color::Green };
	sf::Color water_color{ sf::Color::Blue };
	sf::Color beach_color{ sf::Color::Red };

	sf::RenderWindow& renderWindow;

	sf::Font& font;
	noiz::Noise2f noise;
	std::vector<sf::Uint8> pixels;

	uint16_t image_size;
	uint8_t octave{ 8 };
	float step = 0.1f;
	float persistance = 1.f;
	float lacunarity = 2.f;
	float frequency = 1.f;
	float amplitude = 1.f;

	uint8_t grass_level = 60;
	uint8_t beach_level = 55;
	uint8_t water_level = 50;

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

	//hybrid multifractal variables, i have no idea what these mean, just plugging tjhen ill learn
	float hbf_H = 0.25f;
	float hbf_offset = 0.7f;
	std::vector<double> hbf_exponent_array;

};
