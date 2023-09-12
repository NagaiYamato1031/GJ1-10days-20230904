#include "ScenePlay.h"

#include "GameScene.h"

#include "Mymath.h"

ScenePlay::ScenePlay() {}

ScenePlay::~ScenePlay() {}

void ScenePlay::Initialize(GameScene* gameScene) {

	gameScene_ = gameScene;
	input_ = Input::GetInstance();

	blocks_ = nullptr;

	timeFrame = 0;
	currentLoadData_ = 0;

	int32_t handle = TextureManager::Load("Sausage/wallPaper.png");
	backGround_.reset(Sprite::Create(handle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f}));

	// ブロックの左上座標
	Vector2 position = {320, 64};

	// 一度しかファイル読み込みしない
	static bool hasInit = false;

	if (!hasInit) {
		fileName_ = "stage";
		blockDatas_.clear();
		stageDatas_.clear();
		LoadStageFile(fileName_);
		CreateBlocks(position);
		hasInit = true;
	} else {
		blockDatas_.clear();
		CreateBlocks(position);
		//// フラグを元に戻す
		//for (auto& datas : blockDatas_) {
		//	for (auto& data : datas) {
		//		Vector2 temp = data->GetPosition();
		//		data->Initialize();
		//		data->SetPosition(temp);
		//	}
		//}
	}

	GlobalConfigs* configs_ = GlobalConfigs::GetInstance();
	const char* groupName = "ScenePlay";
	configs_->CreateGroup(groupName);

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
}

void ScenePlay::Update() {
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

	ImGui::End();

#endif // _DEBUG
}

void ScenePlay::DrawBackdrop() {
	blocks_ = &blockDatas_[currentLoadData_];
	backGround_->Draw();

	for (auto& block : *blocks_) {
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
	// GlobalConfigs* configs = GlobalConfigs::GetInstance();
	// const char* groupName = "ScenePlay";
}