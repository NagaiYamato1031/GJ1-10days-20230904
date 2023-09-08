#pragma once

#include "IObject.h"
#include "Input.h"

#include <numbers>
#include <vector>

#pragma region 読み込んだテクスチャ

enum PlayerTexture {
	kPlayerLine,
	kPlayerTop,
	kPlayerBody,
	kPlayerCanon,

	kCountofPlayerTexture,
};

#pragma endregion

#pragma region 構造体やenum

enum CanonType {
	kCanonLow,
	kCanonNormal,
	kCanonHigh,

	kCountofCanonType,
};

#pragma endregion

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

	// 大砲の操作
	// マウス
	void ControlCanonMouse();
	// キーボード
	void ControlCanonKeyBoard();

	// 大砲のタイプを切り替える
	void ChangeCanonType(bool isUp);

	// 大砲の描画
	void DrawCanon();


private:
	// シングルトン
	Input* input_ = nullptr;

	// プレイヤーの当たり判定
	Vector3 kPlayerSize_ = {1, 1, 1};

	// プレイヤーにかかる重力
	//float kGravity_ = 0.98f;

#pragma region 大砲で撃つ

	// プレイヤーの速度
	Vector2 movementVelocity_ = {0, 0};

	// 大砲のタイプを決める
	CanonType canonType_ = kCanonNormal;

	// 大砲で撃った時の速度
	float kCanonPower_ = 5.0f;

	// 大砲が固定されているかどうか
	bool isLockedCanon_ = false;

	// 大砲の回転方向
	int32_t canonDirection_ = 1;
	// 回転速度
	float kCanonRotateSpeed_ = 0.07f;

	// 大砲の位置
	Vector2 canonPosition_ = {0, 0};

	// マウスの位置として記録
	Vector2 mousePosition_ = {0, 0};

	// 大砲の角度
	float canonRotate_ = 0.0f;

	// 大砲の角度制限(上方向から左右に広がる)
	float kCanonRotateLimit_ = static_cast<float>(std::numbers::pi) / 3.0f;

#pragma endregion

};
