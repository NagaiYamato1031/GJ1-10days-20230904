#include "SceneResult.h"

#include "Config/GlobalConfigs.h"
#include "GameScene.h"

SceneResult::SceneResult() {}

SceneResult::~SceneResult() {}

void SceneResult::Initialize(GameScene* gameScene) {
	objectManager_ = ObjectManager::GetInstance();
	gameScene_ = gameScene;
	input_ = Input::GetInstance();
}

void SceneResult::Update() {  }

void SceneResult::DrawBackdrop() {}

void SceneResult::Draw3D() {}

void SceneResult::DrawOverlay() {}
