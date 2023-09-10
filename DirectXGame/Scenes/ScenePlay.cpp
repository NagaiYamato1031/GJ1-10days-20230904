#include "ScenePlay.h"

#include "GameScene.h"


ScenePlay::ScenePlay() {}

ScenePlay::~ScenePlay() {}

void ScenePlay::Initialize(GameScene* gameScene) {

	gameScene_ = gameScene;
	input_ = Input::GetInstance();

	//BlockSqawn();

	for (int x = 0; x < 36; x++) {
		for (int y = 0; y < 4; y++) {
			BlockEffect* blockEffect = new BlockEffect();
			blockEffect->Initialize();
			blockEffect->SetPosition({32.0f * float(x) + 80.0f, 32.0f * float(y) + 80.0f});
			blockEffects_.emplace_back(blockEffect);
		}
	}
}

void ScenePlay::Update() {
	bool spawnFlage = false;
	for (size_t i = 0; i < blocks_.size(); i++) {
		Block* block = blocks_[i].get();
		block->Update();
		if (block->IsDead()) {
			for (size_t j = 0; j < blockEffects_.size(); j++) {
				BlockEffect* blockEffect = blockEffects_[j].get();
				if (blockEffect->GetPosition().x == block->GetPosition().x &&
				    blockEffect->GetPosition().y == block->GetPosition().y) {
					blockEffect->SetIsActive(false);
					blockEffect->ChangeState(BlockState::kBreak);
				}
			}
			blocks_.erase(blocks_.begin() + i);
		}
	}

	for (size_t i = 0; i < blockEffects_.size(); i++) {
		BlockEffect* blockEffect = blockEffects_[i].get();
		if (input_->PushKey(DIK_L)) {
			blockEffect->ChangeState(BlockState::kVanish);
			blocks_.clear();
		}
		if (input_->PushKey(DIK_O)) {
			blockEffect->ChangeState(BlockState::kSpawn);
		}
		blockEffect->Update();
		spawnFlage = blockEffect->GetFlage();
	}

	if (spawnFlage) {
		BlockSqawn();
		for (size_t i = 0; i < blocks_.size(); i++) {
			Block* block = blocks_[i].get();
			block->Update();
			if (block->IsDead()) {
				blocks_.erase(blocks_.begin() + i);
			}
		}
	}

	

	if (input_->PushKey(DIK_E)) {
		gameScene_->SetScene(Scene::kTitle);
	}
}

void ScenePlay::DrawBackdrop() { 
	for (size_t i = 0; i < blockEffects_.size(); i++) {
		BlockEffect* blockEffect = blockEffects_[i].get();
		blockEffect->Draw();
	}
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


