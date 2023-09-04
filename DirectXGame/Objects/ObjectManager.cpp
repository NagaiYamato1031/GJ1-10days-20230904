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

	// �e�N�X�`����ǂݍ���
	//AddTexture("white1x1.png", "white1x1.png");
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
	spriteDatas_[textureDatas_[name]]->Draw();
}

void ObjectManager::AddObject(OBJType type, IObject* object) {
	if (!objDatas_.contains(type)) {
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
	// ���O��o�^
	textureDatas_[name] = path;

	// �X�v���C�g���o�^
	uint32_t textureHandle = TextureManager::Load(path);
	spriteDatas_[name].reset(Sprite::Create(textureHandle, {0, 0}, {1, 1, 1, 1}, {0.5f, 0.5f}));
}

void ObjectManager::SetSpritePosition(const std::string& name, const Vector2& position) {
	if (!textureDatas_.contains(name)) {
		return;
	}
	spriteDatas_[textureDatas_[name]]->SetPosition(position);
}

Sprite* const ObjectManager::GetSprite(const std::string& name) {
	if (!textureDatas_.contains(name)) {
		return;
	}
	return spriteDatas_[textureDatas_[name]].get();
}