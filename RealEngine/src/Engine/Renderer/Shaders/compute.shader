#version 430 core
layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D img_output;

uniform uint RandomFromCPU;

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash(uint x) {
	x += (x << 10u);
	x ^= (x >> 6u);
	x += (x << 3u);
	x ^= (x >> 11u);
	x += (x << 15u);
	return x;
}

// From the internet, not ah416
// Compound versions of the hashing algorithm I whipped together.
uint hash(uvec2 v) { return hash(v.x ^ hash(v.y)); }
uint hash(uvec3 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z)); }
uint hash(uvec4 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w)); }

// Again from internet, not ah416
// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct(uint m) {
	const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
	const uint ieeeOne = 0x3F800000u;	   // 1.0 in IEEE binary32

	m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
	m |= ieeeOne;                          // Add fractional part to 1.0

	float  f = uintBitsToFloat(m);         // Range [1:2]
	return f - 1.0;                        // Range [0:1]
}

// Pseudo-random value in half-open range [0:1].
float random(float x) { return floatConstruct(hash(floatBitsToUint(x))); }
float random(vec2  v) { return floatConstruct(hash(floatBitsToUint(v))); }
float random(vec3  v) { return floatConstruct(hash(floatBitsToUint(v))); }
float random(vec4  v) { return floatConstruct(hash(floatBitsToUint(v))); }

void main() {
	// base pixel colour for image
	vec4 pixel = vec4(0.0, 0.0, 0.0, 1.0);

	// get index in global work group i.e x,y position
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	vec2 float_coords = vec2(pixel_coords);

	// Camera variables
	vec3 lower_left = vec3(-0.5, -0.5, -1);
	vec3 horizontal = vec3(1.0, 0.0, 0.0);
	vec3 vertical = vec3(0.0, 1.0, 0.0);

	ivec2 dims = imageSize(img_output); // fetch image dimensions
	float x = (float(float_coords.x) / dims.x);
	float y = (float(float_coords.y) / dims.y);
	vec3 ray_o = vec3(0.0, 0.0, 0.0);
	vec3 ray_d = vec3(lower_left + x*horizontal + y*vertical - vec3(0.0, 0.0, 0.0)); // calculate direction based on x and y coordinates and camera coords

	vec3 sphere_c = vec3(0.0, 0.0, -2.0);
	float sphere_r = 1.0;

	vec3 oc = ray_o - sphere_c;
	float a = dot(oc, oc);
	float b = 2.0 * dot(oc, ray_d);
	float c = dot(oc, oc) - sphere_r * sphere_r;
	float discriminant = b * b - 4.0*a*c;

	// hit one or both sides
	if (discriminant >= 0.0) {
		float normal = (-b - sqrt(discriminant)) / (2.0 * a);
		vec3 vector_normal = normalize((ray_o + ray_d * normal) - sphere_c);
		vector_normal = 0.5 * (vector_normal + 1);
		pixel = vec4(vector_normal, 1.0);
	}
	else
	{
		vec3 unit_direction = normalize(ray_d);
		float t = 0.5 * (unit_direction.y + 1.0);
		vec3 lerped = (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.3, 0.5, 0.7);
		pixel = vec4(lerped, 1.0);
	}

	pixel = vec4(x, y, max(x, y), 1.0);
	//pixel = vec4(random(vec3(gl_GlobalInvocationID.xy, RandomFromCPU)), random(vec3(gl_GlobalInvocationID.xy, RandomFromCPU)), random(vec3(gl_GlobalInvocationID.xy, RandomFromCPU)), 1.0);

	// output to a specific pixel in the image
	imageStore(img_output, pixel_coords, pixel);
}
