#include <SFML/Graphics.hpp>
#include "TerrainGeneration.h"

int main() {
    auto window = sf::RenderWindow{ { 1280u, 720u }, "CMake SFML Project" };
    window.setFramerateLimit(144);

    TerrainGeneration terrainGeneration{ window };

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            terrainGeneration.processEvent();
        }

        window.clear();
        terrainGeneration.draw();
        window.display();
    }
}