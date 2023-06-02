#version 460 core
layout(local_size_x = 4, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D img_output;
layout(rgba32f, binding = 1) uniform image2D img_input;

uniform mat3 u_Gx;
uniform mat3 u_Gy;

ivec2 kpos(int index) {
    return ivec2[9] (
        ivec2(-1, -1), ivec2(0, -1), ivec2(1, -1),
        ivec2(-1, 0), ivec2(0, 0), ivec2(1, 0), 
        ivec2(-1, 1), ivec2(0, 1), ivec2(1, 1)
    )[index];
}

mat3[3] sample_region(ivec2 coords) {
	vec4[9] region;

	for (int i = 0; i < 9; i++) {
		region[i] = imageLoad(img_input, coords + kpos(i));
	}

	mat3[3] matrix_region;
	for (int i = 0; i < 3; i++) {
		matrix_region[i] = mat3(
			region[0][i], region[1][i], region[2][i],
        	region[3][i], region[4][i], region[5][i],
        	region[6][i], region[7][i], region[8][i]
		);
	}

	return matrix_region;
}
vec3 convolve(mat3 kernel, ivec2 coords) {
	vec3 pixel;

	mat3[3] region = sample_region(coords);

	for (int i = 0; i < 3; i++) {
		mat3 rc = region[i];
		mat3 c = matrixCompMult(kernel, rc);
		float channel = c[0][0] + c[1][0] + c[2][0]
					  + c[0][1] + c[1][1] + c[2][1]
					  + c[0][2] + c[1][2] + c[2][2];

		pixel[i] = channel;
	}

	return pixel;
}

void main() {
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

	vec3 Gx = convolve(u_Gx, pixel_coords);
	vec3 Gy = convolve(u_Gy, pixel_coords);

	vec3 G = sqrt(vec3(pow(Gx.x, 2), pow(Gx.y, 2), pow(Gx.z, 2)) + vec3(pow(Gy.x, 2), pow(Gy.y, 2), pow(Gy.z, 2)));

	vec4 pixel = vec4(G, 1.0);
	//pixel = pixel * 0.5;

	imageStore(img_output, pixel_coords, pixel);
}
