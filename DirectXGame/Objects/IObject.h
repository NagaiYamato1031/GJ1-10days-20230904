#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include <string>
#include <vector>
#include <memory>

#include "Vector2.h"

class GameScene;

// 仮想クラス
class IObject {
public:
	// 仮想関数
	IObject();
	virtual ~IObject();

	// 仮想初期化関数
	virtual void Initialize();

	// 仮想更新関数
	virtual void Update();

	// 仮想描画関数
	virtual void Draw();

	// 仮想衝突検知関数
	virtual void OnCollision();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	virtual void AddlyGlobalConfigs() = 0;
	virtual void AddlyAllGlobalConfigs();

public: // ゲッターセッター
	// 親となるワールドトランスフォームをセット
	void SetParent(const WorldTransform* parent) { worldTransformBase_.parent_ = parent; }

	// WorldTransform を取得
	const WorldTransform& GetWorldTransform() const { return worldTransformBase_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition() const;
	Vector2 GetPosition() const;

	// ビュープロジェクションを設定
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	// ゲームシーンを設定
	void SetGameScene(GameScene* scene);

	// テクスチャ登録
	void SetTextureName(const std::string& name);

protected:
	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// 基点となる WorldTransform
	WorldTransform worldTransformBase_;

	// モデルなどを動かすための WorldTransform
	std::vector<std::unique_ptr<WorldTransform>> worldTransforms_;
	
	// 使う画像の名前を保存
	std::vector<std::string> textureName_;

	int32_t kUseSpriteMax_ = 1;

	// カメラ
	const ViewProjection* viewProjection_ = nullptr;
};
