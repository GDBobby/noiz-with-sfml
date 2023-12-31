#pragma once

#include "../sfmlextensions/TextButton.h"
#include <iostream>

struct TerrainGUI {
	sf::Vector2f pos;
	struct Perlin_Control {
		enum Enum_Button_Type {
			BT_iterary,
			BT_shifty,
		};

		Label name;
		Textbox value;
		std::pair<ImageButton, ImageButton> buttons;

		std::pair<sf::Texture, sf::Texture> button_texture_pair;
		std::pair<sf::Sprite, sf::Sprite> button_sprite_pair;
		explicit Perlin_Control(std::string const& name, sf::Vector2f const& position, Enum_Button_Type const& button_type, sf::Font& font) :
			name{ position, 24, name, 1, font, sf::Vector2f{0.f, 30.f} },
			value{ position, 24, font, {50.f, 30.f}, sf::Color::White, 5, "0"},
			buttons{ {sf::Vector2f{16.f,16.f}, sf::Color::Black},{sf::Vector2f{16.f,16.f}, sf::Color::Black} }
		{
			if (button_type == BT_iterary) {
				button_texture_pair.first.loadFromFile("res/minus.png");
				button_texture_pair.second.loadFromFile("res/plus.png");
			}
			else if (button_type == BT_shifty) {
				button_texture_pair.first.loadFromFile("res/half.png");
				button_texture_pair.second.loadFromFile("res/double.png");
			}

			button_sprite_pair.first.setTexture(button_texture_pair.first);
			button_sprite_pair.second.setTexture(button_texture_pair.second);

			buttons.first.setPosition(sf::Vector2f{ position.x, position.y + 30.f }, button_sprite_pair.first);
			buttons.second.setPosition(sf::Vector2f{ position.x + 20.f, position.y + 30.f }, button_sprite_pair.second);
			value.setTextColor(sf::Color::Black);
			
			value.setPosition(position);
			//this->name.setPosition(sf::Vector2f{ position.x, position.y - 40.f }, 4);
			//value.setBoxTrans();
			this->name.setTextColor(sf::Color::White);
		}

		void draw(sf::RenderWindow& renderWindow) {
			buttons.first.drawTo(renderWindow);
			buttons.second.drawTo(renderWindow);
			renderWindow.draw(button_sprite_pair.first);
			renderWindow.draw(button_sprite_pair.second);
			name.drawTo(renderWindow);
			value.drawTo(renderWindow);
		}
		int8_t click(sf::RenderWindow& renderWindow) {
			if (buttons.first.isMouseOver(renderWindow)) {
				return 0;
			}
			else if (buttons.second.isMouseOver(renderWindow)) {
				return 1;
			}
			return -1;

		}
	};

	TerrainGUI(uint16_t image_size, sf::Font& font);

	void draw(sf::RenderWindow& renderWindow);

	Label title;
	Perlin_Control octave;
	Perlin_Control water;
	Perlin_Control persistance;
	Perlin_Control lacunarity;
	Perlin_Control frequency;
	Perlin_Control amplitude;

	Perlin_Control textureCount;


	comboBox generationType;

	Perlin_Control steps;

	TextButton regenerateSeed;
};