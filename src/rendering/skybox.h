/*
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
#ifndef SKYBOX_H_INCLUDED
#define SKYBOX_H_INCLUDED

#include "mesh.h"
#include "texture.h"
#include "material.h"
#include "shader.h"

class Skybox {
public:
    Skybox(const std::string& skyboxName, float scale = 50.0f) :
            m_mesh("skybox.obj"),
            m_material("skybox"),
            m_transform(Transform(Vector3f(0,0,0), Quaternion(0,0,0,1), scale))
    {
        m_material.SetTexture("skyboxCubeMap", Texture(skyboxName, GL_TEXTURE_CUBE_MAP));
    }

    void Render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
    {
        shader.Bind();
        shader.UpdateUniforms(Transform(Vector3f(0,0,0), Quaternion(0,0,0,1), 50.0f), m_material, renderingEngine, camera);
        glCullFace(GL_FRONT);
        m_mesh.Draw();
        glCullFace(GL_BACK);
    }
protected:
private:
    Mesh m_mesh;
    Material m_material;
    Transform m_transform;
};


#endif //SKYBOX_H_INCLUDED
