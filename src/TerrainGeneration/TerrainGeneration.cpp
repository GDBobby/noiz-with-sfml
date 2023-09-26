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
				case 5: {
					std::cout << "setting regeneration to hybrid multifractal" << std::endl;
					noisePointer = &TerrainGeneration::MultiLayerBasicNoise;
					break;
				}

					  /*
					  
						generationType.pushback("Rigid");
						generationType.pushback("Hybrid Multifractal");
						generationType.pushback("MultiLayer Basic");
						*/
			}
			

		}
		break;
	}
	}

}

uint8_t TerrainGeneration::basicNoise(noiz::Vec2f const& point) {
	return static_cast<uint8_t>((noise.at_with_octave(point, octave, persistance, lacunarity) + 1.f) * 50.f);
}

uint8_t TerrainGeneration::turbulenceNoise(noiz::Vec2f const& point) {
	float amplitude = this->amplitude;
	float frequency = this->frequency;
	
		float sum = 0.f;
		float normalizer = 0.f;
		for (int i = 0; i < octave; i++) {
			sum += noise.at(noiz::Vec2f{.x = point.x * frequency, .y = point.y * frequency})* amplitude;
			normalizer += amplitude;
			amplitude *= persistance;
			frequency *= lacunarity;
		}
		//normally return sum;
		//but im doing some additional adjustment in this function
		return static_cast<uint8_t>((sum + 3.f) * 16.6666f);
}

uint8_t TerrainGeneration::billowyNoise(noiz::Vec2f const& point) {
	//return 1.f - abs(noise.at(point));
	return static_cast<uint8_t>(std::abs(noise.at(point)) * 100.f);
}

uint8_t TerrainGeneration::rigidNoise(noiz::Vec2f const& point) {
	return static_cast<uint8_t>((1.f - std::abs(noise.at(point))) * 100.f);
}
uint8_t TerrainGeneration::hybridMultifractalNoise(noiz::Vec2f const& point) {
	//https://www.classes.cs.uchicago.edu/archive/2015/fall/23700-1/final-project/MusgraveTerrain00.pdf
	
	//double HybridMultifractal( Vector point, double H, double lacunarity,
	//double octaves, double offset )
	noiz::Vec2f tempPoint = point;

		double frequency, result, signal, weight, remainder;
		//double Noise3(); what is this??
		/* precompute and store spectral weights */
		if (hbf_exponent_array.size() != octave) {
			hbf_exponent_array.resize(octave);
			/* seize required memory for exponent_array */
			//exponent_array = (double*)malloc(octave * sizeof(double));
			frequency = 1.0;
			for (int i = 0; i < octave; i++) {
				/* compute weight for each frequency */
				hbf_exponent_array[i] = pow(frequency, -hbf_H);
				frequency *= lacunarity;
			}
		}
		/* get first octave of function */
		result = (noise.at(tempPoint) + hbf_offset) * hbf_exponent_array[0];
		weight = result;
		/* increase frequency */
		tempPoint.x *= lacunarity;
		tempPoint.y *= lacunarity;
		/* spectral construction inner loop, where the fractal is built */
		for (int i = 1; i < octave; i++) {
			/* prevent divergence */
			if (weight > 1.0) weight = 1.0;
			/* get next higher frequency */
			signal = (noise.at(tempPoint) + hbf_offset) * hbf_exponent_array[i];
			/* add it in, weighted by previous freq's local value */
			result += weight * signal;
			/* update the (monotonically decreasing) weighting value */
			/* (this is why H must specify a high fractal dimension) */
			weight *= signal;
			/* increase frequency */
			tempPoint.x *= lacunarity;
			tempPoint.y *= lacunarity;
		} /* for */
		/* take care of remainder in “octaves” */
		
		/* octave is currently an int, may swap for this
		remainder = octave - (int)octave;
		if (remainder)
			// “i” and spatial freq. are preset in loop above
			result += remainder * noise.at(point) * hbf_exponent_array[octave - 1];
		*/
		static float biggestValue = 0.f;
		static float smallestValue = 1.f;
		if (result > biggestValue) {
			biggestValue = result;
			std::cout << "biggest result : " << result << std::endl;
		}
		else if (result < smallestValue) {
			smallestValue = result;
			std::cout << "smallest result : " << result << std::endl;
		}

		//return result;
		return static_cast<uint8_t>(result * 25.0);

}
uint8_t TerrainGeneration::MultiLayerBasicNoise(noiz::Vec2f const& point) {
	return 0;
}

void TerrainGeneration::generateImage() {
	pixels.reserve(image_size * image_size * 4);

	for (int y = 0; y < image_size; y++) {
		for (int x = 0; x < image_size; x++) {
			auto point = noiz::Vec2f{};
			point.x = static_cast<float>(x) * step;
			point.y = static_cast<float>(y) * step;
			const uint8_t adjusted_noise = static_cast<uint8_t>((noise.at_with_octave(point, octave, persistance, lacunarity) + 1.f) * 50.f);

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
			point.x = static_cast<float>(x) * step;
			point.y = static_cast<float>(y) * step;

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
	noise = noiz::Noise2f{};
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
	if (noise > beach_level) {
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