#include "IScene.h"

IScene::IScene() {}

IScene::~IScene() {}

void IScene::CheckAllCollision() {}

void IScene::LoadStageFile(const std::string& name) {

	if (name.empty()) {
		return;
	}

	// ファイルを開く
	std::ifstream file;
	file.open("Resources/Sausage/" + name + ".csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	std::stringstream ss;
	ss << file.rdbuf();

	// ファイルを閉じる
	file.close();

	static Vector2 pos{0, 0};

	// 1 行分の文字列を入れる変数
	std::string line;

	int32_t locateY = 0;
	int32_t locateX = 0;

	// データ作成
	std::array<std::array<bool, kStageWidth>, kStageHeight> buffer;
	std::array<bool, kStageWidth> buff;
	buff.fill(false);
	buffer.fill(buff);
	// コマンド実行ループ
	while (std::getline(ss, line)) {

		// 1 行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// , 区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');
		// 入っていない行は空
		if (word == "") {
			// 登録
			stageDatas_.push_back(buffer);
			// 行を飛ばす
			locateY = 0;
			locateX = 0;
			continue;
		}
		// 壁
		else {
			// データ作成(初期化)
			buff.fill(false);
			// ファイル読み込み
			int data = (std::atoi(word.c_str()));
			// ブロックが置かれている : true
			if (data == 1) {
				buff[locateX] = true;
			} else {
				buff[locateX] = false;
			}
			for (size_t i = 1; i < kStageWidth; i++) {
				locateX++;
				std::getline(line_stream, word, ',');
				data = (std::atoi(word.c_str()));
				// ブロックが置かれている : true
				if (data == 1) {
					buff[locateX] = true;
				} else {
					buff[locateX] = false;
				}
			}
		}
		buffer[locateY] = buff;
		locateY++;
		locateX = 0;
	}
	stageDatas_.push_back(buffer);
}

void IScene::CreateBlocks(const Vector2& position) {
	blockDatas_.clear();
	for (size_t index = 0; index < stageDatas_.size(); index++) {
		// 空データを作成
		blockDatas_.push_back({});
		auto& stage = stageDatas_[index];

		const float kBlockSize = 32.0f;

		for (size_t i = 0; i < stage.size(); i++) {
			for (size_t j = 0; j < stage[i].size(); j++) {
				// 行と列にブロックが置かれていたら
				if (stage[i][j]) {
					Block* block = new Block();
					block->Initialize();
					block->SetPosition(
					    {kBlockSize * j + kBlockSize / 2.0f + position.x,
					     kBlockSize * i + kBlockSize / 2.0f + position.y});
					blockDatas_[index].emplace_back(block);
				}
			}
		}
	}
}

void IScene::AddlyConfigs() {}