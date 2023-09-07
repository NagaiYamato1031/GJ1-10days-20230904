#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include "./Config/GlobalConfigs.h"

#include "./Scenes/SceneTitle.h"
#include "Scenes/ScenePlay.h"

GameScene* GameScene::GetInstance() {
	static GameScene instance;
	return &instance;
}

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	configs_ = GlobalConfigs::GetInstance();
	
	// ゲームシーン内で初期化
	ObjectManager* objectManager = 	ObjectManager::GetInstance();
	objectManager->Initialize();

	// スプライト読み込み
	// 名前 ： パス
	objectManager->LoadTexture("white1x1", "white1x1.png");


	// 一度すべてのシーンを取得する
	sceneList_.clear();

	// タイトル
	sceneList_.emplace_back(new SceneTitle());
	// プレイ
	sceneList_.emplace_back(new ScenePlay());
	// エンド
	//sceneList_.emplace_back(new SceneEnd());

	// タイトルを今のシーンに設定する
	currentScene_ = sceneList_[Scene::kTitle].get();
	currentScene_->Initialize();
	
}

void GameScene::Update() { 
	currentScene_->Update(this);
	
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	currentScene_->DrawBackdrop();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	currentScene_->Draw3D();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	currentScene_->DrawOverlay();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::SetScene(Scene nextScene) { 
	currentScene_ = sceneList_[nextScene].get();
	currentScene_->Initialize();
	
}


