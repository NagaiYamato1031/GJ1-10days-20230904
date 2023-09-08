#include "Player.h"

#include <cmath>

#include "Config/GlobalConfigs.h"

#include "Mymath.h"

#ifdef _DEBUG

#include <imgui.h>

#endif // _DEBUG

Player::Player() {}

Player::~Player() {}

void Player::Initialize() {
	input_ = Input::GetInstance();

	worldTransformBase_.Initialize();
	sprites_.clear();

	kPlayerSize_ = {1, 1, 1};
	// kGravity_ = 0.98f;
	isLockedCanon_ = false;
	// kUseSpriteMax_ = kCountofPlayerTexture;

	worldTransformBase_.translation_ = {640, 500, 0};

	//// プレイヤーのテクスチャのデータは、画像読み込みの段階で作ってある
	//// 使う画像の最大数を決める
	// kUseSpriteData_[kPlayerLine].max_ = 1;
	// kUseSpriteData_[kPlayerTop].max_ = 1;
	// kUseSpriteData_[kPlayerBody].max_ = 1;
	// kUseSpriteData_[kPlayerCanon].max_ = 1;

	SetSprite(kPlayerLine, "Sausage/line.png");
	SetSprite(kPlayerTop, "Sausage/sausage.png");
	SetSprite(kPlayerBody, "Sausage/sausage.png");
	SetSprite(kPlayerCanon, "Sausage/canon.png");

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Player";
	configs->CreateGroup(groupName);

	configs->AddItem(groupName, "kPlayerSize", kPlayerSize_);
	// configs->AddItem(groupName, "kGravity", kGravity_);
	configs->AddItem(groupName, "kCanonPower", kCanonPower_);
	configs->AddItem(groupName, "kCanonRotateLimit", kCanonRotateLimit_);
	// configs->AddItem(groupName, "kUseSpriteMax", kUseSpriteMax_);

	//// 使うテクスチャの数だけ WorldTransform を作る
	// for (size_t i = 0; i < kCountofPlayerTexture; i++) {
	//	WorldTransform* wr = new WorldTransform();
	//	wr->Initialize();
	//	worldTransforms_.emplace_back(wr);
	// }

	// 一度更新する
	worldTransformBase_.UpdateMatrix();

	FetchSpriteData();
}

void Player::Update() {
	// ControlLineUpdate();

	// ControlCanonMouse();
	ControlCanonKeyBoard();

	worldTransformBase_.translation_ += Vector3(movementVelocity_.x, movementVelocity_.y, 0.0f);

	// float rotate = std::atan2(movementVelocity_.y, movementVelocity_.x);
	/*worldTransforms_[kPlayerTop]->rotation_.z = rotate;
	worldTransforms_[kPlayerTop]->translation_ = worldTransformBase_.translation_;
	*/// objectManager_->GetSprite("PlayerTop")->SetRotation(rotate);
	// objectManager_->GetSprite("PlayerTop")->SetPosition({worldTransformBase_.translation_.x,
	// worldTransformBase_.translation_.y});

	worldTransformBase_.UpdateMatrix();
	/*for (auto& worldTransform : worldTransforms_) {
	    worldTransform->UpdateMatrix();
	}*/

#ifdef _DEBUG

	ImGui::Begin("Player");

	ImGui::DragFloat2("PlayerPosition", &worldTransformBase_.translation_.x, 0.1f);
	Vector2 pos = sprites_[kPlayerTop].begin()->get()->sprite_->GetSize();
	ImGui::DragFloat2("SpriteSize", &pos.x, 0.1f);
	ImGui::DragFloat2("CanonPosition", &canonPosition_.x, 0.1f);
	ImGui::DragFloat("CanonRotate", &canonRotate_, 0.1f);
	ImGui::SliderInt("isLockedCanon", reinterpret_cast<int*>(&isLockedCanon_), 0, 1);
	ImGui::SliderInt("CanonType", reinterpret_cast<int*>(&canonType_), 0, kCountofCanonType - 1);

	ImGui::End();

#endif // _DEBUG

	SpriteUpdate();
}

void Player::Draw() {
	// プレイヤーの描画
	// objectManager_->DrawSprite("PlayerTop", worldTransforms_[kPlayerTop].get());

	sprites_[kPlayerTop][0]->sprite_->Draw();

	//DrawCanon();
	// なぞった線を描画する
	// DrawLine();
}

void Player::OnCollision() {}

void Player::AddlyGlobalConfigs() {
	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Player";

	kPlayerSize_ = configs->GetVector3Value(groupName, "kPlayerSize");
	// kGravity_ = configs->GetFloatValue(groupName, "kGravity");
	kCanonPower_ = configs->GetFloatValue(groupName, "kCanonPower");
	kCanonRotateLimit_ = configs->GetFloatValue(groupName, "kCanonRotateLimit");
	// kUseSpriteMax_ = configs->GetIntValue(groupName, "kUseSpriteMax");
}

void Player::ControlCanonMouse() {
	// マウス座標
	mousePosition_ = input_->GetMousePosition();
	// 大砲がロックされている時
	if (isLockedCanon_) {
		// 一度方向を取得する
		Vector2 way = mousePosition_ - canonPosition_;
		// 角度を求める
		float theta = std::atan2(way.y, way.x);
		// 角度制限
		if (0) {
			// 角度によって向く方向の点を修正する
			float length = Mymath::Length(way);
			// 上方向
			float upTheta = static_cast<float>(-std::numbers::pi) / 2.0f;
			if (theta < upTheta - kCanonRotateLimit_) {
				theta = upTheta - kCanonRotateLimit_;
				mousePosition_.x = mousePosition_.x - (std::cosf(theta) * length);
				mousePosition_.y = mousePosition_.y + (std::sinf(theta) * length);
			} else if (upTheta + kCanonRotateLimit_ < theta) {
				theta = upTheta + kCanonRotateLimit_;
				mousePosition_.x = mousePosition_.x - (std::cosf(theta) * length);
				mousePosition_.y = mousePosition_.y + (std::sinf(theta) * length);
			}
		}
		canonRotate_ = theta;

		// 左クリックの挙動
		if (input_->IsTriggerMouse(0)) {
			Vector2 velocity = {std::cosf(canonRotate_), std::sinf(canonRotate_)};
			velocity *= kCanonPower_;
			movementVelocity_ = velocity;
			isLockedCanon_ = false;
		}
		// 右クリックの挙動
		if (input_->IsTriggerMouse(1)) {
			ChangeCanonType(true);
		}
	} else {
		canonPosition_ = mousePosition_;
		if (input_->IsTriggerMouse(0)) {
			isLockedCanon_ = true;
		}
		if (input_->IsTriggerMouse(1)) {
			ChangeCanonType(true);
		}
	}
}

void Player::ControlCanonKeyBoard() {

	float kCanonMoveSpeed = 3.0f;
	// 大砲の移動
	if (input_->PushKey(DIK_W)) {
		canonPosition_.y -= kCanonMoveSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		canonPosition_.y += kCanonMoveSpeed;
	}
	if (input_->PushKey(DIK_A)) {
		canonPosition_.x -= kCanonMoveSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		canonPosition_.x += kCanonMoveSpeed;
	}

	// 上方向
	/// float upTheta = static_cast<float>(-std::numbers::pi) / 2.0f;
	// 大砲の回転方向調整
	/*if (input_->TriggerKey(DIK_LEFT)) {
	    if (canonRotate_ <= 0) {
	        canonDirection_ = 1;
	    } else {
	        canonDirection_ = -1;
	    }
	}
	if (input_->TriggerKey(DIK_RIGHT)) {
	    if(canonRotate_ <= 0) {
	        canonDirection_ = 1;
	    } else {
	        canonDirection_ = -1;
	    }
	}*/
	if (input_->PushKey(DIK_LEFT)) {
		canonRotate_ -= kCanonRotateSpeed_ * canonDirection_;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		canonRotate_ += kCanonRotateSpeed_ * canonDirection_;
	}

	// 一回転したときの挙動
	canonRotate_ = std::fmodf(canonRotate_, static_cast<float>(std::numbers::pi) * 2);

	// 射出の挙動
	if (input_->TriggerKey(DIK_Q)) {
		Vector2 velocity = {std::cosf(canonRotate_), std::sinf(canonRotate_)};
		velocity *= kCanonPower_;
		movementVelocity_ = velocity;
		isLockedCanon_ = false;
	}
	// タイプ変更の挙動
	if (input_->TriggerKey(DIK_UP)) {
		ChangeCanonType(true);
	} else if (input_->TriggerKey(DIK_DOWN)) {
		ChangeCanonType(false);
	}
}

void Player::ChangeCanonType(bool isUp) {
	if (isUp) {
		canonType_ = static_cast<CanonType>(canonType_ + 1);
		if (canonType_ == kCountofCanonType) {
			canonType_ = kCanonLow;
		}
	} else {
		canonType_ = static_cast<CanonType>(canonType_ - 1);
		if (canonType_ == -1) {
			canonType_ = kCanonHigh;
		}
	}
}

void Player::DrawCanon() {
	//// 試しに描画
	//Vector2 way = mousePosition_ - canonPosition_;
	/*worldTransforms_[kPlayerLine]->scale_.x = Mymath::Length(way);
	worldTransforms_[kPlayerLine]->scale_.y = 5.0f;
	worldTransforms_[kPlayerLine]->rotation_.z = canonRotate_;
	way = canonPosition_ + way * 0.5f;
	worldTransforms_[kPlayerLine]->translation_.x = way.x;
	worldTransforms_[kPlayerLine]->translation_.y = way.y;
	*///objectManager_->DrawSprite("Line", worldTransforms_[kPlayerLine].get());
	// Sprite* buff = objectManager_->GetSprite("1x1");
	// buff->SetSize({Mymath::Length(way), 5.0f});
	// buff->SetRotation(canonRotate_);
	// way = canonPosition_ + way * 0.5f;
	// buff->SetPosition(way);

	// buff->Draw();

	// worldTransforms_[kPlayerCanon]->translation_.x = canonPosition_.x;
	// worldTransforms_[kPlayerCanon]->translation_.y = canonPosition_.y;
	// worldTransforms_[kPlayerCanon]->rotation_.z = canonRotate_;
	// worldTransforms_[kPlayerCanon]->scale_.x = 100;
	// worldTransforms_[kPlayerCanon]->scale_.y = 100;
	////objectManager_->DrawSprite("Canon", worldTransforms_[kPlayerCanon].get());
	//// buff = objectManager_->GetSprite("canon");
	// buff->SetPosition(canonPosition_);
	// buff->SetRotation(canonRotate_);
	// buff->SetSize({100, 100});

	// buff->Draw();
}