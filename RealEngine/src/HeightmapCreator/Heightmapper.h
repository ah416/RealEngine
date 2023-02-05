#include "stb_image.h"

#include <cstdint>
#include <iostream>
#include <cstring>
#include <cmath>
#include <string>
#include <random>

class Heightmapper {
public:
	enum class ImageType {
		None = 0, PNG, JPG
	};

	Heightmapper(const char *filename);
	Heightmapper(int w, int h, int channels);
	Heightmapper(const Heightmapper &img);
	~Heightmapper();

	uint8_t* GetData();

	// Data must be same width and height as image
	void SetData(uint8_t* d) { data = d; }

	void Grayscale();

	void AddPerlinNoise2D();

	void AddSimplexNoise2D(int minimum, int maximum, float scale, int iterations);

	bool Write(const char* filename);
private:
	bool read(const char* filename);

	void Map1DTo2DArray();

	void Map2DTo1DArray();

	void GetPixel(int x, int y, int& r, int& g, int& b);

	/* Function from Christian Maher (cmaher.github.io/posts/working-with-simplex-noise)
	 * Iterations: num of iterations of noise
	 * X and Y are the position of the noise
	 * Persistence affects the 
	 * Scale affects the size of the noise generated
	 * Low and High are the maximum values of noise to be generated
	 */
	float SumOctave(int iterations, int64_t x, int64_t y, float persistence, float scale, int low, int high) const;
private:
	uint8_t* data = NULL;
	uint8_t* Data2D = NULL;
	size_t size = 0;
	ImageType imageType;
	const char* imageExt;
	int w;
	int h;
	int channels;
};