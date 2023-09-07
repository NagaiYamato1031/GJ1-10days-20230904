#include "SceneTitle.h"

#include "Objects/Player.h"
#include "Objects/Block.h"
#include "GameScene.h"


SceneTitle::~SceneTitle() {}

void SceneTitle::Initialize() {
	objectManager_ = ObjectManager::GetInstance();

	input_ = Input::GetInstance();
	
	Player* player_ = new Player();
	player_->Initialize();

	objectManager_->AddObject(kPlayer, player_);

	// プレイヤーのテクスチャ
	objectManager_->LoadTexture("Line", "Sausage/line.png");
	objectManager_->LoadTexture("PlayerTop", "Sausage/sausage.png", kPlayer);
	objectManager_->LoadTexture("1x1", "white1x1.png");
	objectManager_->LoadTexture("uv", "uvChecker.png", kPlayer);
  
	objectManager_->GetSprite("Line")->SetPosition({-10, -10});
	objectManager_->GetSprite("PlayerTop")->SetPosition({640, 360});
	objectManager_->GetSprite("1x1")->SetPosition({64, 64});

	

}

void SceneTitle::Update(GameScene* gameScene) { 
 	objectManager_->Update(); 
	
	if (input_->PushKey(DIK_SPACE)) {
		objectManager_->Clear();
		gameScene->SetScene(Scene::kPlay);
		
	}
}

void SceneTitle::Update() { objectManager_->Update(); }

void SceneTitle::DrawBackdrop() {}
void SceneTitle::Draw3D() {}
void SceneTitle::DrawOverlay() { objectManager_->Draw(); }
