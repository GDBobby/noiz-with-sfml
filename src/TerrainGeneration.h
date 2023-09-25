#pragma once

#include <SFML/Graphics.hpp>
#include <noiz/include/noiz/noise2.hpp>

#include <vector>

class TerrainGeneration {
public:
	explicit TerrainGeneration(sf::RenderWindow& window) : renderWindow{ window } {
		generateImage();
		texture.loadFromImage(image);
		sprite.setTexture(texture);
	}
	void draw() const {
		renderWindow.draw(sprite);
	}

	void processEvent() {

	
	}

	sf::RenderWindow& renderWindow;

	noiz::Noise2f noise;
	std::vector<sf::Uint8> pixels;

	uint16_t image_size = 256;
	uint8_t octave = 8;
	float step = 0.1f;
	float persistence = 1.f;
	float lacunarity = 2.f;

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
private:
	void generateImage();
	sf::Color grass_color{ sf::Color::Green };
	sf::Color water_color{ sf::Color::Blue };
	sf::Color beach_color{ sf::Color::Yellow };

};
