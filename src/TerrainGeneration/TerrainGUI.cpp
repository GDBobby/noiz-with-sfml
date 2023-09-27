#include "TerrainGUI.h"

TerrainGUI::TerrainGUI(uint16_t image_size, sf::Font& font) :
	title{ sf::Vector2f{static_cast<float>(image_size * 4), 0.f}, 24, "Terrain Generation", 0, font },
	octave{ "Octave", sf::Vector2f{static_cast<float>(image_size) + 400.f, 100.f}, Perlin_Control::BT_iterary, font },
	water{ "Water Level", sf::Vector2f{ static_cast<float>(image_size) + 500.f, 300.f }, Perlin_Control::BT_iterary, font },
	persistance{ "Persistance", sf::Vector2f{ static_cast<float>(image_size) + 400.f, 200.f }, Perlin_Control::BT_shifty, font },
	lacunarity{ "Lacunarity", sf::Vector2f{ static_cast<float>(image_size) + 400.f, 300.f }, Perlin_Control::BT_iterary, font },
	amplitude{ "Amplitude", sf::Vector2f{ static_cast<float>(image_size) + 400.f, 400.f }, Perlin_Control::BT_iterary, font },
	frequency{ "Frequency", sf::Vector2f{ static_cast<float>(image_size) + 400.f, 500.f }, Perlin_Control::BT_iterary, font },
	regenerateSeed{ "new seed", sf::Vector2f{static_cast<float>(image_size) + 400.f, 20.f}, 24, sf::Color{20,20,20,255}, sf::Color::Red, font, 2.f },
	generationType{ sf::Vector2f{ static_cast<float>(image_size) + 600.f, 100.f }, 24, font, 200},
	steps{ "Steps", sf::Vector2f{ static_cast<float>(image_size) + 500.f, 200.f }, Perlin_Control::BT_iterary, font },
	textureCount{ "Texture Count", sf::Vector2f{ static_cast<float>(image_size) + 500.f, 100.f }, Perlin_Control::BT_iterary, font }
{
	title.setTextColor(sf::Color::White);
	generationType.pushback("Basic");
	generationType.pushback("fBm");
	generationType.pushback("Billowy");
	generationType.pushback("Rigid");
	generationType.pushback("Hybrid Multifractal");
}


void TerrainGUI::draw(sf::RenderWindow& renderWindow) {
	//std::cout << "pls" << std::endl;
	title.drawTo(renderWindow);
	octave.draw(renderWindow);
	water.draw(renderWindow);
	persistance.draw(renderWindow);
	lacunarity.draw(renderWindow);
	if (generationType.getSelection() == 1) {
		amplitude.draw(renderWindow);
		frequency.draw(renderWindow);
	}
	generationType.draw(renderWindow);
	regenerateSeed.draw(renderWindow);
	textureCount.draw(renderWindow);
}