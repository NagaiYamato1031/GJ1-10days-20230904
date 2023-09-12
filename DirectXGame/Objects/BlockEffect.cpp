#include "BlockEffect.h"

#include "Config/GlobalConfigs.h"

#define _USE_MATH_DEFINES
#include <math.h>

BlockEffect::BlockEffect() {}

BlockEffect::~BlockEffect() {}

void BlockEffect::Initialize() {
	worldTransformBase_.Initialize();

	sprites_.clear();

	SetSprite(kNormal, 1, "block.png");
	SetSprite(kPixel, 6, "blockpix.png");

	//sprites_[kNormal][0]->isUse_ = false;

	

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Block";
	configs->CreateGroup(groupName);

	// 一度更新する
	worldTransformBase_.UpdateMatrix();

	//for (auto& wt : worldTransforms_) {
	//	wt->UpdateMatrix();
	//}
}

void BlockEffect::Update() {
	if (blockEffectBehaviorRequest_) {
		// 振る舞いを変更する
		blockEffectBehavior_ = blockEffectBehaviorRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (blockEffectBehavior_) {
		case BlockState::kNone:
			break;
		case BlockState::kSpawn:
			SpawnEffectInitialize();
			break;
		case BlockState::kBreak:
			BreakEffectInitialize();
			break;
		case BlockState::kVanish:
			VanishEffectInitialize();
			break;
		}
		// 振る舞いリクエストをリセット
		blockEffectBehaviorRequest_ = std::nullopt;
	}

	switch (blockEffectBehavior_) {
	case BlockState::kNone:
		break;
	case BlockState::kSpawn:
		SpawnEffect();
		break;
	case BlockState::kBreak:
		BreakEffect();
		break;
	case BlockState::kVanish:
		VanishEffect();
		break;
	}

	worldTransformBase_.UpdateMatrix(); /*
	 for (auto& worldTransform : worldTransforms_) {
	     worldTransform->UpdateMatrix();
	 }*/
	SpriteUpdate();

}

void BlockEffect::Draw() { 
	//objectManager_->SetSpritePosition("block", position_);
	//objectManager_->DrawSprite("block"); 
	DrawSprite();
}

void BlockEffect::OnCollision() {}

void BlockEffect::AddlyGlobalConfigs() {

	/*GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Block";*/
}

void BlockEffect::SetPosition(const Vector2 pos) { 
	worldTransformBase_.translation_.x = pos.x;
	worldTransformBase_.translation_.y = pos.y;
}

void BlockEffect::SpawnEffectInitialize() { 
	sprites_[kNormal][0]->transform_.rotate_ = 0.0f; 
	sprites_[kNormal][0]->transform_.size_ = {0.0f, 0.0f};
	degree_ = 0.0f;
	effectSize_ = 0.0f;
	sprites_[kNormal][0]->isUse_ = true;
	isActive_ = true;
}

void BlockEffect::SpawnEffect() { 
	if (effectSize_ < 32.0f) {
		degree_ += rotateSpeed_;
		float radian = degree_ * float(M_PI / 180.0f);
		sprites_[kNormal][0]->transform_.rotate_ = radian;
		effectSize_ += 0.2f;
		sprites_[kNormal][0]->transform_.size_ = {effectSize_, effectSize_};
	} 
	else {
		sprites_[kNormal][0]->transform_.rotate_ = 0.0f;
		blockEffectBehaviorRequest_ = BlockState::kNone;
	}
}

void BlockEffect::BreakEffectInitialize() { 
	const float speed = 1.0f;
	degree_ = 0.0f;
	for (int i = 0; i < 6; i++) {
		pixel_[i].position = {worldTransformBase_.translation_.x, worldTransformBase_.translation_.y};
		pixel_[i].velocity.x =
		    speed * cosf(degree_ * float(M_PI / 180.0f)) - speed * sinf(degree_ * float(M_PI / 180.0f));
		pixel_[i].velocity.y =
		    speed * sinf(degree_ * float(M_PI / 180.0f)) + speed * cosf(degree_ * float(M_PI / 180.0f));
		degree_ += 360.0f / 6.0f;
		sprites_[kPixel][i]->isUse_ = true;
	}
	effectSize_ = 4.0f;
	sprites_[kNormal][0]->isUse_ = false;

}

void BlockEffect::BreakEffect() {
	if (effectSize_ > 0.0f) {
		
		for (int i = 0; i < 6; i++) {
			pixel_[i].position.x += pixel_[i].velocity.x;
			pixel_[i].position.y += pixel_[i].velocity.y;
			sprites_[kPixel][i]->transform_.position_ = pixel_[i].position;
			sprites_[kPixel][i]->transform_.size_ = {effectSize_, effectSize_};
		}
		effectSize_ -= 0.2f;
	} else {
		blockEffectBehaviorRequest_ = BlockState::kNone;
		for (int i = 0; i < 6; i++) {
			sprites_[kPixel][i]->isUse_ = false;
		}
	}
}

void BlockEffect::VanishEffectInitialize() {
	sprites_[kNormal][0]->transform_.rotate_ = 0.0f;
	sprites_[kNormal][0]->transform_.size_ = {0.0f, 0.0f};
	degree_ = 0.0f;
	effectSize_ = 32.0f;
	for (int i = 0; i < 6; i++) {
		sprites_[kPixel][i]->isUse_ = false;
	}
}

void BlockEffect::VanishEffect() {
	if (effectSize_ > 0.0f) {
		degree_ += rotateSpeed_;
		float radian = degree_ * float(M_PI / 180.0f);
		sprites_[kNormal][0]->transform_.rotate_ = radian;
		effectSize_ -= 0.2f;
		sprites_[kNormal][0]->transform_.size_ = {effectSize_, effectSize_};
	} 
	else {
		sprites_[kNormal][0]->transform_.rotate_ = 0.0f;
		sprites_[kNormal][0]->transform_.size_ = {0.0f, 0.0f};
		blockEffectBehaviorRequest_ = BlockState::kNone;
		sprites_[kNormal][0]->isUse_ = false;
		
	}
}

void BlockEffect::ChangeState(BlockState behavior) { 
	blockEffectBehaviorRequest_ = behavior; 
}

void BlockEffect::SetSpritePosition(const Vector2& position) { 
	sprites_[kNormal][0]->transform_.position_ = position;
	for (int i = 0; i < 6; i++) {
		sprites_[kPixel][i]->transform_.position_ = position;
	}
	SetPosition(position);
}
