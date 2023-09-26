#include <SFML/Graphics.hpp>
#include "TerrainGeneration/TerrainGeneration.h"

int main() {
    auto window = sf::RenderWindow{ { 1280u, 720u }, "CMake SFML Project" };
    window.setFramerateLimit(144);

    sf::Font font;
    if (!font.loadFromFile("src/sfmlextensions/bahnschrift.ttf")) {
        std::cout << "Failed to load font " << std::endl;
    }
    TerrainGeneration terrainGeneration{ window, font };

    while (window.isOpen()) {
        for (auto event = sf::Event{}; window.pollEvent(event);) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            terrainGeneration.processEvent(event);
        }

        window.clear();
        terrainGeneration.draw();
        window.display();
    }
}