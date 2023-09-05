#include "SceneTitle.h"
#include "Objects/Player.h"

SceneTitle::~SceneTitle() {

}

void SceneTitle::Initialize() { 
	objectManager_ = ObjectManager::GetInstance();
	
	Player* player_ = new Player();
	player_->Initialize();
	

	objectManager_->AddObject(kPlayer, player_);

	// プレイヤーのテクスチャ
	objectManager_->LoadTexture("uv", "uvChecker.png", kPlayer);




}

void SceneTitle::Update() { objectManager_->Update(); }

void SceneTitle::DrawBackdrop() {

}
void SceneTitle::Draw3D() {

}
void SceneTitle::DrawOverlay() { objectManager_->Draw(); }
