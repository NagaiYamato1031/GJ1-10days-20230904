#pragma once

#include <fstream>
#include <list>
#include <memory>
#include <sstream>
#include <string>

#include "Config/GlobalConfigs.h"
#include "Input.h"
#include "Objects/Block.h"

class GameScene;

#ifdef _DEBUG

#include "ImGuiManager.h"

#endif // _DEBUG

class IScene {
public:
	// 仮想関数
	IScene();
	virtual ~IScene();

	// 純粋仮想初期化関数
	virtual void Initialize(GameScene* gameScene) = 0;

	// 純粋仮想更新関数
	virtual void Update() = 0;

	// 純粋仮想描画関数
	// 背景スプライト
	virtual void DrawBackdrop() = 0;
	// 3D
	virtual void Draw3D() = 0;
	// 前景スプライト
	virtual void DrawOverlay() = 0;

	// シーン内の当たり判定を取る
	virtual void CheckAllCollision();

	// .csv を抜いたファイルの名前
	virtual void LoadStageFile(const std::string& name);

	// 読み込んだデータによってブロックを追加する
	virtual void CreateBlocks(const Vector2& position);

	virtual void AddlyConfigs();

	void SetLoadFileName(const std::string& name) { fileName_ = name; }

protected:
	static const uint32_t kStageHeight = 22;
	static const uint32_t kStageWidth = 20;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// 入力
	Input* input_ = nullptr;

	// 経過したフレーム
	uint32_t timeFrame = 0;

	// 現在のステージのブロック配置
	std::vector<std::array<std::array<bool, kStageWidth>, kStageHeight>> stageDatas_;

	// ファイルを既に読み込んでいるかどうか
	std::string fileName_;

	// 今使っているステージの添え字
	uint16_t currentLoadData_ = 0;

	// ブロックのデータ
	std::vector<std::vector<std::unique_ptr<Block>>> blockDatas_;
	std::vector<std::unique_ptr<Block>>* blocks_ = nullptr;
};
