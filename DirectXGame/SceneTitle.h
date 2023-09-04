#pragma once
#include "IScene.h"

#include "ObjectManager.h"

class SceneTitle : public IScene {
public:

	~SceneTitle() override;

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void DrawBackdrop() override;
	void Draw3D() override;
	void DrawOverlay() override;

private:
	// シーン内の変数

};
