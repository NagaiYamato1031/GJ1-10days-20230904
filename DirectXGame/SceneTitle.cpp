#include "SceneTitle.h"

SceneTitle::~SceneTitle() {

}

void SceneTitle::Initialize() { objectManager_ = ObjectManager::GetInstance(); }

void SceneTitle::Update() { objectManager_->Update(); }

void SceneTitle::DrawBackdrop() {

}
void SceneTitle::Draw3D() {

}
void SceneTitle::DrawOverlay() { objectManager_->Draw(); }
