#version 330 core

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in VS_OUT {
	vec3 fragmentPosition;
	vec2 textureCoords;
	mat3 TBN;
	//vec3 tangentLightPosition;
	vec3 tangentViewPosition;
	vec3 tangentFragmentPosition;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

//uniform vec3 viewPosition;
uniform DirectionalLight dirLight;

out vec4 fragmentColor;

void main() {
	vec3 normal = texture(texture_normal1, fs_in.textureCoords).rgb;
	normal = normalize(normal * 2.f - 1.f);

	// Base color
	vec3 color = texture(texture_diffuse1, fs_in.textureCoords).rgb;

	// Ambient component
	vec3 ambient = dirLight.ambient * color;

	// Diffuse component
	vec3 lightDir = normalize(fs_in.TBN * -dirLight.direction);
	//vec3 lightDir = normalize(-dirLight.direction);
	float diff = max(dot(lightDir, normal), 0.f);
	vec3 diffuse = dirLight.diffuse * diff * color;

	// Specular component
	vec3 viewDirection = normalize(fs_in.tangentViewPosition - fs_in.tangentFragmentPosition);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDirection, reflectDir), 0.f), 32.f); // 32 is material.shininess
	vec3 specular = dirLight.specular * spec * vec3(texture(texture_specular1, fs_in.textureCoords));

	//fragmentColor = vec4(ambient + diffuse + specular, 1.f);
	fragmentColor = vec4(fs_in.tangentFragmentPosition, 1.f);
}
