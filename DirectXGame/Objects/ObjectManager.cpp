#include "ObjectManager.h"

#include "TextureManager.h"

ObjectManager* ObjectManager::GetInstance() {
	static ObjectManager instance;
	return &instance;
}

void ObjectManager::Initialize() {
	objDatas_.clear();
	objDatas_[OBJType::kPlayer];
	objDatas_[OBJType::kBlock];
	objDatas_[OBJType::kEffect];

	spriteBaseDatas_.clear();
	spriteDatas_.clear();

	textureDatas_.clear();
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
	for (SpriteEx& buffer : spriteDatas_[name]) {
		// 使われていたら
		if (buffer.isActive_) {
			continue;
		}
		// 使われていないなら描画
		buffer.data_->Draw();
		buffer.isActive_ = true;
	}
}

void ObjectManager::DrawSprite(const std::string& name, const WorldTransform& wr) {
	if (!textureDatas_.contains(name)) {
		return;
	}
	for (SpriteEx& buffer : spriteDatas_[name]) {
		// 使われていたら
		if (buffer.isActive_) {
			continue;
		}
		buffer.data_->SetSize({wr.scale_.x, wr.scale_.y});
		buffer.data_->SetRotation(wr.rotation_.z);
		buffer.data_->SetPosition({wr.translation_.x, wr.translation_.y});
		// 使われていないなら描画
		buffer.data_->Draw();
		buffer.isActive_ = true;
	}
}
void ObjectManager::DrawSprite(const std::string& name, const WorldTransform* wr) {
	if (!textureDatas_.contains(name)) {
		return;
	}
	for (SpriteEx& buffer : spriteDatas_[name]) {
		// 使われていたら
		if (buffer.isActive_) {
			continue;
		}
		buffer.data_->SetSize({wr->scale_.x, wr->scale_.y});
		buffer.data_->SetRotation(wr->rotation_.z);
		buffer.data_->SetPosition({wr->translation_.x, wr->translation_.y});
		// 使われていないなら描画
		buffer.data_->Draw();
		buffer.isActive_ = true;
	}
}

void ObjectManager::CreateSprite(const std::string& name, int num) {
	if (!textureDatas_.contains(name)) {
		return;
	}
	for (size_t i = 0; i < num; i++) {
		SpriteEx buff;
		memcpy(
		    buff.data_.get(), spriteBaseDatas_[name].begin()->get(),
		    sizeof(spriteBaseDatas_[name].begin()->get()));
		buff.isActive_ = false;
	}
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

void ObjectManager::DrawReset() {
	for (auto& data : spriteDatas_) {
		for (auto& buffer : data.second) {
			buffer.isActive_ = false;
		}
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

	spriteBaseDatas_[name].emplace_back(
	    Sprite::Create(textureHandle, {0, 0}, {1, 1, 1, 1}, {0.5f, 0.5f}));

	/*SpriteEx buffer;
	buffer.isActive_ = false;
	buffer.data_.reset(Sprite::Create(textureHandle, {0, 0}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	spriteDatas_[name].push_back(buffer);*/
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

	spriteBaseDatas_[name].emplace_back(
	    Sprite::Create(textureHandle, {0, 0}, {1, 1, 1, 1}, {0.5f, 0.5f}));

	// SpriteEx buffer;
	// buffer.isActive_ = false;
	// buffer.data_.reset(Sprite::Create(textureHandle, {0, 0}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	// spriteDatas_[name].push_back(buffer);
	for (auto& object : objDatas_[type]) {
		object->SetTextureName(name);
		SpriteData data = {
		    .name_{name},
		    .max_{1},
		};
		object->GetSpriteData().push_back(data);
	}
}

IObject* ObjectManager::GetOneObject(OBJType type) {
	if (!objDatas_.contains(type)) {
		return nullptr;
	}
	return objDatas_[type].begin()->get();
}

// void ObjectManager::SetSpritePosition(const std::string& name, const Vector2& position) {
//	if (!textureDatas_.contains(name)) {
//		return;
//	}
//	//spriteDatas_[name]->SetPosition(position);
// }
//
// Sprite* const ObjectManager::GetSprite(const std::string& name) {
//	if (!textureDatas_.contains(name)) {
//		return nullptr;
//	}
//	for (auto& buffer : spriteDatas_[name]) {
//		if (!buffer.isActive_) {
//			return buffer.data_.get();
//		}
//	}
//	return nullptr;
// }