#include "Score.h"

#include "Config/GlobalConfigs.h"

#define _USE_MATH_DEFINES
#include <math.h>

Score* Score::GetInstance() { 
	static Score instance;
	return &instance;
}

void Score::Initialize() {
	worldTransformBase_.Initialize();

	sprites_.clear();

	SetSprite(kScoreBar, 1, "blockpix.png");
	SetSprite(kScoreBase, 1, "Sausage/scorebase.png");

	sprites_[kScoreBar][0]->isUse_ = true;
	sprites_[kScoreBase][0]->isUse_ = true;

	score_ = 0.0f;
	sprites_[kScoreBar][0]->transform_.position_ = {10.0f, 24.0f};
	sprites_[kScoreBar][0]->transform_.size_ = {score_, 32.0f};
	sprites_[kScoreBase][0]->transform_.position_ = {640.0f, 24.0f};
	sprites_[kScoreBase][0]->transform_.size_ = {1280.0f, 48.0f};

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Score";
	configs->CreateGroup(groupName);

	// 一度更新する
	worldTransformBase_.UpdateMatrix();

	//for (auto& wt : worldTransforms_) {
	//	wt->UpdateMatrix();
	//}
}

void Score::Update() {
	
	sprites_[kScoreBar][0]->transform_.size_ = {score_, 32.0f};
	worldTransformBase_.UpdateMatrix(); /*
	 for (auto& worldTransform : worldTransforms_) {
	     worldTransform->UpdateMatrix();
	 }*/
	SpriteUpdate();

}

void Score::Draw() { 
	//objectManager_->SetSpritePosition("block", position_);
	//objectManager_->DrawSprite("block"); 
	DrawSprite();
}

void Score::OnCollision() {}

void Score::AddlyGlobalConfigs() {

	/*GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Block";*/
}

void Score::SetPosition(const Vector2 pos) { 
	worldTransformBase_.translation_.x = pos.x;
	worldTransformBase_.translation_.y = pos.y;
	sprites_[kNormal][0]->transform_.position_ = {worldTransformBase_.translation_.x, worldTransformBase_.translation_.y};
}

void Score::AddScore() { 
	if (score_ < 1270.0f) {
		score_ += scoreAdd_;
	}

}

void Score::SubtractScore() { 
	if (score_ > scoreDecrease_) {
		score_ -= scoreDecrease_;
	}

}


