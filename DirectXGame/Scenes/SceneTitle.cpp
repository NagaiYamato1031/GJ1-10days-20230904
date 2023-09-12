#include "SceneTitle.h"

#include "GameScene.h"

SceneTitle::~SceneTitle() {}

void SceneTitle::Initialize(GameScene* gameScene) {
	gameScene_ = gameScene;
	input_ = Input::GetInstance();

	timeFrame = 0;
	nextLoadData_ = 0;

	GlobalConfigs* configs_ = GlobalConfigs::GetInstance();
	const char* groupName = "SceneTitle";
	configs_->CreateGroup(groupName);

	int32_t handle = TextureManager::Load("Sausage/wallPaper.png");
	backGround_.reset(Sprite::Create(handle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	handle = TextureManager::Load("Sausage/title.png");
	titleLogo_.reset(Sprite::Create(handle, {640, 500}, {1, 1, 1, 1}, {0.5f, 0.5f}));

	// プレイヤーのテクスチャ
	// enum を使用している時、enum の順番通りに読み込ませる
	// objectManager_->LoadTexture("Line", "Sausage/line.png",kPlayer);
	// objectManager_->LoadTexture("PlayerTop", "Sausage/sausage.png", kPlayer);
	// objectManager_->LoadTexture("PlayerBody", "Sausage/sausage.png", kPlayer);
	// objectManager_->LoadTexture("canon", "Sausage/canon.png", kPlayer);
	// objectManager_->LoadTexture("1x1", "white1x1.png");
}

void SceneTitle::Update() {
	if (input_->PushKey(DIK_SPACE)) {
		gameScene_->SetScene(Scene::kSelect);
	}
}

void SceneTitle::DrawBackdrop() {
	backGround_->Draw();
	titleLogo_->Draw();
}
void SceneTitle::Draw3D() {}
void SceneTitle::DrawOverlay() {}

void SceneTitle::CheckAllCollision() {}

void SceneTitle::AddlyConfigs() {}