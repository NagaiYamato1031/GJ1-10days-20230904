#pragma once

#include <list>
#include <memory>

#include "./Objects/ObjectManager.h"
#include "Input.h"

class GameScnee;

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

protected:

	// オブジェクト管理
	ObjectManager* objectManager_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// 入力
	Input* input_ = nullptr;

};
