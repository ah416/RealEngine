#version 460 core
layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D img_output;
layout(rgba32f, binding = 1) uniform image2D img_input;

uniform float[9] u_Kernel;

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

	// this works
	accum += imageLoad(img_input, ivec2(pixel_coords.x - step_size.x, pixel_coords.y + step_size.y)) * u_Kernel[0]; // top left
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
