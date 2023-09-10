#include "ScenePlay.h"

#include "GameScene.h"

ScenePlay::ScenePlay() {}

ScenePlay::~ScenePlay() {}

void ScenePlay::Initialize(GameScene* gameScene) {

	gameScene_ = gameScene;
	input_ = Input::GetInstance();

	int32_t handle = TextureManager::Load("Sausage/wallPaper.png");
	backGround_.reset(Sprite::Create(handle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f}));

	BlockSqawn();

	player_.reset(new Player);
	player_->Initialize();

	player_->SetStagePosition({0, 0});
	player_->SetStageSize({1280, 720});
	/*
	player_->SetStagePosition({1280 / 4.0f, 0});
	player_->SetStageSize({1280 / 2.0f, 720});
	*/

	/*std::vector<std::unique_ptr<Block>> blocks;
	blocks.clear();*/
}

void ScenePlay::Update() {
	player_->Update();
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
	backGround_->Draw();

	for (size_t i = 0; i < blocks_.size(); i++) {
		Block* block = blocks_[i].get();
		block->Draw();
	}

	player_->Draw();
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
