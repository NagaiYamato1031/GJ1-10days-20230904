#pragma once

#include "IObject.h"

class ObjectManager;

// Plyaerクラス
class Player : public IObject {
public:
	// 仮想関数
	Player();
	~Player() override;

	// 初期化関数
	void Initialize() override;

	// 更新関数
	void Update() override;

	// 描画関数
	void Draw() override;

	// 衝突検知関数
	void OnCollision() override;

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void AddlyGlobalConfigs() override;

private:

	ObjectManager* objectManager_ = nullptr;
};
