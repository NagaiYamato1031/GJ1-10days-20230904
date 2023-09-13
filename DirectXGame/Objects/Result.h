#pragma once

#include "IObject.h"

#include "Vector2.h"

#include "Input.h"

#include "Objects/Score.h"

#include <optional>

#pragma region 読み込んだテクスチャ

enum ResultTexture {
	kResultBack,
	kResultBase,
	kResultStar,
	kResultNumber,
};

#pragma endregion


// Resultクラス
class Result : public IObject {
public:
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


private:
	Score* score_;
	struct transform {
		Vector2 pos;
		Vector2 scale;
		Vector2 size;
	};

	transform base_;
	transform star_[3];
	//transform starBlank_[3];
	transform number_[6];

	const float size_ = 128.0f;


};
