#version 330 core
out vec4 color;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 tangentFragPos;
in vec3 tangentLightPos;
in vec3 tangentViewPos;

in LIGHT {
	vec3 tangentLightPos1;
	vec3 tangentLightPos2;
	vec3 tangentLightPos3;
	vec3 tangentLightPos4;
}L;

float lightStrengths[4] = float[](0.85f, 0.05f, 0.05f, 0.05f);

uniform sampler2D diffuse_map;

uniform sampler2D bump_map;

uniform sampler2D spec_map;

vec4 CalcPointLight(vec3 pos, vec3 normal, vec3 diffColor);

void main()
{

	vec3 normal = normalize(texture2D(bump_map, TexCoords).rgb * 2.0 - 1.0);
	
	vec3 diffColor = texture2D(diffuse_map, TexCoords).rgb;
	
	color = CalcPointLight(L.tangentLightPos1, normal, diffColor) * lightStrengths[0];
	color += CalcPointLight(L.tangentLightPos2, normal, diffColor) * lightStrengths[1];
	color += CalcPointLight(L.tangentLightPos3, normal, diffColor) * lightStrengths[2];
	color += CalcPointLight(L.tangentLightPos4, normal, diffColor) * lightStrengths[3];
}

vec4 CalcPointLight(vec3 pos, vec3 normal, vec3 diffColor) {
	
    // Ambient
    float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * diffColor;

    // Diffuse
    float diffuseStrength = 0.4f;
	vec3 lightDir = normalize(tangentLightPos - tangentFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseStrength * diffColor;

    // Specular
    float specularGlobalStrngth = 0.7f;
	float specularStrength = texture2D(spec_map, TexCoords).r;
	vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 2);
    vec3 specular = vec3(specularGlobalStrngth * specularStrength * spec);
	
	return vec4((ambient + diffuse + specular), 1.0);
}
