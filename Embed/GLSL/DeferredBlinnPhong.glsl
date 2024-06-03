#ludens group 0 FrameStatic
#ludens group 1 Viewport
#ludens group 2 GBuffer

#ludens vertex

#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexUV;
layout (location = 0) out vec2 vTexUV;

void main()
{
	vTexUV = aTexUV;
	gl_Position = vec4(aPos, 0.0, 1.0);
}

#ludens fragment

#version 450 core
layout (location = 0) in vec2 vTexUV;
layout (location = 0) out vec4 fColor;

struct PointLightUBO
{
	vec4 PosRadius;
	vec4 Color;
};

layout (group = 0, binding = 0, std140) uniform LightingUBO
{
	vec4 DirLight;
	vec4 DirLightColor;
	PointLightUBO PointLights[128]; // TODO: upstream from macros
} uLightingUBO;

layout (group = 1, binding = 0, std140) uniform ViewportUBO
{
	mat4 ViewMat;
	mat4 ProjMat;
	mat4 ViewProjMat;
	vec4 ViewPos;
	vec2 Viewport;
	int PointLightStart;
	int PointLightCount;
} uViewportUBO;

layout (group = 2, binding = 0) uniform sampler2D uPosition;
layout (group = 2, binding = 1) uniform sampler2D uNormal;
layout (group = 2, binding = 2) uniform sampler2D uAlbedo;

void main()
{
	vec2 uv = vTexUV;

#ifdef LD_VULKAN
	uv.y = 1.0 - uv.y;
#endif

	vec4 albedoSpec = texture(uAlbedo, uv);
	vec3 position = texture(uPosition, uv).rgb;
	vec3 normal = texture(uNormal, uv).rgb;
	vec3 albedo = albedoSpec.rgb;
	float specular = albedoSpec.a;
	vec3 color;

	// TODO: push constant debug switch instead of using directional light W component?
	int rawTexture = int(uLightingUBO.DirLight.w);
	if (rawTexture == 1)
		color = position;
	else if (rawTexture == 2)
		color = normal;
	else if (rawTexture == 3)
		color = albedo;
	else
	{
		// single directional light
		vec3 lightPos;
		vec3 lightColor = uLightingUBO.DirLightColor.rgb;
		vec3 lightDir = normalize(-uLightingUBO.DirLight.xyz);
		vec3 viewPos = uViewportUBO.ViewPos.xyz;
		vec3 viewDir = normalize(viewPos - position);
		vec3 halfwayDir = normalize(lightDir + viewDir);
		
		// diffuse
		color = albedo * 0.1;

		color += max(dot(normal, lightDir), 0.0) * albedo * lightColor;
		color += pow(max(dot(normal, halfwayDir), 0.0), 16.0) * specular * lightColor;

		// point lights
		for (int i = 0; i < uViewportUBO.PointLightCount; i++)
		{
			int idx = uViewportUBO.PointLightStart + i;
			lightPos = uLightingUBO.PointLights[idx].PosRadius.xyz;
			lightColor = uLightingUBO.PointLights[idx].Color.rgb;
			lightDir = normalize(lightPos - position);
			halfwayDir = normalize(lightDir + viewDir);

			float distance = length(lightPos - position);
			float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance)); 

			color += max(dot(normal, lightDir), 0.0) * albedo * lightColor * attenuation * 0.6;
			color += pow(max(dot(normal, halfwayDir), 0.0), 16.0) * specular * lightColor;
		}
	}

	fColor = vec4(color, 1.0);
}