#include "SceneSelect.h"

#include "Config/GlobalConfigs.h"
#include "GameScene.h"

#include "Mymath.h"

SceneSelect::~SceneSelect() {}

void SceneSelect::Initialize(GameScene* gameScene) {
	gameScene_ = gameScene;
	input_ = Input::GetInstance();

	blocks_.clear();
	timeFrame = 0;
	stageDatas_.clear();
	nextLoadData_ = 0;

	player_.reset(new Player());
	player_->Initialize();

	player_->SetStagePosition({0, 0});
	player_->SetStageSize({1280, 720});

	// ステージ選択用のブロック
	Block* blockStageOne_;
	Block* blockStageTwo_;
	Block* blockStageThree_;
	blockStageOne_ = new Block();
	blockStageOne_->Initialize();

	blockStageTwo_ = new Block();
	blockStageTwo_->Initialize();

	blockStageThree_ = new Block();
	blockStageThree_->Initialize();

	blocks_.emplace_back(blockStageOne_);
	blocks_.emplace_back(blockStageTwo_);
	blocks_.emplace_back(blockStageThree_);

	GlobalConfigs* configs_ = GlobalConfigs::GetInstance();
	const char* groupName = "SceneSelect";
	configs_->CreateGroup(groupName);

	configs_->AddItem(groupName, "CanonLimitPosition", {500, 300, 0});
	configs_->AddItem(groupName, "CanonLimitSize", {200, 100.0});
	float windowQuarter = 1280 / 4.0f;
	configs_->AddItem(groupName, "StageOnePosition", {windowQuarter, 100, 0});
	configs_->AddItem(groupName, "StageTwoPosition", {windowQuarter * 2, 100, 0});
	configs_->AddItem(groupName, "StageThreePosition", {windowQuarter * 3, 100, 0});

	// 設定読み込み
	AddlyConfigs();

	int32_t handle = TextureManager::Load("Sausage/wallPaper.png");
	backGround_.reset(Sprite::Create(handle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f}));
}

void SceneSelect::Update() {

	if (blocks_[0]->IsDead()) {
		gameScene_->SetScene(Scene::kPlay);
	} else if (blocks_[1]->IsDead()) {
		gameScene_->SetScene(Scene::kPlay);
	} else if (blocks_[2]->IsDead()) {
		gameScene_->SetScene(Scene::kPlay);
	}

	for (auto& block : blocks_) {
		block->Update();
	}

	player_->Update();

#ifdef _DEBUG

	ImGui::Begin("SceneSelect");

	if (ImGui::Button("Addly")) {
		AddlyConfigs();
	}

	ImGui::End();

#endif // _DEBUG
}

void SceneSelect::DrawBackdrop() {
	backGround_->Draw();

	for (auto& block : blocks_) {
		block->Draw();
	}

	player_->Draw();
}
void SceneSelect::Draw3D() {}
void SceneSelect::DrawOverlay() {}

void SceneSelect::CheckAllCollision() {
	Transform2D playerData = player_->GetTransform2D();
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

void SceneSelect::AddlyConfigs() {
	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "SceneSelect";

	Vector3 bufferVec3 = configs->GetVector3Value(groupName, "CanonLimitPosition");
	Vector2 bufferVec2 = {bufferVec3.x, bufferVec3.y};
	player_->SetCanonMovePosition(bufferVec2);
	bufferVec3 = configs->GetVector3Value(groupName, "CanonLimitSize");
	bufferVec2 = {bufferVec3.x, bufferVec3.y};
	player_->SetCanonMoveSize(bufferVec2);

	// ブロックの位置
	bufferVec3 = configs->GetVector3Value(groupName, "StageOnePosition");
	bufferVec2 = {bufferVec2.x, bufferVec2.y};
	blocks_[0]->SetPosition(bufferVec2);

	bufferVec3 = configs->GetVector3Value(groupName, "StageTwoPosition");
	bufferVec2 = {bufferVec2.x, bufferVec2.y};
	blocks_[1]->SetPosition(bufferVec2);

	bufferVec3 = configs->GetVector3Value(groupName, "StageThreePosition");
	bufferVec2 = {bufferVec2.x, bufferVec2.y};
	blocks_[2]->SetPosition(bufferVec2);
}