#include "Player.h"

#include <cmath>

#include "Config/GlobalConfigs.h"
#include "ObjectManager.h"

#include "Mymath.h"

#ifdef _DEBUG

#include <imgui.h>

#endif // _DEBUG

Player::Player() {}

Player::~Player() {}

void Player::Initialize() {
	input_ = Input::GetInstance();
	objectManager_ = ObjectManager::GetInstance();

	worldTransformBase_.Initialize();
	worldTransforms_.clear();

	kPlayerSize_ = {1, 1, 1};
	// kGravity_ = 0.98f;
	isLockedCanon_ = false;

	worldTransformBase_.translation_ = {640, 500, 0};

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Player";
	configs->CreateGroup(groupName);

	configs->AddItem(groupName, "kPlayerSize", kPlayerSize_);
	// configs->AddItem(groupName, "kGravity", kGravity_);
	configs->AddItem(groupName, "kCanonPower", kCanonPower_);
	configs->AddItem(groupName, "kCanonRotateLimit", kCanonRotateLimit_);

	// 一度更新する
	worldTransformBase_.UpdateMatrix();

	for (WorldTransform& wt : worldTransforms_) {
		wt.UpdateMatrix();
	}
}

void Player::Update() {
	// ControlLineUpdate();

	ControlCanonMouse();

	worldTransformBase_.translation_ += Vector3(movementVelocity_.x, movementVelocity_.y, 0.0f);

	float rotate = std::atan2(movementVelocity_.y, movementVelocity_.x);
	objectManager_->GetSprite("PlayerTop")->SetRotation(rotate);
	objectManager_->GetSprite("PlayerTop")
	    ->SetPosition({worldTransformBase_.translation_.x, worldTransformBase_.translation_.y});

	worldTransformBase_.UpdateMatrix();
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}

#ifdef _DEBUG

	ImGui::Begin("Player");

	ImGui::DragFloat2("PlayerPosition", &worldTransformBase_.translation_.x, 0.1f);
	ImGui::DragFloat2("CanonPosition", &canonPosition_.x, 0.1f);
	ImGui::DragFloat("CanonRotate", &canonRotate_, 0.1f);
	ImGui::SliderInt("isLockedCanon", reinterpret_cast<int*>(&isLockedCanon_), 0, 1);
	ImGui::SliderInt("CanonType", reinterpret_cast<int*>(&canonType_), 0, kCountofCanonType - 1);

	ImGui::End();

#endif // _DEBUG
}

void Player::Draw() {
	// プレイヤーの描画
	objectManager_->DrawSprite("PlayerTop");

	DrawCanon();
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
			ChangeCanonType();
		}
	} else {
		canonPosition_ = mousePosition_;
		if (input_->IsTriggerMouse(0)) {
			isLockedCanon_ = true;
		}
		if (input_->IsTriggerMouse(1)) {
			ChangeCanonType();
		}
	}
}

void Player::ControlCanonKeyBoard() {

	// 大砲がロックされている時
	if (isLockedCanon_) {
		// 左クリックの挙動
		if (input_->TriggerKey(DIK_SPACE)) {
			Vector2 velocity = {std::cosf(canonRotate_), std::sinf(canonRotate_)};
			velocity *= kCanonPower_;
			movementVelocity_ = velocity;
			isLockedCanon_ = false;
		}
		// 右クリックの挙動
		if (input_->IsTriggerMouse(DIK_UP)) {
			ChangeCanonType();
		}
	} else {
		canonPosition_ = mousePosition_;
		if (input_->IsTriggerMouse(0)) {
			isLockedCanon_ = true;
		}
		if (input_->IsTriggerMouse(1)) {
			ChangeCanonType();
		}
	}
}

void Player::ChangeCanonType() {
	canonType_ = static_cast<CanonType>(canonType_ + 1);
	if (canonType_ == kCountofCanonType) {
		canonType_ = kCanonNormal;
	}
}

void Player::DrawCanon() {
	// 試しに描画
	Vector2 way = mousePosition_ - canonPosition_;
	Sprite* buff = objectManager_->GetSprite("1x1");
	buff->SetSize({Mymath::Length(way), 5.0f});
	buff->SetRotation(canonRotate_);
	way = canonPosition_ + way * 0.5f;
	buff->SetPosition(way);

	buff->Draw();
}