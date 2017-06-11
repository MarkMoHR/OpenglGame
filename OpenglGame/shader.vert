#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 tangentFragPos;
out vec3 tangentLightPos;
out vec3 tangentViewPos;

out LIGHT {
	vec3 tangentLightPos1;
	vec3 tangentLightPos2;
	vec3 tangentLightPos3;
	vec3 tangentLightPos4;
}L;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 lightPos3;
uniform vec3 lightPos4;

uniform vec3 viewPos;

void main()
{

	FragPos = vec3(projection * view * model * vec4(position, 1.0f));
	TexCoords = texCoords;   
	
	vec3 LightPos1 = vec3(projection * view * model * vec4(lightPos1, 1.0));
	vec3 LightPos2 = vec3(projection * view * model * vec4(lightPos2, 1.0));
	vec3 LightPos3 = vec3(projection * view * model * vec4(lightPos3, 1.0));
	vec3 LightPos4 = vec3(projection * view * model * vec4(lightPos4, 1.0));
	
    mat3 normalMatrix = transpose(inverse(mat3(projection * view * model)));
	vec3 T = normalize(normalMatrix * tangent);
	//vec3 B = normalize(normalMatrix * bitangent);
	vec3 N = normalize(normalMatrix * normal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
    
	mat3 TBN = transpose(mat3(T, B, N));
	
	L.tangentLightPos1 = TBN * LightPos1;
	L.tangentLightPos2 = TBN * LightPos2;
	L.tangentLightPos3 = TBN * LightPos3;
	L.tangentLightPos4 = TBN * LightPos4;
	
	tangentViewPos = TBN * viewPos;
	tangentFragPos = TBN * FragPos;
    
	gl_Position = projection * view *  model * vec4(position, 1.0f);
}
