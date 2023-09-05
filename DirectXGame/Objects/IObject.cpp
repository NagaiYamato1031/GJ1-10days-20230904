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
	worldTransforms_.clear();
	textureName_.clear();

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Base";
	configs->CreateGroup(groupName);

	
	// 一度更新する
	worldTransformBase_.UpdateMatrix();

	for (WorldTransform& wt : worldTransforms_) {
		wt.UpdateMatrix();
	}
}

void IObject::Update() {
	worldTransformBase_.UpdateMatrix();
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
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

void IObject::SetGameScene(GameScene* scene) { gameScene_ = scene; }

void IObject::SetTextureName(const std::string& name) { textureName_.push_back(name); }
