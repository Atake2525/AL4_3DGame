﻿#include "GameScene.h"
#include <cassert>
#include <3d\AxisIndicator.h>
#include "fstream"
using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete player_;
	for (Enemy* enemy : enemies_ ) {
		delete enemy;
	}
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = KamataEngine::DirectXCommon::GetInstance();
	input_ = KamataEngine::Input::GetInstance();
	audio_ = KamataEngine::Audio::GetInstance();

	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するカメラを指定する(アドレス無し)
	AxisIndicator::GetInstance()->SetTargetCamera(&camera_);

	// プレイヤーの初期化
	modelPlayer_ = KamataEngine::Model::CreateFromOBJ("enemy");
	player_ = new Player();
	player_->Initialize(modelPlayer_, &camera_, Vector3{-4.0f, -4.0f, 20.0f});

	// レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize(player_->GetWorldTransform());
	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	// 敵の初期化
	modelEnemy_ = KamataEngine::Model::CreateFromOBJ("cube");
	LoadEnemyPopData();

	// 天球の初期化
	modelSkydome_ = Model::CreateFromOBJ("skyDome");
	skyDome_ = new Skydome();
	skyDome_->Initialize(modelSkydome_);


	worldTransform_.Initialize();
	camera_.Initialize();
}

void GameScene::Update() { 
	railCamera_->Update();
	player_->Update();
	UpdateEnemyPopCommands();
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Update();
	}
	skyDome_->Update();
	CheckAllCollisions();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// カメラ行列の転送
		camera_.TransferMatrix();
	} else {
		// カメラ行列の更新と転送
		camera_.UpdateMatrix();
	}

	camera_.matView = railCamera_->GetCamera().matView;
	// camera_.matProjection = railCamera_->GetCamera().matProjection;
	camera_.TransferMatrix();
	//worldTransform_.UpdateMatirx();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	KamataEngine::Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	skyDome_->Draw(worldTransform_, camera_);
	player_->Draw(camera_);
	for (Enemy* enemy : enemies_) {
		enemy->Draw(camera_);
	}
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Draw(camera_);
	}

	// 3Dオブジェクト描画後処理
	KamataEngine::Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*> enemyBullets = GetEnemyBullets();

	#pragma	 region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 座標AとBの距離を求める
		float dist = pow((posB.x - posA.x), 2.0f) + pow((posB.y - posA.y), 2.0f) + pow((posB.z - posA.z), 2.0f);
		float len = pow((1.0f + 1.0f), 2.0f);

		// 弾と弾の交差判定
		if (dist <= len) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region 自弾と敵キャラの当たり判定
	for (Enemy* enemy : enemies_) {
		// 敵キャラの座標
		posA = enemy->GetWorldPosition();

		// 敵キャラと自弾全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			// 自弾の座標
			posB = bullet->GetWorldPosition();

			// 座標AとBの距離を求める
			float dist = pow((posB.x - posA.x), 2.0f) + pow((posB.y - posA.y), 2.0f) + pow((posB.z - posA.z), 2.0f);
			float len = pow((1.0f + 1.0f), 2.0f);

			// 弾と弾の交差
			if (dist <= len) {
				// 敵キャラの衝突コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
	#pragma endregion

	#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		// 自弾の座標
		posA = playerBullet->GetWorldPosition();
		for (EnemyBullet* enemyBullet : enemyBullets) {
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			// 座標AとBの距離を求める
			float dist = pow((posB.x - posA.x), 2.0f) + pow((posB.y - posA.y), 2.0f) + pow((posB.z - posA.z), 2.0f);
			float len = pow((1.0f + 1.0f), 2.0f);

			// 弾と弾の交差
			if (dist <= len) {
				// 自弾の衝突コールバックを呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
	#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

// 敵発生コマンド
void GameScene::LoadEnemyPopData() {

	// ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

// 敵発生コマンドの更新
void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (waitFlag) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機完了
			waitFlag = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (std::getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			std::getline(line_stream, word, ',');
			float z = (float)std::stof(word.c_str());

			// 敵を発生させる
			Enemy* enemy = new Enemy();
			enemy->SetPlayer(player_);
			enemy->Initialize(modelEnemy_, Vector3{ x, y, z });
			enemy->SetGameScene(this);
			enemies_.push_back(enemy);

		} else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			waitFlag = true;
			waitTimer = waitTime;

			// ループを抜ける
			break;
		}
	}
}