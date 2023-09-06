#include "ScenePlay.h"

#include "SceneTitle.h"
#include "GameScene.h"

ScenePlay::ScenePlay() {}

ScenePlay::~ScenePlay() {}

void ScenePlay::Initialize() { input_ = Input::GetInstance();

}

void ScenePlay::Update(GameScene* gameScene) { 
	
	if (input_->PushKey(DIK_E)) {
		gameScene->SetScene(Scene::kTitle);
	}
}

void ScenePlay::DrawBackdrop() {}

void ScenePlay::Draw3D() {}

void ScenePlay::DrawOverlay() {}
