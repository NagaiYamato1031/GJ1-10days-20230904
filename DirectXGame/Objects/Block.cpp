#include "Block.h"

#include "Config/GlobalConfigs.h"

Block::Block() {}

Block::~Block() {}

void Block::Initialize() {
	worldTransformBase_.Initialize();
	worldTransforms_.clear();


	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Block";
	configs->CreateGroup(groupName);

	// 一度更新する
	worldTransformBase_.UpdateMatrix();

	for (auto& wt : worldTransforms_) {
		wt->UpdateMatrix();
	}

	position_ = {0.0f, 0.0f};
}

void Block::Update() {
	worldTransformBase_.UpdateMatrix();
	for (auto& worldTransform : worldTransforms_) {
		worldTransform->UpdateMatrix();
	}
}

void Block::Draw() { 
	//objectManager_->SetSpritePosition("block", position_);
	//objectManager_->DrawSprite("block"); 
}

void Block::OnCollision() {}

void Block::AddlyGlobalConfigs() {

	/*GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Block";*/
}
