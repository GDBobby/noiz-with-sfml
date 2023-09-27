#pragma once

#include <SFML/Graphics.hpp>
#include <noiz/noise2.hpp>
#include <noiz/noise-processing2.hpp>

#include "TerrainGUI.h"

#include <vector>
#include <format>

class TerrainGeneration {
public:
	explicit TerrainGeneration(sf::RenderWindow& window, sf::Font& font) : image_size{ 256 }, renderWindow{ window }, font{ font }, terrainGUI{ 256, font }, currently_selected_texture{ 0 }, texture_count{ 1 } {
		//currently_selected_texture = 0;

		noise_structs.emplace_back();

		terrainGUI.octave.value.setText(std::to_string(noise_structs[currently_selected_texture].noise_processor.octave));
		terrainGUI.water.value.setText(std::to_string(water_level));

		std::string persistence_string = std::to_string(noise_structs[currently_selected_texture].noise_processor.persistence);
		terrainGUI.persistance.value.setText(std::format("{:.2f}", noise_structs[currently_selected_texture].noise_processor.persistence));
		terrainGUI.lacunarity.value.setText(std::format("{:.1f}", noise_structs[currently_selected_texture].noise_processor.lacunarity));
		terrainGUI.textureCount.value.setText("1");
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

	void generateTurbulentImage();

	void add_noise_to_pixels(const uint8_t noise, int const& x, int const& y);
	void change_pixel_to_noise(uint8_t const& noise, int const& x, int const& y);


	void regenerateSeed();

	uint8_t basicNoise(noiz::Vec2f const& point);
	uint8_t turbulenceNoise(noiz::Vec2f const& point);
	uint8_t billowyNoise(noiz::Vec2f const& point);
	uint8_t rigidNoise(noiz::Vec2f const& point);
	uint8_t hybridMultifractalNoise(noiz::Vec2f const& point);

	sf::Color grass_color{ sf::Color::Green };
	sf::Color water_color{ sf::Color::Blue };
	sf::Color beach_color{ sf::Color::Red };

	sf::RenderWindow& renderWindow;

	sf::Font& font;
	uint8_t(TerrainGeneration::* noisePointer)(noiz::Vec2f const&);

	struct Noise_Struct {
		noiz::Noise2f noise;
		noiz::Noise_Processor2f noise_processor;


		Noise_Struct() : noise{}, noise_processor{ noise } {}


		enum Noise_Type {
			noise_basic,
			noise_turbulence,
			noise_billowy,
			noise_rigid,
			noise_hmf,
		};
		Noise_Type noise_type;
	};

	std::vector<Noise_Struct> noise_structs;


	std::vector<sf::Uint8> pixels;

	uint16_t image_size;

	uint8_t grass_level = 60;
	uint8_t beach_level = 55;
	uint8_t water_level = 50;

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

	uint8_t currently_selected_texture;
	uint8_t texture_count;

};
