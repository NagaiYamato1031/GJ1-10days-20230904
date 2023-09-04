#pragma once

#include "WorldTransform.h"
#include "ViewProjection.h"
#include <vector>


class GameScene;

class IObject {
public:
	// 仮想関数
	IObject();
	virtual ~IObject();


	virtual void Initialize();

	virtual void Update();

	virtual void Draw();

	virtual void OnCollision();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	virtual void AddlyGlobalConfigs() = 0;
	virtual void AddlyAllGlobalConfigs();

public: // ゲッターセッター
	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent) { worldTransformBase_.parent_ = parent; }

	const WorldTransform& GetWorldTransform() const { return worldTransformBase_; }

	Vector3 GetWorldPosition() const;

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	void SetGameScene(GameScene* scene);

protected:
	float kSphereSize = 1.0f;

	GameScene* gameScene_ = nullptr;

	std::vector<WorldTransform> worldTransforms_;

	WorldTransform worldTransformBase_;

	const ViewProjection* viewProjection_;
};
