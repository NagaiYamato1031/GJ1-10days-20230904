#include "ScenePlay.h"

#include "GameScene.h"

#include "Objects/Block.h"

ScenePlay::ScenePlay() {}

ScenePlay::~ScenePlay() {}

void ScenePlay::Initialize(GameScene* gameScene) { 
	objectManager_ = ObjectManager::GetInstance();

	gameScene_ = gameScene;
	input_ = Input::GetInstance();

	/*std::vector<std::unique_ptr<Block>> blocks;
	blocks.clear();*/


	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			Block* block_ = new Block();
			block_->Initialize();
			block_->SetPosition({16.0f * float(x), 16.0f * float(y)});

			objectManager_->AddObject(kBlock, block_);

			// プレイヤーのテクスチャ
			objectManager_->LoadTexture("block", "block.png", kBlock);
		}
	
	}

	


}

void ScenePlay::Update() { 
	
	if (input_->PushKey(DIK_E)) {
		objectManager_->Clear();
		gameScene_->SetScene(Scene::kTitle);
	}
}

void ScenePlay::DrawBackdrop() {}

void ScenePlay::Draw3D() {}

void ScenePlay::DrawOverlay() { objectManager_->Draw(); }
