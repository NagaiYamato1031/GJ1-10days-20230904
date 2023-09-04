#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <optional>

#pragma region 前方宣言

class GlobalConfigs;
class IScene;

#pragma endregion

#pragma region Scene切り替え

enum Scene {
	kTitle, // タイトル
	kPlay,	// プレイ
	kEnd,	// エンド

	kSceneCount,	// カウント用
};

#pragma endregion


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// コンフィグ
	GlobalConfigs* configs_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	std::vector<std::unique_ptr<IScene>> sceneList_;

	IScene* currentScene_ = nullptr;

	std::optional<Scene> nextScene_ = std::nullopt;


};
