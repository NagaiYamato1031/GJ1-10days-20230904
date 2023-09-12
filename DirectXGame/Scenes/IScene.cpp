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

	// コマンド実行ループ
	while (std::getline(ss, line)) {
		// 1 行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// , 区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');
		// 入っていない行は空
		if (word.empty()) {
			// 行を飛ばす
			locateY = 0;
			locateX = 0;
			continue;
		}
		// 壁
		else {
			// データ作成
			std::array<std::array<bool, kStageWidth>, kStageHeight> buffer;
			stageDatas_.push_back(buffer);
			// ファイル読み込み
			for (size_t i = 0; i < kStageHeight; i++) {
				std::getline(line_stream, word, ',');
				uint16_t data = static_cast<uint16_t>(std::atoi(word.c_str()));
				// ブロックが置かれている : true
				buffer[locateY][locateX] = data == 1 ? true : false;
				locateX++;
			}
		}
		locateY++;
		locateX = 0;
	}
}

void IScene::CreateBlocks() {
	auto& stage = stageDatas_[nextLoadData_];

	const float kBlockSize = 32.0f;

	for (size_t i = 0; i < stage.size(); i++) {
		for (size_t j = 0; j < stage[i].size(); j++) {
			// 行と列にブロックが置かれていたら
			if (stage[i][j]) {
				Block* block = new Block();
				block->SetPosition(
				    {kBlockSize * i + kBlockSize / 2.0f, kBlockSize * j + kBlockSize / 2.0f});
			}
		}
	}
}

void IScene::AddlyConfigs() {}