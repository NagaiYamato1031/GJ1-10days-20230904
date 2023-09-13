#pragma once

#include "IObject.h"
#include "Input.h"

#include <numbers>
#include <vector>

#include "Objects/Score.h"

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

	// ステージのサイズを設定する
	void SetStageSize(const Vector2& size) { stageSize_ = size; }
	void SetStagePosition(const Vector2& position) { stagePosition_ = position; }

	// ステージでの大砲の移動制限を設定する
	void SetCanonMoveSize(const Vector2& size) { canonMoveLimitSize_ = size; }
	void SetCanonMovePosition(const Vector2& position) { canonMoveLimitPosition_ = position; }

	const Transform2D& GetTransform2D();
	const std::vector<Transform2D> GetTransform2Ds();

	void SetIsMouse(bool num) { isMouse_ = num; }
	bool GetIsMouse() const { return isMouse_; }

	void SetPosition(const Vector2& position);
	void SetCanonPosition(const Vector2& position);
	void SetCanonMoveLimitSize(const Vector2& size) { canonMoveLimitSize_ = size; }
	void SetCanonMoveLimitPosition(const Vector2& position) { canonMoveLimitPosition_ = position; }

private:
	// 大砲の操作
	// マウス
	void ControlCanonMouse();
	// キーボード
	void ControlCanonKeyBoard();

	// 大砲のタイプを切り替える
	void ChangeCanonType(bool isUp);

	// 大砲とプレイヤーの衝突判定を取る
	bool CheckHitCollision();

	// 衝突を含めた処理
	void ControlCanonCollision();

	// 発射時の処理
	void CanonShot();

private:
	// シングルトン
	Input* input_ = nullptr;
	Score* score_ = nullptr;

	// 全体的なスケール
	float kAllScale_ = 1.0f;

	// プレイヤーの当たり判定
	float kPlayerSize_ = 64;
	float kCanonSize_ = 64;

	Vector2 canonMoveLimitSize_ = {1280, 420};
	Vector2 canonMoveLimitPosition_ = {0, 300};

	// 追加ソーセージ
	float kOffsetOption_ = -120.0f;

	// プレイヤーにかかる重力
	// float kGravity_ = 0.98f;

	// 大砲に入っているか
	bool isReloaded_ = false;

	// 大砲との判定を行わないフレーム
	int32_t kNonCollisionFrame_ = 30;
	int32_t nonCollisionFrame_ = 30;

	// ステージのサイズを外部から設定
	Vector2 stageSize_;
	Vector2 stagePosition_;

	bool isMouse_ = false;

#pragma region 大砲で撃つ

	// プレイヤーの速度
	Vector2 movementVelocity_ = {0, 0};

	// 大砲のタイプを決める
	CanonType canonType_ = kCanonLow;

	// 大砲で撃った時の速度
	float kCanonPower_ = 5.0f;

	float kCanonMoveSpeed_ = 3.0f;

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
