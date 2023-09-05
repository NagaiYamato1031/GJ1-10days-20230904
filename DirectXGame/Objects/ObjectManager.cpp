#include "ObjectManager.h"

#include "TextureManager.h"

ObjectManager* ObjectManager::GetInstance() {
	static ObjectManager instance;
	return &instance;
}

void ObjectManager::Initialize() {
	objDatas_.clear();
	objDatas_[OBJType::kPlayer];
	objDatas_[OBJType::kEnemy];
	objDatas_[OBJType::kEffect];

	spriteDatas_.clear();

	// テクスチャを読み込む
	// AddTexture("white1x1.png", "white1x1.png");
}

void ObjectManager::Update() {
	for (const auto& data : objDatas_) {
		for (const auto& object : data.second) {
			object->Update();
		}
	}
}

void ObjectManager::Draw() {
	for (const auto& data : objDatas_) {
		for (const auto& object : data.second) {
			object->Draw();
		}
	}
}

void ObjectManager::DrawSprite(const std::string& name) {
	if (!textureDatas_.contains(name)) {
		return;
	}
	spriteDatas_[name]->Draw();
}

void ObjectManager::AddObject(OBJType type, IObject* object) {
	if (objDatas_.contains(type)) {
		objDatas_[type].emplace_back(object);
	}
}

void ObjectManager::Clear() {
	for (auto& data : objDatas_) {
		data.second.clear();
	}
}

void ObjectManager::LoadTexture(const std::string& name, const std::string& path) {
	if (name == "" || path == "") {
		return;
	}
	// 名前を登録
	textureDatas_[name] = path;

	// スプライトも登録
	uint32_t textureHandle = TextureManager::Load(path);
	spriteDatas_[name].reset(Sprite::Create(textureHandle, {0, 0}, {1, 1, 1, 1}, {0.5f, 0.5f}));
}

void ObjectManager::LoadTexture(const std::string& name, const std::string& path, OBJType type) {
	if (name == "" || path == "") {
		return;
	}
	if (!objDatas_.contains(type)) {
		return;
	}

	// 名前を登録
	textureDatas_[name] = path;

	// スプライトも登録
	uint32_t textureHandle = TextureManager::Load(path);
	spriteDatas_[name].reset(Sprite::Create(textureHandle, {0, 0}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	for (auto& object : objDatas_[type]) {
		object->SetTextureName(name);
	}
}

void ObjectManager::SetSpritePosition(const std::string& name, const Vector2& position) {
	if (!textureDatas_.contains(name)) {
		return;
	}
	spriteDatas_[name]->SetPosition(position);
}

Sprite* const ObjectManager::GetSprite(const std::string& name) {
	if (!textureDatas_.contains(name)) {
		return nullptr;
	}
	return spriteDatas_[name].get();
}