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

#include "3DEngine.h"
#include "testing.h"

#include "components/freeLook.h"
#include "components/freeMove.h"
#include "components/physicsEngineComponent.h"
#include "components/physicsObjectComponent.h"
#include "physics/boundingSphere.h"

class TestGame : public Game
{
public:
	TestGame() {}
	
	virtual void Init(const Window& window);
protected:
private:
	TestGame(const TestGame& other) {}
	void operator=(const TestGame& other) {}
};

void TestGame::Init(const Window& window)
{
	Material bricks("bricks", Texture("bricks.jpg"), 0.0f, 0,
			Texture("bricks_normal.jpg"), Texture("bricks_disp.png"), 0.03f, -0.5f);
	Material bricks2("bricks2", Texture("bricks2.jpg"), 0.0f, 0,
			Texture("bricks2_normal.png"), Texture("bricks2_disp.jpg"), 0.04f, -1.0f);

	Material gun_pbr_material("gun_pbr");
	gun_pbr_material.SetTexture("albedoMap", Texture("Cerberus_A.tga"));
	gun_pbr_material.SetTexture("normalMap", Texture("Cerberus_N.tga"));
	gun_pbr_material.SetTexture("metallicMap", Texture("Cerberus_M.tga"));
	gun_pbr_material.SetTexture("roughnessMap", Texture("Cerberus_R.tga"));
	gun_pbr_material.SetTexture("aoMap", Texture("Cerberus_AO.tga"));

    Material gold_pbr_material("gold_pbr");
    gold_pbr_material.SetTexture("albedoMap", Texture("Gold_Glossy_00_M.tga"));
    gold_pbr_material.SetTexture("normalMap", Texture("Gold_Glossy_00_N.tga"));
    gold_pbr_material.SetTexture("metallicMap", Texture("Gold_Glossy_00_M.tga"));
    gold_pbr_material.SetTexture("roughnessMap", Texture("Gold_Glossy_00_R.tga"));
    gold_pbr_material.SetTexture("aoMap", Texture("Gold_Glossy_00_AO.tga"));

    Material floor_pbr_material("floor_pbr");
    floor_pbr_material.SetTexture("albedoMap", Texture("mahogfloor_basecolor.png"));
    floor_pbr_material.SetTexture("normalMap", Texture("mahogfloor_normal.png"));
    floor_pbr_material.SetTexture("metallicMap", Texture("Gold_Glossy_00_R.tga"));
    floor_pbr_material.SetTexture("roughnessMap", Texture("mahogfloor_roughness.png"));
    floor_pbr_material.SetTexture("aoMap", Texture("AO.png"));

	IndexedModel square;
	{
		square.AddVertex(1.0f, -1.0f, 0.0f);  square.AddTexCoord(Vector2f(1.0f, 1.0f));
		square.AddVertex(1.0f, 1.0f, 0.0f);   square.AddTexCoord(Vector2f(1.0f, 0.0f));
		square.AddVertex(-1.0f, -1.0f, 0.0f); square.AddTexCoord(Vector2f(0.0f, 1.0f));
		square.AddVertex(-1.0f, 1.0f, 0.0f);  square.AddTexCoord(Vector2f(0.0f, 0.0f));
		square.AddFace(0, 1, 2); square.AddFace(2, 1, 3);
	}
	Mesh customMesh("square", square.Finalize());

	AddToScene((new Entity(Vector3f(0, 2, -7), Quaternion(Matrix4f().InitRotationEuler(0, ToRadians(0), 0)), 1))
				->AddComponent(new CameraComponent(Matrix4f().InitPerspective(
							ToRadians(70.0f), window.GetAspect(), 0.1f, 1000.0f)))
				->AddComponent(new FreeLook(window.GetCenter()))
				->AddComponent(new FreeMove(10.0f)));


    AddToScene((new Entity(Vector3f(0, 2, 0), Quaternion(), 3))
                       ->AddComponent(new MeshRenderer(Mesh("gun.obj"),
                                                       Material("gun_pbr"))));

    AddToScene((new Entity(Vector3f(0, 0, 0), Quaternion(Matrix4f().InitRotationFromDirection(Vector3f(0,0,1), Vector3f(0, 1, 0))), 1))
                       ->AddComponent(new MeshRenderer(Mesh("floor.obj"),
                                                       Material("floor_pbr"))));

    AddToScene((new Entity(Vector3f(0, 0, -3), Quaternion(Matrix4f().InitRotationFromDirection(Vector3f(0,1,-1), Vector3f(0,0,1)))))
                       ->AddComponent(new DirectionalLight(Vector3f(1,1,1),
                                                           6, 8, 20)));

//    AddToScene((new Entity(Vector3f(-10, 10, 10), Quaternion(0,0,0,1)))
//                       ->AddComponent(new PointLight(Vector3f(1,1,1),
//                                                     300, Attenuation(0,0,1))));
//
//    AddToScene((new Entity(Vector3f(10, 10, 10), Quaternion(0,0,0,1)))
//                       ->AddComponent(new PointLight(Vector3f(1,1,1),
//                                                     300, Attenuation(0,0,1))));
//
//    AddToScene((new Entity(Vector3f(-10, 10, -10), Quaternion(0,0,0,1)))
//                       ->AddComponent(new PointLight(Vector3f(1,1,1),
//                                                     300, Attenuation(0,0,1))));
//
//    AddToScene((new Entity(Vector3f(10, 10, -10), Quaternion(0,0,0,1)))
//                       ->AddComponent(new PointLight(Vector3f(1,1,1),
//                                                     300, Attenuation(0,0,1))));


}

#include <iostream>

int main()
{
	Testing::RunAllTests();

	TestGame game;
	Window window(1280, 720, "3D Game Engine");
	RenderingEngine renderer(window);
	
	//window.SetFullScreen(true);
	
	CoreEngine engine(60, &window, &renderer, &game);
	engine.Start();
	
	return 0;
}
