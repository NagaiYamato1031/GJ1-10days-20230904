#include "ScenePlay.h"

#include "GameScene.h"

#include "Mymath.h"

ScenePlay::ScenePlay() {}

ScenePlay::~ScenePlay() {}

void ScenePlay::Initialize(GameScene* gameScene) {

	gameScene_ = gameScene;
	input_ = Input::GetInstance();

	blocks_.clear();
	stageDatas_.clear();

	timeFrame = 0;
	nextLoadData_ = 0;

	int32_t handle = TextureManager::Load("Sausage/wallPaper.png");
	backGround_.reset(Sprite::Create(handle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f}));

	BlockSqawn();
	

	GlobalConfigs* configs_ = GlobalConfigs::GetInstance();
	const char* groupName = "ScenePlay";
	configs_->CreateGroup(groupName);


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

  score_ = Score::GetInstance();
  score_->Initialize();

  ScenePlayBehavior_ = ScenePlayState::kPlay;
}

void ScenePlay::Update() {
  if (ScenePlayBehaviorRequest_) {
	  // 振る舞いを変更する
	  ScenePlayBehavior_ = ScenePlayBehaviorRequest_.value();
	  // 各振る舞いごとの初期化を実行
	  switch (ScenePlayBehavior_) {
	  case ScenePlayState::kPlay:
		  break;
	  case ScenePlayState::kResult:
		  result_.reset(new Result);
		  result_->Initialize();
		  break;
	  }
	  // 振る舞いリクエストをリセット
	  ScenePlayBehaviorRequest_ = std::nullopt;
  }

  switch (ScenePlayBehavior_) {
  case ScenePlayState::kPlay:
	  player_->Update();

	  for (auto& block : blocks_) {
		  block->Update();
	  }
	  if (input_->PushKey(DIK_E)) {
		  gameScene_->SetScene(Scene::kTitle);
	  }

	  CheckAllCollision();

	  score_->Update();
	  if (input_->PushKey(DIK_B)) {
		  ScenePlayBehaviorRequest_ = ScenePlayState::kResult;
	  }
	  break;
  case ScenePlayState::kResult:
	  result_->Update();
	  break;
  }

}



void ScenePlay::DrawBackdrop() {
  switch (ScenePlayBehavior_) {
  case ScenePlayState::kPlay:
	  backGround_->Draw();

	  for (auto& block : blocks_) {
		  block->Draw();
	  }
	  player_->Draw();

	  score_->Draw();
	  break;
  case ScenePlayState::kResult:
	  backGround_->Draw();

	  for (auto& block : blocks_) {
		  block->Draw();
	  }
	  player_->Draw();

	  score_->Draw();
	  result_->Draw();
	  break;
  }
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
		if (!block->IsDead()) {
			Vector2 blockData = block->GetPosition();

			float blockSize = 32.0f;

			float distance = Mymath::Length(playerData.position_ - blockData);
			if (distance <= playerData.size_.x * playerData.scale_.x / 2.5f + blockSize) {
				block->OnCollision();
				score_->AddScore();
			}
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

void ScenePlay::AddlyConfigs() {
	//GlobalConfigs* configs = GlobalConfigs::GetInstance();
	//const char* groupName = "ScenePlay";
}