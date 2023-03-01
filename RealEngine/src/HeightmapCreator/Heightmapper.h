#include <cstdint>
#include <iostream>
#include <cstring>
#include <cmath>
#include <string>
#include <random>

#include <Engine/Tools/Format.h>

struct Image {
	uint32_t* Data;
	int Width, Height, Channels;

	Image() {
		Data = NULL;
		Width = 0;
		Height = 0;
		Channels = 0;
	}

	Image(const char* filename) {
		if (!Read(filename)) {
			std::cerr << fmt::format("Failed to load image from {}!", filename);
		}
	}

	Image(int w, int h, int c) {
		Data = (uint32_t*)malloc(sizeof(uint32_t) * w * h * 4);
		Width = w;
		Height = h;
		Channels = c;
	}

	~Image() {
		free(Data);
	}
	
	bool Read(const char* filename);

	bool Write(const char* filename);
};

class Heightmapper {
public:
	Heightmapper(const char *filename);
	Heightmapper(int w, int h, int channels);
	Heightmapper(const Heightmapper &img);
	~Heightmapper();

	void AddPerlinNoise();

	void AddSimplexNoise(int minimum, int maximum, float scale, int iterations);

	Image GetImage() const { return m_Image; }
private:
	void GetPixel(int x, int y, uint8_t& r, uint8_t& g, uint8_t& b);

	/* Function from Christian Maher (cmaher.github.io/posts/working-with-simplex-noise)
	 * Iterations: num of iterations of noise
	 * X and Y are the position of the noise
	 * Persistence affects the 
	 * Scale affects the size of the noise generated
	 * Low and High are the maximum values of noise to be generated
	 */
	float SumOctave(int iterations, int64_t x, int64_t y, float persistence, float scale, int low, int high) const;

private:
	Image m_Image;
};