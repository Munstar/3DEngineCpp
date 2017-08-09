/*
 * Copyright (C) 2014 Benny Bobaganoosh
 * Copyright (C) 2017 Xin Song
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "common.glh"

varying vec2 TexCoords;
varying vec3 WorldPos;
varying vec4 ShadowMapCoords;
varying mat3 TBN;

#if defined(VS_BUILD)
attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec3 tangent;

uniform mat4 T_model;
uniform mat4 T_MVP;
uniform mat4 R_lightMatrix;

void main()
{
    TexCoords = texCoord;
    ShadowMapCoords = R_lightMatrix * vec4(position, 1.0);
    WorldPos = vec3(T_model * vec4(position, 1.0));
    vec3 N = mat3(T_model) * normal;
    vec3 T = mat3(T_model) * tangent;
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);

    gl_Position = T_MVP * vec4(position, 1.0);
}

#elif defined(FS_BUILD)

#include "lighting.glh"
#include "sampling.glh"

DeclareFragOutput(0, vec4);

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

uniform vec3 C_eyePos;

uniform DirectionalLight R_directionalLight;
uniform sampler2D R_shadowMap;
uniform float R_shadowVarianceMin;
uniform float R_shadowLightBleedingReduction;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

bool InRange(float val)
{
	return val >= 0.0 && val <= 1.0;
}

float CalcShadowAmount(sampler2D shadowMap, vec4 initialShadowMapCoords)
{
	vec3 shadowMapCoords = (initialShadowMapCoords.xyz/initialShadowMapCoords.w);

	if(InRange(shadowMapCoords.z) && InRange(shadowMapCoords.x) && InRange(shadowMapCoords.y))
	{
		return SampleVarianceShadowMap(shadowMap, shadowMapCoords.xy, shadowMapCoords.z, R_shadowVarianceMin, R_shadowLightBleedingReduction);
	}
	else
	{
		return 1.0;
	}
}

void main()
{
    vec3 Lo = vec3(0.0);
    vec3 V = normalize(C_eyePos - WorldPos);
    vec3 L = normalize(R_directionalLight.direction);

    vec3 albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
    vec3 normal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;
    float metallic = texture(metallicMap, TexCoords).r;
    float roughness = texture(roughnessMap, TexCoords).r;

    vec3 N = normalize(TBN * normal);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 H = normalize(V + L);
    vec3 radiance = R_directionalLight.base.color * R_directionalLight.base.intensity;
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 nominator = NDF * G * F;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = nominator / denominator;

    vec3 Ks = F;
    vec3 Kd = vec3(1.0) - Ks;
    Kd = Kd * (1.0 - metallic);

    float NdotL = max(dot(N, L), 0.0);

    float shadow_amount = CalcShadowAmount(R_shadowMap, ShadowMapCoords);

    Lo = (Kd * albedo / PI + specular) * radiance * NdotL * (shadow_amount);



//        // HDR tonemapping
//        Lo = Lo / (Lo + vec3(1.0));
//        // gamma correct
//        Lo = pow(Lo, vec3(1.0/2.2));
	SetFragOutput(0, vec4(Lo, 1));
}
#endif
