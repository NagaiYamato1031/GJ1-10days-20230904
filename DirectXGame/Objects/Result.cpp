#include "Result.h"

#include "Config/GlobalConfigs.h"

#define _USE_MATH_DEFINES
#include <math.h>

void Result::Initialize() {
	worldTransformBase_.Initialize();

	sprites_.clear();

	SetSprite(kResultBack, 1, "Sausage/resultBack.png");
	SetSprite(kResultBase, 1, "Sausage/scoreboard.png");
	SetSprite(kResultStar, 3, "Sausage/star.png");
	SetSprite(kResultNumber, 6, "Sausage/number.png");

	sprites_[kResultBack][0]->isUse_ = true;
	sprites_[kResultBack][0]->transform_.position_ = {640.0f, 360.0f};
	sprites_[kResultBack][0]->transform_.size_ = {1280.0f, 720.0f};

	sprites_[kResultBase][0]->isUse_ = true;
	sprites_[kResultBase][0]->transform_.position_ = {640.0f, 360.0f};
	sprites_[kResultBase][0]->transform_.size_ = {400.0f, 550.0f};

	for (int i = 0; i < 6; i++) {
		sprites_[kResultNumber][i]->isUse_ = true;
		sprites_[kResultNumber][i]->sprite_->SetTextureRect({0.0f, 0.0f}, {128.0f, 128.0f});
		sprites_[kResultNumber][i]->transform_.scale_ = {0.1f, 1.0f};
	}
	/*sprites_[4][0]->transform_.position_ = {300.0f, 360.0f};
	sprites_[4][0]->transform_.scale_ = {1.0f, 0.1f};
	sprites_[4][0]->sprite_->SetTextureRect({0.0f, 64.0f}, {64.0f, 64.0f});*/
	
	for (int i = 0; i < 3; i++) {
		sprites_[kResultStar][i]->isUse_ = true;
	}

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Result";
	configs->CreateGroup(groupName);
	configs->AddItem(groupName, "basePositionX", base_.pos.x);
	configs->AddItem(groupName, "basePositionY", base_.pos.y);
	configs->AddItem(groupName, "starPositionX_1", star_[0].pos.x);
	configs->AddItem(groupName, "starPositionY_1", star_[0].pos.y);
	configs->AddItem(groupName, "starPositionX_2", star_[1].pos.x);
	configs->AddItem(groupName, "starPositionY_2", star_[1].pos.y);
	configs->AddItem(groupName, "starPositionX_3", star_[2].pos.x);
	configs->AddItem(groupName, "starPositionY_3", star_[2].pos.y);
	configs->AddItem(groupName, "number1X", number_[0].pos.x);
	configs->AddItem(groupName, "number1Y", number_[0].pos.y);
	configs->AddItem(groupName, "number2X", number_[1].pos.x);
	configs->AddItem(groupName, "number2Y", number_[1].pos.y);
	configs->AddItem(groupName, "number3X", number_[2].pos.x);
	configs->AddItem(groupName, "number3Y", number_[2].pos.y);
	configs->AddItem(groupName, "number4X", number_[3].pos.x);
	configs->AddItem(groupName, "number4Y", number_[3].pos.y);
	configs->AddItem(groupName, "number5X", number_[4].pos.x);
	configs->AddItem(groupName, "number5Y", number_[4].pos.y);
	configs->AddItem(groupName, "number6X", number_[5].pos.x);
	configs->AddItem(groupName, "number6Y", number_[5].pos.y);
	configs->AddItem(groupName, "number1ScaleX", number_[0].scale.x);
	configs->AddItem(groupName, "number1ScaleY", number_[0].scale.y);
	configs->AddItem(groupName, "number2ScaleX", number_[1].scale.x);
	configs->AddItem(groupName, "number2ScaleY", number_[1].scale.y);
	configs->AddItem(groupName, "number3ScaleX", number_[2].scale.x);
	configs->AddItem(groupName, "number3ScaleY", number_[2].scale.y);
	configs->AddItem(groupName, "number4ScaleX", number_[3].scale.x);
	configs->AddItem(groupName, "number4ScaleY", number_[3].scale.y);
	configs->AddItem(groupName, "number5ScaleX", number_[4].scale.x);
	configs->AddItem(groupName, "number5ScaleY", number_[4].scale.y);
	configs->AddItem(groupName, "number6ScaleX", number_[5].scale.x);
	configs->AddItem(groupName, "number6ScaleY", number_[5].scale.y);

	score_ = Score::GetInstance();

	// 一度更新する
	worldTransformBase_.UpdateMatrix();

	//for (auto& wt : worldTransforms_) {
	//	wt->UpdateMatrix();
	//}
}

void Result::Update() {
	sprites_[kResultBase][0]->transform_.position_ = base_.pos;
	for (int i = 0; i < 3; i++) {
		sprites_[kResultStar][i]->transform_.position_ = star_[i].pos;
	}
	for (int i = 0; i < 6; i++) {
		sprites_[kResultNumber][i]->transform_.position_ = number_[i].pos;
		sprites_[kResultNumber][i]->transform_.scale_ = number_[i].scale;
		float rank = 1;
		int num = 0;
		for (int t = 5 - i; t > 0; t--) {
			rank *= 10;
		}
		if (i == 0) {
			num = int(score_->GetScore() / rank);

		} else {
			num = int(score_->GetScore() / rank) % 10;
		}
		sprites_[kResultNumber][i]->sprite_->SetTextureRect({size_ * num, 0.0f}, {128.0f, 128.0f});
	}
#ifdef _DEBUG

	AddlyGlobalConfigs();

#endif // _DEBUG
	worldTransformBase_.UpdateMatrix(); /*
	 for (auto& worldTransform : worldTransforms_) {
	     worldTransform->UpdateMatrix();
	 }*/
	SpriteUpdate();

}

void Result::Draw() { 
	//objectManager_->SetSpritePosition("block", position_);
	//objectManager_->DrawSprite("block"); 
	DrawSprite();
}

void Result::OnCollision() {}

void Result::AddlyGlobalConfigs() {

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Result";

	base_.pos.x = configs->GetFloatValue(groupName, "basePositionX");
	base_.pos.y = configs->GetFloatValue(groupName, "basePositionY");
	star_[0].pos.x = configs->GetFloatValue(groupName, "starPositionX_1");
	star_[0].pos.y = configs->GetFloatValue(groupName, "starPositionY_1");
	star_[1].pos.x = configs->GetFloatValue(groupName, "starPositionX_2");
	star_[1].pos.y = configs->GetFloatValue(groupName, "starPositionY_2");
	star_[2].pos.x = configs->GetFloatValue(groupName, "starPositionX_3");
	star_[2].pos.y = configs->GetFloatValue(groupName, "starPositionY_3");
	number_[0].pos.x = configs->GetFloatValue(groupName, "number1X");
	number_[0].pos.y = configs->GetFloatValue(groupName, "number1Y");
	number_[1].pos.x = configs->GetFloatValue(groupName, "number2X");
	number_[1].pos.y = configs->GetFloatValue(groupName, "number2Y");
	number_[2].pos.x = configs->GetFloatValue(groupName, "number3X");
	number_[2].pos.y = configs->GetFloatValue(groupName, "number3Y");
	number_[3].pos.x = configs->GetFloatValue(groupName, "number4X");
	number_[3].pos.y = configs->GetFloatValue(groupName, "number4Y");
	number_[4].pos.x = configs->GetFloatValue(groupName, "number5X");
	number_[4].pos.y = configs->GetFloatValue(groupName, "number5Y");
	number_[5].pos.x = configs->GetFloatValue(groupName, "number6X");
	number_[5].pos.y = configs->GetFloatValue(groupName, "number6Y");
	number_[0].scale.x = configs->GetFloatValue(groupName, "number1ScaleX");
	number_[0].scale.y = configs->GetFloatValue(groupName, "number1ScaleY");
	number_[1].scale.x = configs->GetFloatValue(groupName, "number2ScaleX");
	number_[1].scale.y = configs->GetFloatValue(groupName, "number2ScaleY");
	number_[2].scale.x = configs->GetFloatValue(groupName, "number3ScaleX");
	number_[2].scale.y = configs->GetFloatValue(groupName, "number3ScaleY");
	number_[3].scale.x = configs->GetFloatValue(groupName, "number4ScaleX");
	number_[3].scale.y = configs->GetFloatValue(groupName, "number4ScaleY");
	number_[4].scale.x = configs->GetFloatValue(groupName, "number5ScaleX");
	number_[4].scale.y = configs->GetFloatValue(groupName, "number5ScaleY");
	number_[5].scale.x = configs->GetFloatValue(groupName, "number6ScaleX");
	number_[5].scale.y = configs->GetFloatValue(groupName, "number6ScaleY");

}

void Result::SetPosition(const Vector2 pos) { 
	worldTransformBase_.translation_.x = pos.x;
	worldTransformBase_.translation_.y = pos.y;
}

