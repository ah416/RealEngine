#version 460 core
layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D img_output;
layout(rgba32f, binding = 1) uniform image2D img_input;

uniform uint RandomFromCPU;
uniform float[9] u_Kernel;

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
	vec2 step_size = 1.0 / imageSize(img_input);
	ivec2 dims = imageSize(img_output);

	//pixel = (imageLoad(img_input, pixel_coords) / 0.5) * (vec4(random(vec3(gl_GlobalInvocationID.xy, RandomFromCPU)), random(vec3(gl_GlobalInvocationID.xy, RandomFromCPU)), random(vec3(gl_GlobalInvocationID.xy, RandomFromCPU)), 1.0) / 0.5);

	// === TODO === IF THE KERNEL IS NOT SYMMETRIC, FLIP THE KERNEL BEFORE CONVOLVING

	vec4 accum = vec4(0.0);

	// this might work
	accum += imageLoad(img_input, ivec2(pixel_coords.x - step_size.x, pixel_coords.y + 1)) * u_Kernel[0]; // top left
	accum += imageLoad(img_input, ivec2(pixel_coords.x, pixel_coords.y + step_size.y)) * u_Kernel[1]; // top
	accum += imageLoad(img_input, ivec2(pixel_coords.x + step_size.x, pixel_coords.y + step_size.y)) * u_Kernel[2]; // top right
	accum += imageLoad(img_input, ivec2(pixel_coords.x - step_size.x, pixel_coords.y)) * u_Kernel[3]; // left
	accum += imageLoad(img_input, pixel_coords) * u_Kernel[4]; // center
	accum += imageLoad(img_input, ivec2(pixel_coords.x + step_size.x, pixel_coords.y)) * u_Kernel[5]; // right
	accum += imageLoad(img_input, ivec2(pixel_coords.x - step_size.x, pixel_coords.y - step_size.y)) * u_Kernel[6]; // bottom left
	accum += imageLoad(img_input, ivec2(pixel_coords.x, pixel_coords.y - step_size.y)) * u_Kernel[7]; // bottom
	accum += imageLoad(img_input, ivec2(pixel_coords.x + step_size.x, pixel_coords.y - step_size.y)) * u_Kernel[8]; // bottom right

	accum.a = 1.0;
	pixel = accum;
	//pixel = vec4(random(vec3(gl_GlobalInvocationID.xy, RandomFromCPU)), random(vec3(gl_GlobalInvocationID.xy, RandomFromCPU)), random(vec3(gl_GlobalInvocationID.xy, RandomFromCPU)), 1.0);

	// output to a specific pixel in the image
	imageStore(img_output, pixel_coords, pixel);
}
