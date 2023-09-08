#include "IObject.h"
#include "GameScene.h"

#include "./Config/GlobalConfigs.h"
#include "Mymath.h"

IObject::IObject() {
	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Base";
	configs->CreateGroup(groupName);
}
IObject::~IObject() {}

void IObject::Initialize() {
	worldTransformBase_.Initialize();
	sprites_.clear();

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Base";
	configs->CreateGroup(groupName);

	// 一度更新する
	worldTransformBase_.UpdateMatrix();
}

void IObject::Update() {
	worldTransformBase_.UpdateMatrix();
	SpriteUpdate();
}

void IObject::Draw() {}

void IObject::OnCollision() {}

void IObject::AddlyAllGlobalConfigs() {
	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Base";

	// オブジェクト内で共通する設定を読み込む
	configs;
	groupName;

	// オーバーライドされた関数を呼び出す
	AddlyGlobalConfigs();
}

Vector3 IObject::GetWorldPosition() const {
	Vector3 worldPos;

	// worldPos = Mymath::TransformNormal(worldTransform_.translation_, worldTransform_.matWorld_);

	// ワールド座標の平行移動成分を取得
	worldPos.x = worldTransformBase_.matWorld_.m[3][0];
	worldPos.y = worldTransformBase_.matWorld_.m[3][1];
	worldPos.z = worldTransformBase_.matWorld_.m[3][2];
	return worldPos;
}

Vector2 IObject::GetPosition() const {
	Vector2 worldPos;

	// worldPos = Mymath::TransformNormal(worldTransform_.translation_, worldTransform_.matWorld_);

	// ワールド座標の平行移動成分を取得
	worldPos.x = worldTransformBase_.matWorld_.m[3][0];
	worldPos.y = worldTransformBase_.matWorld_.m[3][1];
	return worldPos;
}

void IObject::SetGameScene(GameScene* scene) { gameScene_ = scene; }

void IObject::SetSprite(int index, const std::string& path) {
	Vector2 pos{0, 0};
	Vector4 color{1, 1, 1, 1};
	Vector2 ancher{0.5f, 0.5f};
	// ハンドル
	uint32_t handle = TextureManager::Load(path);
	// スプライト
	UniqueSprite sprite;
	sprite.reset(Sprite::Create(handle, pos, color, ancher));
	// トランスフォーム
	Transform2D transform{
	    .size_{sprite->GetSize()},
	    .scale_{1, 1},
	    .rotate_{0},
	    .position_{0, 0},
	};
	SpriteData* data = new SpriteData;
	data->isUse_ = false;
	data->transform_ = transform;
	data->sprite_.reset(Sprite::Create(handle, pos, color, ancher));
	sprites_[index].emplace_back(data);
}
void IObject::SetSprite(int index, int num, const std::string& path) {
	Vector2 pos{0, 0};
	Vector4 color{1, 1, 1, 1};
	Vector2 ancher{0.5f, 0.5f};
	// ハンドル
	uint32_t handle = TextureManager::Load(path);
	for (size_t i = 0; i < num; i++) {
		// スプライト
		UniqueSprite sprite;
		sprite.reset(Sprite::Create(handle, pos, color, ancher));
		// トランスフォーム
		Transform2D transform{
		    .size_{sprite->GetSize()},
		    .scale_{1, 1},
		    .rotate_{0},
		    .position_{0, 0},
		};
		SpriteData* data = new SpriteData;
		data->isUse_ = false;
		data->transform_ = transform;
		data->sprite_.reset(Sprite::Create(handle, pos, color, ancher));
		sprites_[index].emplace_back(data);
	}
}

void IObject::SpriteUpdate() {
	for (int i = 0; i < sprites_.size(); i++) {
		for (auto& data : sprites_[i]) {
			if (data->isUse_) {
				Vector2 size;
				size.x = data->transform_.size_.x * data->transform_.scale_.x;
				size.y = data->transform_.size_.y * data->transform_.scale_.y;
				data->sprite_->SetSize(size);
				data->sprite_->SetPosition(data->transform_.position_);
				data->sprite_->SetRotation(data->transform_.rotate_);
			}
		}
	}
}

void IObject::FetchSpriteData() {
	for (int i = 0; i < sprites_.size(); i++) {
		for (auto& data : sprites_[i]) {
			data->transform_.position_ = data->sprite_->GetPosition();
			data->transform_.rotate_ = data->sprite_->GetRotation();
			data->transform_.size_ = data->sprite_->GetSize();
		}
	}
}

void IObject::DrawSprite() {
	for (int i = 0; i < sprites_.size(); i++) {
		for (auto& data : sprites_[i]) {
			if (data->isUse_) {
				data->sprite_->Draw();
			}
		}
	}
}