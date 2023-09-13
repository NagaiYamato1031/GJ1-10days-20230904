#include "Player.h"

#include <cmath>

#include "Config/GlobalConfigs.h"

#include "MyConst.h"
#include "Mymath.h"

#ifdef _DEBUG

#include <imgui.h>

#endif // _DEBUG

Player::Player() {}

Player::~Player() {}

void Player::Initialize() {
	input_ = Input::GetInstance();

	audio_ = Audio::GetInstance();

	score_ = Score::GetInstance();

	worldTransformBase_.Initialize();
	sprites_.clear();

	SetSprite(kPlayerLine, 1, "Sausage/line.png");
	SetSprite(kPlayerTop, 1, "Sausage/sausage.png");
	SetSprite(kPlayerBody, 3, "Sausage/sausage2.png");
	SetSprite(kPlayerCanon, 1, "Sausage/canon.png");

	FetchSpriteData();

	sprites_[kPlayerLine][0]->isUse_ = true;
	sprites_[kPlayerTop][0]->isUse_ = true;
	// sprites_[kPlayerBody][0]->isUse_ = false;
	sprites_[kPlayerCanon][0]->isUse_ = true;

	kPlayerSize_ = 64;
	kCanonSize_ = 64;
	// kGravity_ = 0.98f;
	isLockedCanon_ = false;
	// kUseSpriteMax_ = kCountofPlayerTexture;

	worldTransformBase_.translation_ = {640, 500, 0};
	sprites_[kPlayerTop][0]->transform_.position_ = {640, 500};
	canonPosition_ = {600, 540};

	//// プレイヤーのテクスチャのデータは、画像読み込みの段階で作ってある
	//// 使う画像の最大数を決める
	// kUseSpriteData_[kPlayerLine].max_ = 1;
	// kUseSpriteData_[kPlayerTop].max_ = 1;
	// kUseSpriteData_[kPlayerBody].max_ = 1;
	// kUseSpriteData_[kPlayerCanon].max_ = 1;

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Player";
	configs->CreateGroup(groupName);

	configs->AddItem(groupName, "kPlayerSize", kPlayerSize_);
	configs->AddItem(groupName, "kCanonSize", kCanonSize_);
	configs->AddItem(groupName, "kCanonMoveSpeed", kCanonMoveSpeed_);
	configs->AddItem(groupName, "kCanonPower", kCanonPower_);
	configs->AddItem(groupName, "kCanonRotateLimit", kCanonRotateLimit_);
	configs->AddItem(groupName, "kNonCollisionFrame", kNonCollisionFrame_);
	configs->AddItem(groupName, "kOffsetOption", kOffsetOption_);

	AddlyAllGlobalConfigs();

	//// 使うテクスチャの数だけ WorldTransform を作る
	// for (size_t i = 0; i < kCountofPlayerTexture; i++) {
	//	WorldTransform* wr = new WorldTransform();
	//	wr->Initialize();
	//	worldTransforms_.emplace_back(wr);
	// }

	// 一度更新する
	worldTransformBase_.UpdateMatrix();

	shootSoundHandle_ = audio_->LoadWave("sound/shoot.wav");
	boundSoundHandle_ = audio_->LoadWave("sound/bound.wav");
	boundFloorSoundHandle_ = audio_->LoadWave("sound/boundFloor.wav");
}

void Player::Update() {
	// ControlLineUpdate();

	if (isMouse_) {
		ControlCanonMouse();
	}
	ControlCanonKeyBoard();

	// worldTransformBase_.translation_ += Vector3(movementVelocity_.x, movementVelocity_.y, 0.0f);
	// 範囲外に出ないようにする処理

	Vector2 position = sprites_[kPlayerTop][0]->transform_.position_ + movementVelocity_;
	float playerSIze = sprites_[kPlayerTop][0]->transform_.size_.x *
	                   sprites_[kPlayerTop][0]->transform_.scale_.x / 2.0f;
	if (position.x < stagePosition_.x + playerSIze) {
		position.x = stagePosition_.x + playerSIze;
		movementVelocity_.x *= -1;
		audio_->PlayWave(boundSoundHandle_, false, 0.4f);
	} else if (stagePosition_.x + stageSize_.x - playerSIze < position.x) {
		position.x = stagePosition_.x + stageSize_.x - playerSIze;
		movementVelocity_.x *= -1;
		audio_->PlayWave(boundSoundHandle_, false, 0.4f);
	}
	if (position.y < stagePosition_.y + playerSIze) {
		position.y = stagePosition_.y + playerSIze;
		movementVelocity_.y *= -1;
		audio_->PlayWave(boundSoundHandle_, false, 0.4f);
	} else if (stagePosition_.y + stageSize_.y - playerSIze < position.y) {
		position.y = stagePosition_.y + stageSize_.y - playerSIze;
		movementVelocity_.y *= -1;
		score_->SubtractScore();
		audio_->PlayWave(boundFloorSoundHandle_, false, 0.4f);
	}
	sprites_[kPlayerTop][0]->transform_.position_ = position;

	// 回転させる
	float rotate = std::atan2f(movementVelocity_.y, movementVelocity_.x);
	sprites_[kPlayerTop][0]->transform_.rotate_ = rotate;

	// 本体に合わせて動かす
	auto& playerDatas = sprites_[kPlayerBody];
	for (size_t i = 0; i < playerDatas.size(); i++) {
		if (!playerDatas[i]->isUse_) {
			continue;
		}
		float rotateBuffer = rotate - (1 - static_cast<float>(i)) * 0.3f;
		playerDatas[i]->transform_.rotate_ = rotateBuffer;
		// 角度で取得
		Vector2 direction = {std::cosf(rotateBuffer), std::sinf(rotateBuffer)};
		direction *= kOffsetOption_;
		playerDatas[i]->transform_.position_ = position + direction;
	}

	worldTransformBase_.UpdateMatrix();
	/*for (auto& worldTransform : worldTransforms_) {
	    worldTransform->UpdateMatrix();
	}*/
	ControlCanonCollision();

#ifdef _DEBUG

	ImGui::Begin("Player");
	Vector2 pos = sprites_[kPlayerTop].begin()->get()->sprite_->GetSize();
	ImGui::DragFloat2("SpriteSize", &pos.x, 0.1f);
	ImGui::DragFloat2("CanonPosition", &canonPosition_.x, 0.1f);
	ImGui::DragFloat("CanonRotate", &canonRotate_, 0.1f);
	ImGui::DragFloat("kOffsetOption", &kOffsetOption_, 0.1f);
	ImGui::SliderInt("isLockedCanon", reinterpret_cast<int*>(&isLockedCanon_), 0, 1);
	ImGui::SliderInt("CanonType", reinterpret_cast<int*>(&canonType_), 0, kCountofCanonType - 1);
	ImGui::SliderInt("isReloaded", reinterpret_cast<int*>(&isReloaded_), 0, 1);
	ImGui::SliderInt("isMouse", reinterpret_cast<int*>(&isMouse_), 0, 1);

	ImGui::Separator();
	ImGui::Text("size:%d", playerDatas.size());

	for (size_t i = 0; i < playerDatas.size(); i++) {
		if (!playerDatas[i]->isUse_)
			continue;
		std::string str = std::to_string(i) + ": rotate";
		ImGui::DragFloat(str.c_str(), &playerDatas[i]->transform_.rotate_, 0.02f);
		str = std::to_string(i) + "position";
		ImGui::DragFloat2(str.c_str(), &playerDatas[i]->transform_.position_.x, 0.02f);
	}

	if (ImGui::Button("AddlyConfig")) {
		AddlyAllGlobalConfigs();
	}

	ImGui::End();

#endif // _DEBUG

	SpriteUpdate();
}

void Player::Draw() {
	// プレイヤーの描画
	// objectManager_->DrawSprite("PlayerTop", worldTransforms_[kPlayerTop].get());

	// sprites_[kPlayerTop][0]->sprite_->Draw();

	DrawSprite();

	// DrawCanon();
	//  なぞった線を描画する
	//  DrawLine();
}

const Transform2D& Player::GetTransform2D() { return sprites_[kPlayerTop][0]->transform_; }
const std::vector<Transform2D> Player::GetTransform2Ds() {
	std::vector<Transform2D> data;
	data.push_back(sprites_[kPlayerTop][0]->transform_);
	for (auto& body : sprites_[kPlayerBody]) {
		if (body->isUse_) {
			data.push_back(body->transform_);
		}
	}
	return data;
}

void Player::SetPosition(const Vector2& position) { 
	sprites_[kPlayerTop][0]->transform_.position_ = position;
}

void Player::SetCanonPosition(const Vector2& position) { canonPosition_ = position; }

void Player::OnCollision() {}

void Player::AddlyGlobalConfigs() {
	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Player";

	kPlayerSize_ = configs->GetFloatValue(groupName, "kPlayerSize") * kAllScale_;
	kCanonSize_ = configs->GetFloatValue(groupName, "kCanonSize") * kAllScale_;
	kCanonMoveSpeed_ = configs->GetFloatValue(groupName, "kCanonMoveSpeed");
	kCanonPower_ = configs->GetFloatValue(groupName, "kCanonPower");
	kCanonRotateLimit_ = configs->GetFloatValue(groupName, "kCanonRotateLimit");
	kNonCollisionFrame_ = configs->GetIntValue(groupName, "kNonCollisionFrame");
	kOffsetOption_ = configs->GetFloatValue(groupName, "kOffsetOption");
}

void Player::ControlCanonMouse() {
	// マウス座標
	mousePosition_ = input_->GetMousePosition();
	// マウス座標をゲームの範囲内に収める
	float canonSize = sprites_[kPlayerCanon][0]->sprite_->GetSize().x / 2.0f;
	if (mousePosition_.x < canonMoveLimitPosition_.x + canonSize) {
		mousePosition_.x = canonMoveLimitPosition_.x + canonSize;
	} else if (canonMoveLimitPosition_.x + canonMoveLimitSize_.x - canonSize < mousePosition_.x) {
		mousePosition_.x = canonMoveLimitPosition_.x + canonMoveLimitSize_.x - canonSize;
	}
	if (mousePosition_.y < canonMoveLimitPosition_.y + canonSize) {
		mousePosition_.y = canonMoveLimitPosition_.y + canonSize;
	} else if (canonMoveLimitPosition_.y + canonMoveLimitSize_.y - canonSize < mousePosition_.y) {
		mousePosition_.y = canonMoveLimitPosition_.y + canonMoveLimitSize_.y - canonSize;
	}

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
		sprites_[kPlayerCanon][0]->transform_.rotate_ = theta;
		canonRotate_ = theta;

		// 左クリックの挙動
		if (input_->IsTriggerMouse(0)) {
			CanonShot();
		}
		// 右クリックの挙動
		if (input_->IsTriggerMouse(1)) {
			ChangeCanonType(true);
		}
	} else {
		sprites_[kPlayerCanon][0]->transform_.position_ = mousePosition_;
		canonPosition_ = mousePosition_;
		if (input_->IsTriggerMouse(0) && isReloaded_) {
			isLockedCanon_ = true;
		}
		if (input_->IsTriggerMouse(1)) {
			ChangeCanonType(true);
		}
	}
}

void Player::ControlCanonKeyBoard() {

	// float kCanonMoveSpeed = 3.0f;
	//  大砲の移動
	if (input_->PushKey(DIK_W)) {
		canonPosition_.y -= kCanonMoveSpeed_;
	}
	if (input_->PushKey(DIK_S)) {
		canonPosition_.y += kCanonMoveSpeed_;
	}
	if (input_->PushKey(DIK_A)) {
		canonPosition_.x -= kCanonMoveSpeed_;
	}
	if (input_->PushKey(DIK_D)) {
		canonPosition_.x += kCanonMoveSpeed_;
	}
	// 範囲制限
	float canonSize = sprites_[kPlayerCanon][0]->sprite_->GetSize().x / 2.0f;
	if (canonPosition_.x < canonMoveLimitPosition_.x + canonSize) {
		canonPosition_.x = canonMoveLimitPosition_.x + canonSize;
	} else if (canonMoveLimitPosition_.x + canonMoveLimitSize_.x - canonSize < canonPosition_.x) {
		canonPosition_.x = canonMoveLimitPosition_.x + canonMoveLimitSize_.x - canonSize;
	}
	if (canonPosition_.y < canonMoveLimitPosition_.y + canonSize) {
		canonPosition_.y = canonMoveLimitPosition_.y + canonSize;
	} else if (canonMoveLimitPosition_.y + canonMoveLimitSize_.y - canonSize < canonPosition_.y) {
		canonPosition_.y = canonMoveLimitPosition_.y + canonMoveLimitSize_.y - canonSize;
	}

	sprites_[kPlayerCanon][0]->transform_.position_ = canonPosition_;

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
	sprites_[kPlayerCanon][0]->transform_.rotate_ = canonRotate_;
	// 射出の挙動
	if (input_->TriggerKey(DIK_Q)) {
		audio_->PlayWave(shootSoundHandle_, false, 0.4f);
		CanonShot();
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

bool Player::CheckHitCollision() {
	float canonRadius = kCanonSize_ * sprites_[kPlayerCanon][0]->transform_.scale_.x;
	Vector2 position = sprites_[kPlayerTop][0]->transform_.position_;
	float playerSIze = kPlayerSize_ * sprites_[kPlayerTop][0]->transform_.scale_.x;

	float distance = Mymath::Length(position - canonPosition_);
	if (distance < canonRadius + playerSIze) {
		return true;
	}
	return false;
}

void Player::ControlCanonCollision() {
	// 当たらないフレームが経過するまで処理しない
	if (nonCollisionFrame_ != 0) {
		nonCollisionFrame_--;
		return;
	}
	if (isReloaded_) {
		auto& playerData = sprites_[kPlayerTop][0]->transform_;
		playerData.rotate_ = canonRotate_;
		playerData.position_ = canonPosition_;

	}
	// 当たった瞬間の処理
	else if (!isReloaded_ && CheckHitCollision()) {
		// 体を消す
		for (auto& body : sprites_[kPlayerBody]) {
			body->isUse_ = false;
		}
		isReloaded_ = true;
	}
}

void Player::CanonShot() {
	if (!isReloaded_) {
		return;
	}
	Vector2 velocity = {std::cosf(canonRotate_), std::sinf(canonRotate_)};
	velocity *= kCanonPower_;
	movementVelocity_ = velocity;
	isLockedCanon_ = false;
	// 当たらないフレームを設定
	nonCollisionFrame_ = kNonCollisionFrame_;
	isReloaded_ = false;
	auto& playerTopData = sprites_[kPlayerTop][0];
	playerTopData->transform_.rotate_ = canonRotate_;
	Vector2 direction{0, 0};
	// ソーセージを増やす
	auto& playerData = sprites_[kPlayerBody];
	switch (canonType_) {
	case kCanonHigh:
		playerData[0]->isUse_ = true;
		playerData[0]->transform_.rotate_ = playerTopData->transform_.rotate_ - 0.3f;
		// 角度で取得
		direction = {
		    std::cosf(playerData[0]->transform_.rotate_),
		    std::sinf(playerData[0]->transform_.rotate_)};
		direction *= kOffsetOption_;
		playerData[0]->transform_.position_ = playerTopData->transform_.position_ + direction;
	case kCanonNormal:
		playerData[1]->isUse_ = true;
		playerData[1]->transform_.rotate_ = playerTopData->transform_.rotate_;
		// 角度で取得
		direction = {
		    std::cosf(playerData[1]->transform_.rotate_),
		    std::sinf(playerData[1]->transform_.rotate_)};
		direction *= kOffsetOption_;
		playerData[1]->transform_.position_ = playerTopData->transform_.position_ + direction;
	case kCanonLow:
		playerData[2]->isUse_ = true;
		playerData[2]->transform_.rotate_ = playerTopData->transform_.rotate_ + 0.3f;
		// 角度で取得
		direction = {
		    std::cosf(playerData[2]->transform_.rotate_),
		    std::sinf(playerData[2]->transform_.rotate_)};
		direction *= kOffsetOption_;
		playerData[2]->transform_.position_ = playerTopData->transform_.position_ + direction;
		break;
	case kCountofCanonType:
	default:
		break;
	}
}