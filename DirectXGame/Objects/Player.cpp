#include "Player.h"

#include "ObjectManager.h"
#include "Config/GlobalConfigs.h"
#include "ObjectManager.h"

Player::Player() {}

Player::~Player() {}

void Player::Initialize() {
	worldTransformBase_.Initialize();
	worldTransforms_.clear();

	objectManager_ = ObjectManager::GetInstance();

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Player";
	configs->CreateGroup(groupName);

	// 一度更新する
	worldTransformBase_.UpdateMatrix();

	for (WorldTransform& wt : worldTransforms_) {
		wt.UpdateMatrix();
	}
}

void Player::Update() {
	worldTransformBase_.UpdateMatrix();
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
}

void Player::Draw() { objectManager_->DrawSprite("uv"); }

void Player::OnCollision() {}

void Player::AddlyGlobalConfigs() {

	/*GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Player";*/
}
