#include "realpch.h"

#include "Heightmapper.h"

#include "stb_image.h"
#include "stb_image_write.h"

#include "FastNoise.h"

// Constructor for an image from a file
Heightmapper::Heightmapper(const char *filename)
{
	if (read(filename))
	{
		printf("Read %s\n", filename);
		size = w * h * channels;
	}
}

// Constructor for a blank image based on inputted width, height, and channels
Heightmapper::Heightmapper(int w, int h, int channels) : w(w), h(h), channels(channels), imageExt("")
{
	imageType = ImageType::None;

	size = w * h * channels;
	data = new uint8_t[size];
}

// Constructor to copy data from an already created image
Heightmapper::Heightmapper(const Heightmapper &img) : Heightmapper(img.w, img.h, img.channels)
{
	memcpy(data, img.data, size);
}

// Destructor to free data from an image
Heightmapper::~Heightmapper()
{
	delete data;
	if (Data2D)
		delete Data2D;
}

uint8_t* Heightmapper::GetData()
{
	if (Data2D)
		Map2DTo1DArray();

	return data;
}

// Calls the stbi_load function with references to w, h, and channels as defined in the "Heightmapper.h" file
bool Heightmapper::read(const char *filename)
{
	const char *ext = strrchr(filename, '.');
	if (ext != nullptr)
	{
		if (strcmp(ext, ".png"))
		{
			imageExt = ".png";
		}
		else if (strcmp(ext, ".jpg"))
		{
			imageExt = ".png";
		}
		else if (strcmp(ext, ".bmp"))
		{
			imageExt = ".bmp";
		}
		else if (strcmp(ext, ".tga"))
		{
			imageExt = ".tga";
		}
	}

	data = stbi_load(filename, &w, &h, &channels, 4);
	return data != NULL;
}

// Maps the stb_image array to a new 2D array
void Heightmapper::Map1DTo2DArray()
{
	if (!Data2D)
		Data2D = new uint8_t[w*channels, h*channels];

	for (int x = 0; x < (w * channels); x++)
	{
		for (int y = 0; y < (h * channels); y++)
		{
			size_t place = x * (h*channels) + y;
			if (place >= size)
				return;
			Data2D[x, y] = *(data + place);
		}
	}
}

// Maps the 2D array back to the stb_image data array
void Heightmapper::Map2DTo1DArray()
{
	for (int x = 0; x < (w * channels); x++)
	{
		for (int y = 0; y < (h * channels); y++)
		{
			size_t place = x * (h*channels) + y;
			if (place < size) {
				*(data + place) = Data2D[x, y];
			}
		}
	}
}

void Heightmapper::GetPixel(int x, int y, int& r, int& g, int& b) {
	r = data[4 * (y * w + x) + 0];
    g = data[4 * (y * w + x) + 1];
    b = data[4 * (y * w + x) + 2];
}

void Heightmapper::Grayscale() {
	if (channels < 3) {
		std::cout << "Image " << this << " already has less than 3 channels!\n";
		return;
	}

	if (Data2D) {
		Map2DTo1DArray();
		std::cout << "Grayscaling the 2D array...\n";
	}

	for (int i = 0; i < size; i += channels) {
		int gray = (data[i] + data[i+1] + data[i+2]) / 3;
		memset(data + i, gray, 3);
	}

}

void Heightmapper::AddPerlinNoise2D() {
	if (!Data2D)
		Map1DTo2DArray();
	
	FastNoiseLite noise = FastNoiseLite();
	noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);

	for (int x = 0; x < (w*channels); x++) {
		for (int y = 0; y < (h*channels); y++) {
			float noiseLevel = noise.GetNoise((float)x, (float)y) + 1;
			noiseLevel = noiseLevel / 2;
			noiseLevel = noiseLevel * 255;
			size_t place = x * (h*channels) + y;
			if (place >= size)
				return;
			Data2D[x, y] = (uint8_t)noiseLevel;
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
    for(int i = 0; i < num_iterations; i++) {
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
void Heightmapper::AddSimplexNoise2D(int minimum, int maximum, float scale, int iterations)
{
	if (!Data2D)
		Map1DTo2DArray();

	std::mt19937 e(std::random_device{}());
    std::uniform_real_distribution<float> dis(0, 9999999);

	const float seed = dis(e);
	int64_t seedX = (int64_t)seed + (w * channels);
	int64_t seedY = (int64_t)seed + (h * channels);

	for (int x = 0; x < (w*channels); x++) {
		for (int y = 0; y < (h*channels); y++) {
			size_t place = (uint64_t)x * (h*channels) + y;
			if (place >= size)
				return;

			float sum = SumOctave(iterations, seedX, seedY, 0.5, scale, minimum, maximum);
			Data2D[x, y] = static_cast<uint8_t>(sum);
			seedY++;
		}
		seedY = (int64_t)seed + (h * channels);
		seedX++;
	}
}

bool Heightmapper::Write(const char* filename)
{
	char complete_name[255];
	memcpy(complete_name, filename, sizeof(filename));
	strcat(complete_name, ".png");
	return stbi_write_png(complete_name, w, h, channels, data, w * channels);
}
