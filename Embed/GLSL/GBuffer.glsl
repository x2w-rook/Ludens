#ludens group 0 Viewport
#ludens group 1 Material

#ludens vertex

#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexUV;
layout (location = 4) in vec4 aModel[3];

layout (location = 0) out vec3 vPos;
layout (location = 1) out vec3 vNormal;
layout (location = 2) out vec2 vTexUV;
layout (location = 3) out mat3 vTBN;


layout (group = 0, binding = 0, std140) uniform Viewport
{
	mat4 View;
	mat4 Proj;
	mat4 ViewProj;
	vec4 CameraPos;
} uViewport;

void main()
{
	mat4 modelMat; 
	modelMat[0] = vec4(aModel[0].x, aModel[1].x, aModel[2].x, 0.0);
	modelMat[1] = vec4(aModel[0].y, aModel[1].y, aModel[2].y, 0.0);
	modelMat[2] = vec4(aModel[0].z, aModel[1].z, aModel[2].z, 0.0);
	modelMat[3] = vec4(aModel[0].w, aModel[1].w, aModel[2].w, 1.0);

	mat3 normalMat = transpose(inverse(mat3(modelMat)));

	vPos = (modelMat * vec4(aPos, 1.0)).xyz;   // world position
	vNormal = normalize(normalMat * aNormal);  // world normal
	vTexUV = aTexUV;

	vec3 T = normalize(normalMat * aTangent);
	vec3 N = vNormal;
	vec3 B = cross(N, T);
	vTBN = mat3(T, B, N);

	gl_Position = uViewport.ViewProj * vec4(vPos, 1.0);
}

#ludens fragment


#version 450 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexUV;
layout (location = 3) in mat3 vTBN;

layout (location = 0) out vec4 fPos;
layout (location = 1) out vec4 fNormal;
layout (location = 2) out vec4 fAlbedoSpec;

layout (group = 0, binding = 0, std140) uniform Viewport
{
	mat4 View;
	mat4 Proj;
	mat4 ViewProj;
	vec4 CameraPos;
} uViewport;

layout (group = 1, binding = 0, std140) uniform Material
{
	vec3 Ambient;
	float UseAmbientTexture;
	vec3 Albedo;
	float UseAlbedoTexture;
	vec3 Specular;
	float UseSpecularTexture;
	vec4 UseNormalTexture;
} uMaterial;

layout (group = 1, binding = 1) uniform sampler2D uAmbient;
layout (group = 1, binding = 2) uniform sampler2D uAlbedo;
layout (group = 1, binding = 3) uniform sampler2D uSpecular;
layout (group = 1, binding = 4) uniform sampler2D uNormals;

float avg(vec3 v)
{
	return (v.r + v.g + v.b) / 3.0f;
}

void main()
{
	float specular = avg(uMaterial.Specular);
	vec4 albedoSpec = vec4(uMaterial.Albedo, specular);
	vec3 normal = normalize(vNormal);

	if (uMaterial.UseAlbedoTexture > 0.0)
	{
		albedoSpec.rgb = texture(uAlbedo, vTexUV).rgb;
	}

	if (uMaterial.UseSpecularTexture > 0.0)
	{
		albedoSpec.a = avg(texture(uSpecular, vTexUV).rgb);
	}

	if (uMaterial.UseNormalTexture.x > 0.0)
	{
		// normal mapping from tangent space to world space
		normal = texture(uNormals, vTexUV).rgb;
		normal = normalize(normal * 2.0 - 1.0);   
		normal = normalize(vTBN * normal);
	}

	fPos = vec4(vPos, 1.0);
	fNormal = vec4(normal, 1.0);
	fAlbedoSpec = albedoSpec;
}