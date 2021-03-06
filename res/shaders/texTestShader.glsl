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

varying vec2 texCoord0;

#if defined(VS_BUILD)
attribute vec3 position;
attribute vec2 texCoord;

uniform mat4 T_MVP;

void main()
{
    texCoord0 = texCoord;
    vec4 pos = T_MVP * vec4(position, 1);
    gl_Position = pos;
}

#elif defined(FS_BUILD)
DeclareFragOutput(0, vec4);

uniform sampler2D diffuse;

void main()
{
    vec4 color = texture(diffuse, texCoord0);
//    color += vec4(1,1,1,1);
	SetFragOutput(0, color);
}
#endif
