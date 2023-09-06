#pragma once

#include "IObject.h"
#include "Input.h"

#include <vector>

class ObjectManager;

#pragma region 読み込んだテクスチャ

enum PlayerTexture {
	kPlayerLine,
	kPlayerTop,
};

#pragma endregion

#pragma region 構造体

struct SubDivision {
	Vector2 point_;
	float length_;
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
	//// 操作関数
	// void ControlLineUpdate();

	//// 線を描画
	// void DrawLine();

private:
	// シングルトン
	Input* input_ = nullptr;
	ObjectManager* objectManager_ = nullptr;

	// プレイヤーの当たり判定
	Vector3 kPlayerSize_ = {1, 1, 1};

	// プレイヤーにかかる重力
	float kGravity_ = 0.98f;

#pragma region 大砲で撃つ



#pragma endregion

#pragma region 線を書いて誘導するための変数

	//// 線のサイズ
	// int32_t kLineSize_ = 8;
	//// 線を区切る点の数
	// int32_t kDivideLine_ = 10;
	//// 現在の区切り数
	// int32_t dividing_ = 0;

	//// 線の残量
	// float kDirectLeft_ = 10.0f;
	// float directLeft_ = 10.0f;

	//// マウスを押しているか
	// bool isPressed_ = false;

	//// 点 + 次の点までの長さ を保存して線を作り出す
	// std::vector<SubDivision> interpPoints_;

	//// 今クリックしている場所
	// Vector2 clickPosition_ = {0, 0};

	//// 線を描画する時間
	// int32_t kDrawTimeLine_ = 300;
	// int32_t drawTimeLine_ = 0;

#pragma endregion
};
