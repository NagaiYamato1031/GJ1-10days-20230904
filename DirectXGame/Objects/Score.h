#pragma once

#include "IObject.h"

#include "Vector2.h"

#include "Input.h"

#include <optional>

#include "Block.h"


#pragma region 読み込んだテクスチャ

enum ScoreTexture {
	kScoreBase,
	kScoreBar,
};

#pragma endregion

// Scoreクラス
class Score : public IObject {
public:
	// 仮想関数
	Score();
	~Score() override;

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

	// worldTransformBaseに位置をSet
	void SetPosition(const Vector2 pos);

	// スコア加算
	void AddScore();

	// スコア減算
	void SubtractScore();

private:
	const float scoreDecrease_ = 100.0f;
	const float scoreAdd_ = 10.0;
	float score_;
};
