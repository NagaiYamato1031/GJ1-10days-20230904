#include "ScenePlay.h"

#include "GameScene.h"


ScenePlay::ScenePlay() {}

ScenePlay::~ScenePlay() {}

void ScenePlay::Initialize(GameScene* gameScene) {

	gameScene_ = gameScene;
	input_ = Input::GetInstance();

	BlockSqawn();

	/*std::vector<std::unique_ptr<Block>> blocks;
	blocks.clear();*/
}

void ScenePlay::Update() {
	for (size_t i = 0; i < blocks_.size(); i++) {
		Block* block = blocks_[i].get();
		block->Update();
		if (block->IsDead()) {
			blocks_.erase(blocks_.begin() + i);
		}
	}
	if (input_->PushKey(DIK_E)) {
		gameScene_->SetScene(Scene::kTitle);
	}
}

void ScenePlay::DrawBackdrop() { 
	for (size_t i = 0; i < blocks_.size(); i++) {
		Block* block = blocks_[i].get();
		block->Draw();
	}

}

void ScenePlay::Draw3D() {}

void ScenePlay::DrawOverlay() {}

void ScenePlay::CheckAllCollision() {}

void ScenePlay::BlockSqawn() { 
	blocks_.clear();

	for (int x = 0; x < 36; x++) {
		for (int y = 0; y < 4; y++) {
			Block* block = new Block();
			block->Initialize();
			block->SetPosition({32.0f * float(x) + 80.0f, 32.0f * float(y) + 80.0f});
			blocks_.emplace_back(block);
		}
	}
}


