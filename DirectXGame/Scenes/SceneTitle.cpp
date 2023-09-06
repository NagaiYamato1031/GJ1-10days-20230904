#include "SceneTitle.h"

#include "Objects/Player.h"
#include "GameScene.h"
#include "ScenePlay.h"

SceneTitle::~SceneTitle() {

}

void SceneTitle::Initialize() { 
	objectManager_ = ObjectManager::GetInstance();

	input_ = Input::GetInstance();
	
	Player* player_ = new Player();
	player_->Initialize();
	

	objectManager_->AddObject(kPlayer, player_);

	// プレイヤーのテクスチャ
	objectManager_->LoadTexture("uv", "uvChecker.png", kPlayer);

	


}

void SceneTitle::Update(GameScene* gameScene) { 
 	objectManager_->Update(); 
	
	if (input_->PushKey(DIK_SPACE)) {
		gameScene->SetScene(Scene::kPlay);
	}

}

void SceneTitle::DrawBackdrop() {

}
void SceneTitle::Draw3D() {

}
void SceneTitle::DrawOverlay() { objectManager_->Draw(); }
