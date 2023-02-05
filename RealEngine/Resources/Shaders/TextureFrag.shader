#version 430 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	float shininess;
};

struct Light {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout(location = 0) out vec4 color;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;

uniform vec3 u_ViewPos;

uniform Material u_Material;
uniform Light u_Light;

void main()
{
	vec3 norm = texture(u_Material.normal, v_TexCoord).rgb;
	norm = normalize(norm * 2.0 - 1.0);

	vec3 ambient = u_Light.ambient * texture(u_Material.diffuse, v_TexCoord).rgb;

	vec3 lightDir = normalize(u_Light.position - v_FragPos);

	float diff = max(0.0, dot(norm, lightDir));
	vec3 diffuse = u_Light.diffuse * diff * texture(u_Material.diffuse, v_TexCoord).rgb;

	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = u_Light.specular * spec * vec3(0.5, 0.5, 0.5);
	//vec3 specular = u_Light.specular * spec * texture(u_Material.specular, v_TexCoord).rgb;

	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0);
}