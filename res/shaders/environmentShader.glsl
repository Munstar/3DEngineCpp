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

varying vec3 texCoord0;

#if defined(VS_BUILD)
attribute vec3 position;

uniform mat4 T_projection;
uniform mat4 T_cameraRot;
uniform mat4 T_model;

void main()
{
    texCoord0 = position;
    gl_Position = T_projection * T_cameraRot * T_model * vec4(position, 1);
}

#elif defined(FS_BUILD)
DeclareFragOutput(0, vec4);

uniform sampler2D E_environmentMap;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    uv = vec2(uv.x, 1.0 - uv.y);
    return uv;
}

void main()
{
    vec2 uv = SampleSphericalMap(normalize(texCoord0));
    vec3 color = texture(E_environmentMap, uv).rgb;
	SetFragOutput(0, vec4(color,1));
}
#endif
