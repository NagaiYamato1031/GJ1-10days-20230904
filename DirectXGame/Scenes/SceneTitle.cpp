#include "SceneTitle.h"

#include "Objects/Player.h"
#include "Objects/Block.h"
#include "GameScene.h"


SceneTitle::~SceneTitle() {}

void SceneTitle::Initialize(GameScene* gameScene) {
	gameScene_ = gameScene;
	input_ = Input::GetInstance();
	
	Player* player_ = new Player();
	player_->Initialize();

	// プレイヤーのテクスチャ
	// enum を使用している時、enum の順番通りに読み込ませる
	//objectManager_->LoadTexture("Line", "Sausage/line.png",kPlayer);
	//objectManager_->LoadTexture("PlayerTop", "Sausage/sausage.png", kPlayer);
	//objectManager_->LoadTexture("PlayerBody", "Sausage/sausage.png", kPlayer);
	//objectManager_->LoadTexture("canon", "Sausage/canon.png", kPlayer);
	//objectManager_->LoadTexture("1x1", "white1x1.png");
}

void SceneTitle::Update() {  
	
	if (input_->PushKey(DIK_SPACE)) {
		gameScene_->SetScene(Scene::kPlay);
		
	}
}

void SceneTitle::DrawBackdrop() {}
void SceneTitle::Draw3D() {}
void SceneTitle::DrawOverlay() {}

void SceneTitle::CheckAllCollision() {}