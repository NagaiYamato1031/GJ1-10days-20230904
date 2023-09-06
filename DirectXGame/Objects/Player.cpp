#include "Player.h"

#include <cmath>

#include "Config/GlobalConfigs.h"
#include "ObjectManager.h"

#include "Mymath.h"

Player::Player() {}

Player::~Player() {}

void Player::Initialize() {
	input_ = Input::GetInstance();
	objectManager_ = ObjectManager::GetInstance();


	worldTransformBase_.Initialize();
	worldTransforms_.clear();

	kPlayerSize_ = {1, 1, 1};
	kGravity_ = 0.98f;

	/*interpPoints_.clear();


	kDivideLine_ = 10;
	kDirectLeft_ = 10.0f;

	drawTimeLine_ = 0;

	isPressed_ = false;*/

	worldTransformBase_.translation_ = {640, 20, 0};

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Player";
	configs->CreateGroup(groupName);

	configs->AddItem(groupName, "kPlayerSize", kPlayerSize_);
	configs->AddItem(groupName, "kGravity", kGravity_);
	/*configs->AddItem(groupName, "kLineSize", kLineSize_);
	configs->AddItem(groupName, "kDivideLine", kDivideLine_);
	configs->AddItem(groupName, "kDirectLeft", kDirectLeft_);
	configs->AddItem(groupName, "kDrawTimeLine", kDrawTimeLine_);*/

	// 一度更新する
	worldTransformBase_.UpdateMatrix();

	for (WorldTransform& wt : worldTransforms_) {
		wt.UpdateMatrix();
	}
}

void Player::Update() {
	//ControlLineUpdate();

	objectManager_->GetSprite("PlayerTop")
	    ->SetPosition({worldTransformBase_.translation_.x, worldTransformBase_.translation_.y});

	worldTransformBase_.UpdateMatrix();
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
}

void Player::Draw() {
	// プレイヤーの描画
	objectManager_->DrawSprite("PlayerTop");

	// なぞった線を描画する
	//DrawLine();
}

void Player::OnCollision() {}

void Player::AddlyGlobalConfigs() {
	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Player";

	kPlayerSize_ = configs->GetVector3Value(groupName, "kPlayerSize");
	kGravity_ = configs->GetFloatValue(groupName, "kGravity");
	//kLineSize_ = configs->GetIntValue(groupName, "kLineSize");
	//kDivideLine_ = configs->GetIntValue(groupName, "kDivideLine");
	//kDirectLeft_ = configs->GetFloatValue(groupName, "kDirectLeft");
	//kDrawTimeLine_ = configs->GetIntValue(groupName, "kDrawTimeLine");
}
//
//void Player::ControlLineUpdate() {
//
//	if (!isPressed_ && 0 < drawTimeLine_) {
//		drawTimeLine_--;
//		// 0 になった瞬間にクリアする
//		if (drawTimeLine_ == 0) {
//			interpPoints_.clear();
//		}
//	}
//
//	// 押したところから話したところで線を描く
//	if (input_->IsTriggerMouse(0)) {
//		interpPoints_.clear();
//		Vector2 mousePos = input_->GetMousePosition();
//		interpPoints_.push_back(SubDivision(mousePos, 0.0f));
//		isPressed_ = true;
//		drawTimeLine_ = 0;
//	}
//
//	if (isPressed_) {
//		Vector2 mousePos = input_->GetMousePosition();
//		clickPosition_ = mousePos;
//		if (!input_->IsPressMouse(0)) {
//			interpPoints_.push_back(SubDivision(mousePos, 0.0f));
//			isPressed_ = false;
//			drawTimeLine_ = kDrawTimeLine_;
//		}
//	}
//
//	// 線が区切られていない場合、区切る
//	if (dividing_ < interpPoints_.size() - 1) {
//		// 最後の一つは必ず 0
//		for (size_t i = dividing_ + 1; i < interpPoints_.size(); i++) {
//			float length = Mymath::Length(interpPoints_[i].point_ - interpPoints_[i - 1].point_);
//			interpPoints_[i - 1].length_ = length;
//		}
//	}
//
//	// 線を設定している時
//	if (isPressed_) {
//		// 線全体の長さ
//		float Alllength = 0;
//		for (SubDivision buff : interpPoints_) {
//			Alllength += buff.length_;
//		}
//
//		Vector2 endPoint = (interpPoints_.end() - 1)->point_;
//		// 今クリックしている場所への長さも加算する
//		float clickLength = 0.0f;
//		clickLength = Mymath::Length(clickPosition_ - endPoint);
//		Alllength += clickLength;
//
//		// 線の長さの分だけ色を変える
//		// 一色の段階を 64 として計算してみる
//		uint32_t kMaxOneColor = 64;
//		// uint32_t kMaxAllColor = kMaxOneColor * 3;
//
//		// 色の段階を求める
//		uint32_t colorLevel = static_cast<int>(Alllength) % 3 + 1;
//
//		// t を求める
//		float colorT = directLeft_ / kDirectLeft_;
//		// カラー
//		uint32_t colorInt = static_cast<int>(kMaxOneColor * colorT) << (colorLevel * 4 + 0xFF);
//
//		uint32_t A = 0xFF;
//		colorInt = colorInt >> 4;
//		uint32_t B = colorInt % 0x100;
//		colorInt = colorInt >> 4;
//		uint32_t G = colorInt % 0x100;
//		colorInt = colorInt >> 4;
//		uint32_t R = colorInt % 0x100;
//
//		Vector4 color = {
//		    static_cast<float>(R), static_cast<float>(G), static_cast<float>(B),
//		    static_cast<float>(A)};
//
//		// Sprite* const buffer = objectManager_->GetSprite(textureName_[kPlayerLine]);
//		// buffer->SetColor(color);
//
//		// Sprite* buffer = objectManager_->GetSprite(textureName_[kPlayerLine]);
//		Sprite* buffer = objectManager_->GetSprite("Line");
//		buffer->SetColor(color);
//
//		// 横長にする
//		Vector2 size = {clickLength / static_cast<float>(1), 1.0f};
//		buffer->SetSize(size);
//
//		// 次の点へのベクトルを取る
//		Vector2 way = clickPosition_ - endPoint;
//		// 今の点から半分進んだ場所に移動させる
//		Vector2 movement = endPoint + way * 0.5;
//		buffer->SetPosition(movement);
//
//		// どのくらい傾けるか
//		float rotate = std::atan2(way.y, way.x);
//		buffer->SetRotation(rotate);
//
//		// スプライトの描画
//		// buffer->Draw();
//	}
//}
//
// void Player::DrawLine() {
//	Sprite* buffer = objectManager_->GetSprite("Line");
//	if (isPressed_) {
//		// スプライトの描画
//		buffer->Draw();
//	}
//	if (0 < drawTimeLine_) {
//		{
//			// ここから線の長さ分描画
//			for (size_t i = 1; i < interpPoints_.size(); i++) {
//				SubDivision current = interpPoints_[i - 1];
//				SubDivision next = interpPoints_[i];
//
//				// 横長にする
//				Vector2 size = {current.length_ / static_cast<float>(kLineSize_ / 8), 1.0f};
//				buffer->SetSize(size);
//
//				// 次の点へのベクトルを取る
//				Vector2 way = next.point_ - current.point_;
//				// 今の点から半分進んだ場所に移動させる
//				Vector2 movement = current.point_ + way * 0.5;
//				buffer->SetPosition(movement);
//
//				// どのくらい傾けるか
//				float rotate = std::atan2(way.y, way.x);
//				buffer->SetRotation(rotate);
//
//				// スプライトの描画
//				buffer->Draw();
//			}
//		}
//	}
// }
