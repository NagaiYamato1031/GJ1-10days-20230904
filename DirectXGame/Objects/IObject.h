#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Sprite.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Vector2.h"

class GameScene;

struct Transform2D {
	int16_t isUse_;
	Vector2 scale_;
	float rotate_;
	Vector2 position_;
};

using UniqueSprite = std::unique_ptr<Sprite>;

struct SpriteData {
	Transform2D transform_;
	UniqueSprite sprite_;
};

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

	// 描画後にスプライトの描画状態リセット
	// virtual void DrawReset();

public: // ゲッターセッター
	// 親となるワールドトランスフォームをセット
	void SetParent(const WorldTransform* parent) { worldTransformBase_.parent_ = parent; }

	// WorldTransform を取得
	const WorldTransform& GetWorldTransform() const { return worldTransformBase_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition() const;
	Vector2 GetPosition() const;

	// 作成するスプライトの数を取得する
	// std::vector<SpriteData>& GetSpriteData();

	// ビュープロジェクションを設定
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	// ゲームシーンを設定
	void SetGameScene(GameScene* scene);

	// テクスチャ登録
	// void SetTextureName(const std::string& name);

	// スプライトの登録
	void SetSprite(int index, const std::string& path);

protected:
	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// 基点となる WorldTransform
	WorldTransform worldTransformBase_;

	// モデルなどを動かすための WorldTransform
	// std::vector<std::unique_ptr<WorldTransform>> worldTransforms_;

	// 画像の添え字( enum希望 )と、スプライトの Transform とデータを保存
	std::map<int, std::vector<std::unique_ptr<SpriteData>>> sprites_;

	// カメラ
	const ViewProjection* viewProjection_ = nullptr;
};
