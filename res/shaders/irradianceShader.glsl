/*
 * Copyright (C) 2014 Benny Bobaganoosh
 * Copyright (C) 2014 Xin Song
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

varying vec3 texCoord0;

#if defined(VS_BUILD)
attribute vec3 position;

uniform mat4 T_projection;
uniform mat4 T_cameraRot;
uniform mat4 T_model;

void main()
{
    texCoord0 = position;
    vec4 pos = T_projection * T_cameraRot * T_model * vec4(position, 1);
    gl_Position = pos;
}

#elif defined(FS_BUILD)
DeclareFragOutput(0, vec4);
uniform samplerCube environmentCubeMap;

void main()
{
    vec3 N = normalize(texCoord0);
    vec3 irradiance = vec3(0.0);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(N, up);
    up = cross(N, right);

    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

            vec3 envColor = texture(environmentCubeMap, sampleVec).rgb;

            irradiance += envColor * cos(theta) * sin(theta);
            nrSamples++;
        }

    }

    irradiance = PI * irradiance * (1.0 / float(nrSamples));

	SetFragOutput(0, vec4(irradiance, 1.0));
}
#endif
