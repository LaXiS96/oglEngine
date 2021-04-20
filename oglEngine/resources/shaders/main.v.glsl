#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTextureCoords;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

//uniform vec3 lightPosition;
uniform vec3 viewPosition;

out VS_OUT {
	vec3 fragmentPosition;
	vec2 textureCoords;
	mat3 TBN;
	//vec3 tangentLightPosition;
	vec3 tangentViewPosition;
	vec3 tangentFragmentPosition;
} vs_out;

void main() {
	vec3 T = normalize(vec3(modelMatrix * vec4(vTangent, 0.f)));
	vec3 B = normalize(vec3(modelMatrix * vec4(vBitangent, 0.f)));
	vec3 N = normalize(vec3(modelMatrix * vec4(vNormal, 0.f)));
	//if (dot(cross(N, T), B) < 0.0)
	//	T = T * -1.f;
	mat3 TBN = transpose(mat3(T, B, N));

	vs_out.fragmentPosition = vec3(modelMatrix * vec4(vPosition, 1.f));
	vs_out.textureCoords = vTextureCoords;
	vs_out.TBN = TBN;
	//vs_out.tangentLightPosition = TBN * lightPosition;
	vs_out.tangentViewPosition = TBN * viewPosition;
	vs_out.tangentFragmentPosition = TBN * vs_out.fragmentPosition;

	gl_Position = projectionMatrix * viewMatrix * vec4(vs_out.fragmentPosition, 1.f);
}
