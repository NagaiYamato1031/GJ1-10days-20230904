#include "Block.h"

#include "Config/GlobalConfigs.h"

Block::Block() {}

Block::~Block() {}

void Block::Initialize() {
	worldTransformBase_.Initialize();

	input_ = Input::GetInstance();

	sprites_.clear();

	SetSprite(kNormal, 1, "block.png");

	sprites_[kNormal][0]->isUse_ = true;

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Block";
	configs->CreateGroup(groupName);

	// 一度更新する
	worldTransformBase_.UpdateMatrix();

	//for (auto& wt : worldTransforms_) {
	//	wt->UpdateMatrix();
	//}
}

void Block::Update() {
	worldTransformBase_.UpdateMatrix(); /*
	 for (auto& worldTransform : worldTransforms_) {
	     worldTransform->UpdateMatrix();
	 }*/
	SpriteUpdate();

	//Collider();

}

void Block::Draw() { 
	//objectManager_->SetSpritePosition("block", position_);
	//objectManager_->DrawSprite("block"); 
	DrawSprite();
}

void Block::OnCollision() {
	isDead_ = true;
	worldTransformBase_.translation_ = {640, 400, 0};
}

void Block::AddlyGlobalConfigs() {

	/*GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Block";*/
}

void Block::SetPosition(const Vector2 pos) { 
	worldTransformBase_.translation_.x = pos.x;
	worldTransformBase_.translation_.y = pos.y;
	sprites_[kNormal][0]->transform_.position_ = {worldTransformBase_.translation_.x, worldTransformBase_.translation_.y};
}

void Block::Collider() {
	Vector2 playerPos = {float(input_->GetMousePosition().x), float(input_->GetMousePosition().y)};
	//playerPos = player_->GetPosition();
	//float playerSize = player_->GetSize();

	float a = playerPos.x - worldTransformBase_.translation_.x;
	float b = playerPos.y - worldTransformBase_.translation_.y;
	float c = sqrtf(a * a + b * b);
	if (c <= 32.0f) {
		isDead_ = true;
	}
}
