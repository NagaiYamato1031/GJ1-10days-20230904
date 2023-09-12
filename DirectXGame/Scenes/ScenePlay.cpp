﻿#include "ScenePlay.h"

#include "GameScene.h"

#include "Mymath.h"

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
  
  for (int x = 0; x < 36; x++) {
		for (int y = 0; y < 4; y++) {
			BlockEffect* blockEffect = new BlockEffect();
			blockEffect->Initialize();
			blockEffect->SetPosition({32.0f * float(x) + 80.0f, 32.0f * float(y) + 80.0f});
			blockEffects_.emplace_back(blockEffect);
		}
  }


  score_ = Score::GetInstance();
  score_->Initialize();
}


void ScenePlay::Update() {
  bool spawnFlage = false;
  
	player_->Update();
  
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
			score_->AddScore();
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

	CheckAllCollision();



	score_->Update();

}



void ScenePlay::DrawBackdrop() {
	backGround_->Draw();
  
	for (size_t i = 0; i < blockEffects_.size(); i++) {
		BlockEffect* blockEffect = blockEffects_[i].get();
		blockEffect->Draw();
	}

	for (size_t i = 0; i < blocks_.size(); i++) {
		Block* block = blocks_[i].get();
		block->Draw();
	}


	player_->Draw();

	score_->Draw();
}

void ScenePlay::Draw3D() {}

void ScenePlay::DrawOverlay() {}

void ScenePlay::CheckAllCollision() {
	Transform2D playerData = player_->GetTransform2D();

	if (720.0f - playerData.size_.x * playerData.scale_.x / 2.0f <= playerData.position_.y) {
		score_->SubtractScore();
	}

	// プレイヤーの先頭に合わせる配慮
	// 距離
	float offset = 64.0f;
	// 角度で取得
	Vector2 direction = {std::cosf(playerData.rotate_), std::sinf(playerData.rotate_)};
	direction.x *= offset;

	playerData.position_ += direction;

	for (auto& block : blocks_) {
		Vector2 blockData = block->GetPosition();

		float blockSize = 32.0f;

		float distance = Mymath::Length(playerData.position_ - blockData);
		if (distance <= playerData.size_.x * playerData.scale_.x / 2.5f + blockSize) {
			block->OnCollision();
		}
	}


}

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
