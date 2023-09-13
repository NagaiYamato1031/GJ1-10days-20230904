﻿#include "ScenePlay.h"

#include "GameScene.h"

#include "Mymath.h"

ScenePlay::ScenePlay() {}

ScenePlay::~ScenePlay() {}

void ScenePlay::Initialize(GameScene* gameScene) {

	gameScene_ = gameScene;
	input_ = Input::GetInstance();

	blocks_ = nullptr;

	limitTimeFrame_ = 0;
	timeFrame = 0;
	currentLoadData_ = 0;

	int32_t handle = TextureManager::Load("Sausage/wallPaper.png");
	backGround_.reset(Sprite::Create(handle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f}));

	// ブロックの左上座標
	Vector2 position = {320, 64};

	// 一度しかファイル読み込みしない
	static bool hasInit = false;

	if (!hasInit) {
		blockDatas_.clear();
		stageDatas_.clear();
		if (fileName_.empty()) {
			fileName_ = "stage";
		}
		LoadStageFile(fileName_);
		CreateBlocks(position);

		phaseChangeAudioHandle_ = Audio::GetInstance()->LoadWave("sound/stateceChange.wav");

		hasInit = true;
	} else {
		blockDatas_.clear();
		CreateBlocks(position);
		//// フラグを元に戻す
		// for (auto& datas : blockDatas_) {
		//	for (auto& data : datas) {
		//		Vector2 temp = data->GetPosition();
		//		data->Initialize();
		//		data->SetPosition(temp);
		//	}
		// }
	}

	player_.reset(new Player);
	player_->Initialize();

	player_->SetStagePosition({0, 64});
	player_->SetStageSize({1280, 720 - 64});
	/*
	player_->SetStagePosition({1280 / 4.0f, 0});
	player_->SetStageSize({1280 / 2.0f, 720});
	*/

	/*std::vector<std::unique_ptr<Block>> blocks;
	blocks.clear();*/

	score_ = Score::GetInstance();
	score_->Initialize();

	ScenePlayBehavior_ = ScenePlayState::kPlay;

	GlobalConfigs* configs_ = GlobalConfigs::GetInstance();
	const char* groupName = "ScenePlay";
	configs_->CreateGroup(groupName);

	configs_->AddItem(groupName, "sausageStageMap1", 1800);
	configs_->AddItem(groupName, "sausageStageMap2", 1800);
	configs_->AddItem(groupName, "sausageStageMap3", 1800);

	AddlyConfigs();

	limitTimeFrame_ = kLimitTimeFrame_;
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
			player_->Initialize();

			player_->SetStagePosition({0, 0});
			player_->SetStageSize({1280, 720});
			player_->SetPosition({630.0f, 650.0f});
			player_->SetCanonPosition({630.0f, 650.0f});
			player_->SetCanonMoveLimitPosition({0.0f, 620.0f});
			player_->SetCanonMoveLimitSize({1280.0f, 130.0f});

			resultBlock_.reset(new Block);
			resultBlock_->Initialize();
			resultBlock_->SetPosition({640.0f, 530.0f});

			break;
		}
		// 振る舞いリクエストをリセット
		ScenePlayBehaviorRequest_ = std::nullopt;
	}

	switch (ScenePlayBehavior_) {
	case ScenePlayState::kPlay:
		limitTimeFrame_--;
		if (limitTimeFrame_ <= 0) {
			ScenePlayBehaviorRequest_ = ScenePlayState::kResult;
		} else {
			int frameDivision = kLimitTimeFrame_ / static_cast<int16_t>(stageDatas_.size());
			int indexBuffer =
			    static_cast<int16_t>(stageDatas_.size()) - limitTimeFrame_ / frameDivision - 1;
			if (indexBuffer != currentLoadData_) {
				currentLoadData_ = static_cast<uint16_t>(indexBuffer);
				Audio::GetInstance()->PlayWave(phaseChangeAudioHandle_, false, 0.2f);
			}
		}

		blocks_ = &blockDatas_[currentLoadData_];

		player_->Update();

		for (auto& block : *blocks_) {
			block->Update();
		}
		if (input_->PushKey(DIK_E)) {
			gameScene_->SetScene(Scene::kTitle);
		}

		CheckAllCollision();

		score_->Update();
		/*if (input_->PushKey(DIK_B)) {
			ScenePlayBehaviorRequest_ = ScenePlayState::kResult;
		}*/
		break;
	case ScenePlayState::kResult:
		player_->Update();
		result_->Update();
		if (resultBlock_->IsDead()) {

			resultBlock_->Update();
			auto& playerDatas = player_->GetTransform2Ds();
			Transform2D playerData = playerDatas[0];

			if (720.0f - playerData.size_.x * playerData.scale_.x / 2.0f <=
			    playerData.position_.y) {
				score_->SubtractScore();
			}

			// プレイヤーの先頭に合わせる配慮
			// 距離
			float offset = 64.0f;
			// 角度で取得
			Vector2 direction = {std::cosf(playerData.rotate_), std::sinf(playerData.rotate_)};
			direction.x *= offset;

			playerData.position_ += direction;
			if (ScenePlayBehavior_ == ScenePlayState::kResult) {
				float distance = Mymath::Length(playerData.position_ - resultBlock_->GetPosition());
				if (distance <= playerData.size_.x * playerData.scale_.x / 2.5f + 32.0f) {
					resultBlock_->OnCollision();
				}
			}
		}
		if (resultBlock_->GetCom()) {
			gameScene_->SetScene(Scene::kTitle);
		}
		break;
	}

#ifdef _DEBUG

	ImGui::Begin("ScenePlay");

	Vector2 pos = {320, 64};
	if (ImGui::Button("1")) {
		currentLoadData_ = 0;
		// CreateBlocks(pos);
	}
	ImGui::SameLine();
	if (ImGui::Button("2")) {
		currentLoadData_ = 1;
		// CreateBlocks(pos);
	}
	ImGui::SameLine();
	if (ImGui::Button("3")) {
		currentLoadData_ = 2;
		// CreateBlocks(pos);
	}
	ImGui::Text("%d", stageDatas_.size());

	if (ImGui::Button("ReloadCSVFile")) {
		LoadStageFile("stage");
		CreateBlocks({320, 64});
	}

	int frameDivision = kLimitTimeFrame_ / static_cast<int16_t>(stageDatas_.size());
	int indexBuffer = static_cast<int16_t>(stageDatas_.size()) - limitTimeFrame_ / frameDivision;

	ImGui::Text("%d", kLimitTimeFrame_);
	ImGui::Text("%d", limitTimeFrame_);
	ImGui::Text("%d", frameDivision);
	ImGui::Text("%d", indexBuffer);
	ImGui::Text("%d", currentLoadData_);

	ImGui::End();

#endif // _DEBUG
}

void ScenePlay::DrawBackdrop() {

	switch (ScenePlayBehavior_) {
	case ScenePlayState::kPlay:
		blocks_ = &blockDatas_[currentLoadData_];
		backGround_->Draw();

		for (auto& block : *blocks_) {
			block->Draw();
		}
		player_->Draw();

		score_->Draw();
		break;
	case ScenePlayState::kResult:
		backGround_->Draw();
		blocks_ = &blockDatas_[currentLoadData_];
		for (auto& block : *blocks_) {
			block->Draw();
		}
		result_->Draw();
		resultBlock_->Draw();
		player_->Draw();
		score_->Draw();
		break;
	}
}

void ScenePlay::Draw3D() {}

void ScenePlay::DrawOverlay() {}

void ScenePlay::CheckAllCollision() {
	auto& playerDatas = player_->GetTransform2Ds();
	Transform2D playerData = playerDatas[0];

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

	for (auto& block : *blocks_) {
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
	offset *= -1;
	for (size_t i = 1; i < playerDatas.size(); i++) {

		// 角度で取得
		direction = {std::cosf(playerDatas[i].rotate_), std::sinf(playerDatas[i].rotate_)};
		direction.x *= offset;

		playerData.position_ = playerDatas[i].position_ + direction;
		for (auto& block : *blocks_) {
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
	if (ScenePlayBehavior_ == ScenePlayState::kResult) {
		float distance = Mymath::Length(playerData.position_ - resultBlock_->GetPosition());
		if (distance <= playerData.size_.x * playerData.scale_.x / 2.5f + 32.0f) {
			resultBlock_->OnCollision();
		}
	}
}

void ScenePlay::BlockSqawn() {
	blocks_->clear();

	for (int x = 0; x < 36; x++) {
		for (int y = 0; y < 4; y++) {
			Block* block = new Block();
			block->Initialize();
			block->SetPosition({32.0f * float(x) + 80.0f, 32.0f * float(y) + 80.0f});
			blocks_->emplace_back(block);
		}
	}
}

void ScenePlay::AddlyConfigs() {
	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "ScenePlay";

	kLimitTimeFrame_ = configs->GetIntValue(groupName, fileName_);
}