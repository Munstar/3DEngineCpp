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

#include "camera.h"
#include "renderingEngine.h"

#include "../core/coreEngine.h"

Matrix4f Camera::GetViewProjection() const
{
	//This comes from the conjugate rotation because the world should appear to rotate
	//opposite to the camera's rotation.
	Matrix4f cameraRotation = GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix();
	Matrix4f cameraTranslation;
	
	//Similarly, the translation is inverted because the world appears to move opposite
	//to the camera's movement.
	cameraTranslation.InitTranslation(GetTransform().GetTransformedPos() * -1);
	
	return m_projection * cameraRotation * cameraTranslation;
}

Matrix4f Camera::GetView() const
{
	Matrix4f cameraRotation = GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix();
	Matrix4f cameraTranslation;
	cameraTranslation.InitTranslation(GetTransform().GetTransformedPos() * -1);

	return cameraRotation * cameraTranslation;
}

Matrix4f Camera::GetProjection() const
{
	return m_projection;
}

Matrix4f Camera::GetCameraRotation() const
{
	Matrix4f cameraRotation = GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix();
	return cameraRotation;
}

Camera Camera::GetCubeCamera(unsigned int index)
{
	index = index % 6;

    static Matrix4f projection = Matrix4f().InitPerspective(ToRadians(90.0), 1.0, 0.1f, 100.0f);
    static Transform transform = Transform();
    switch (index)
    {
        case 0:
            transform.LookAt(Vector3f(-1.0f,  0.0f,  0.0f), Vector3f(0.0f, -1.0f,  0.0f));
            break;
        case 1:
            transform.LookAt(Vector3f( 1.0f,  0.0f,  0.0f), Vector3f(0.0f, -1.0f,  0.0f));
            break;
        case 2:
            transform.LookAt(Vector3f( 0.0f,  1.0f,  0.0f), Vector3f(0.0f,  0.0f,  1.0f));
            break;
        case 3:
            transform.LookAt(Vector3f( 0.0f, -1.0f,  0.0f), Vector3f(0.0f,  0.0f, -1.0f));
            break;
        case 4:
            transform.LookAt(Vector3f( 0.0f,  0.0f,  1.0f), Vector3f(0.0f, -1.0f,  0.0f));
            break;
        case 5:
            transform.LookAt(Vector3f( 0.0f,  0.0f, -1.0f), Vector3f(0.0f, -1.0f,  0.0f));
            break;
        default:
            break;
    }

    return Camera(projection, &transform);
}

void CameraComponent::AddToEngine(CoreEngine* engine) const
{
	//TODO: This is probably not the correct solution in the case of multiple cameras,
	//and should be investigated in the future.
	engine->GetRenderingEngine()->SetMainCamera(m_camera);
}

void CameraComponent::SetParent(Entity* parent)
{
	EntityComponent::SetParent(parent);
	
	//The camera's transform is initialized here because this is the first point where
	//there is a parent object with a transform.
	m_camera.SetTransform(GetTransform());
}

void CameraComponent::Render(const Shader &shader, const RenderingEngine &renderingEngine, const Camera &camera) const {
    if(renderingEngine.m_renderCamera)
    {
        Shader default_shader("basicShader");
        Mesh cube("cube.obj");
        Material default_material("default");
        default_shader.Bind();
        default_shader.UpdateUniforms(GetTransform(), default_material, renderingEngine, camera);
        cube.Draw();
    }
}
