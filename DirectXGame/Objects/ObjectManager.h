#pragma once

#include <map>
#include <memory>

#include "./Objects/IObject.h"
#include "Sprite.h"

#pragma region ObjType

enum OBJType {
	kPlayer, // プレイヤー
	kBlock,  // ブロック
	kEffect, // エフェクト

	kCountofOBJType, // カウント
};

#pragma endregion

#pragma region Struct

using uniqueSprite = std::unique_ptr<Sprite>;

struct SpriteEx {
	int16_t isActive_;
	uniqueSprite data_;
};

#pragma endregion

#pragma region USING

using Objects = std::vector<std::unique_ptr<IObject>>;
using Sprites = std::vector<SpriteEx>;

#pragma endregion

class ObjectManager {
public:
	static ObjectManager* GetInstance();

	// ゲームシーン内で初期化
	void Initialize();

	// シーン内で実行
	void Update();

	// シーン内で実行
	// 今のところ 2D のみの描画
	void Draw();

	void DrawSprite(const std::string& name);
	// WorldTransform を渡して描画させる
	void DrawSprite(const std::string& name, const WorldTransform& wr);
	void DrawSprite(const std::string& name, const WorldTransform* wr);

	void CreateSprite(const std::string& name, int num);

	void AddObject(OBJType type, IObject* object);

	// 名前とパスの登録
	void LoadTexture(const std::string& name, const std::string& path);
	void LoadTexture(const std::string& name, const std::string& path, OBJType type);

	//// スプライトの場所の変更
	// void SetSpritePosition(const std::string& name, const Vector2& position);

	//// 参照アドレスは変えないが、参照もとの情報は書き換えれる ?
	// Sprite* const GetSprite(const std::string& name);

	// シーン切り替え時に実行
	void Clear();

	// フレーム終了時に実行
	void DrawReset();

	IObject* GetOneObject(OBJType type);

private:
	// コピー禁止
	ObjectManager() = default;
	~ObjectManager() = default;
	ObjectManager(const ObjectManager& obj) = delete;
	const ObjectManager& operator=(const ObjectManager& obj) = delete;

private:
	// オブジェクトのデータ
	std::map<OBJType, Objects> objDatas_;

	// ゲームで使うスプライトを保存
	std::map<std::string, std::vector<uniqueSprite>> spriteBaseDatas_;

	// 描画枚数分のスプライトを保存
	std::map<std::string, Sprites> spriteDatas_;
	// スプライトの名前を保存
	std::map<std::string, std::string> textureDatas_;
};
