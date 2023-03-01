#include "realpch.h"

#include "Heightmapper.h"

#include "stb_image.h"
#include "stb_image_write.h"

#include "FastNoise.h"

bool Image::Read(const char* filename)
{
	Data = (uint32_t*)stbi_load(filename, &Width, &Height, &Channels, 4);
	return Data != NULL;
}

bool Image::Write(const char* filename)
{
	return stbi_write_png(filename, Width, Height, Channels, Data, Width * 4);
}

// Constructor for an image from a file
Heightmapper::Heightmapper(const char* filename)
{
	m_Image = Image(filename);
}

// Constructor for a blank image based on inputted width, height, and channels
Heightmapper::Heightmapper(int w, int h, int channels)
{
	m_Image = Image(w, h, channels);
}

// Constructor to copy data from an already created image
Heightmapper::Heightmapper(const Heightmapper& img)
{
	m_Image = img.GetImage();
}

Heightmapper::~Heightmapper()
{
}

void Heightmapper::GetPixel(int x, int y, uint8_t& r, uint8_t& g, uint8_t& b) {
	r = m_Image.Data[x + y * m_Image.Width];
	g = m_Image.Data[x + y * m_Image.Width] >> 8;
	b = m_Image.Data[x + y * m_Image.Width] >> 16;
}

void Heightmapper::AddPerlinNoise() {
	FastNoiseLite noise = FastNoiseLite();
	noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);

	for (int x = 0; x < m_Image.Width; x++) {
		for (int y = 0; y < m_Image.Height; y++) {
			float noiseLevel = noise.GetNoise((float)x, (float)y);

			// Convert the noise level from -1 -> 1 to 0 -> 255
			noiseLevel = noiseLevel + 1;
			noiseLevel = noiseLevel / 2;
			noiseLevel = noiseLevel * 255;

			m_Image.Data[x + y * m_Image.Width] = static_cast<uint8_t>(noiseLevel);
		}
	}
}

// Function from Christian Maher (cmaher.github.io/posts/working-with-simplex-noise)
float Heightmapper::SumOctave(int num_iterations, int64_t x, int64_t y, float persistence, float scale, int low, int high) const
{
	FastNoiseLite noiseGen = FastNoiseLite();
	noiseGen.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);

	float maxAmp = 0;
	float amp = 1;
	float freq = scale;
	float noise = 0;

	// add successively smaller, higher-frequency terms
	for (int i = 0; i < num_iterations; i++) {
		noise += noiseGen.GetNoise(x * freq, y * freq) * amp;
		maxAmp += amp;
		amp *= persistence;
		freq *= 2;
	}

	// take the average value of the iterations
	noise /= maxAmp;

	// normalize the result
	noise = noise * (high - low) / 2 + (high + low) / 2;

	return noise;
}

// Generates simplex noise (OpenSimplex2) using a 2D array (x, y)
void Heightmapper::AddSimplexNoise(int minimum, int maximum, float scale, int iterations)
{
	for (int x = 0; x < m_Image.Width; x++) {
		for (int y = 0; y < m_Image.Height; y++) {
			float sum = SumOctave(iterations, x, y, 0.5, scale, minimum, maximum);
			m_Image.Data[x + y * m_Image.Width] = static_cast<uint8_t>(sum);
		}
	}
}
