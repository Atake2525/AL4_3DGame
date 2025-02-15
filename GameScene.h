﻿#pragma once

#include "3d/WorldTransform.h"
#include "audio/Audio.h"
#include <base\DirectXCommon.h>
#include <input\Input.h>
#include "3d/Model.h"
#include "2d/Sprite.h"
#include "3d/Camera.h"
#include "3d/WorldTransform.h"
#include <math\MathUtility.h>
#include <math\Matrix4x4.h>
#include <math\Vector3.h>
#include <math\Vector4.h>
#include <cmath>
#include "3d/DebugCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "ground.h"
#include "RailCamera.h"
#include "EnemyBullet.h"
#include "3d/LightGroup.h"
#include <sstream>
#include <iostream>
#include "Object.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	bool IsFinished() const { return finished_; }

	bool IsComplete() const { return complete_; }

	bool IsDead() const { return dead_; }

private: // メンバ変数
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;

	uint32_t deadSound_ = 0;
	uint32_t damageSound_ = 0;
	uint32_t bgm_ = 0;
	bool playBgm_ = false;

	// デバッグカメラ
	bool isDebugCameraActive_ = false;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;

	Player* player_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

	Object* object_ = nullptr;

	KamataEngine::ObjectColor black_;
	KamataEngine::ObjectColor white_;

	std::list<Enemy*> enemies_;
	KamataEngine::Model* modelEnemy_ = nullptr;

	std::list<EnemyBullet*> enemyBullets_;

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetEnemyBullets() const { return enemyBullets_; }

	// 天球
	Skydome* skyDome_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;

	// 地面
	Ground* ground_ = nullptr;
	KamataEngine::Model* modelGround_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	// 敵のキルカウント
	int killCounter_ = 0;
	// キル目標
	int killCount_ = 10;
	// 目標達成したかのフラグ
	bool complete_= false;

	// 敵発生コマンドの待機
	bool waitFlag = false;
	int32_t waitTimer = 0;

	// 終了
	bool finished_ = false;
	bool dead_ = false;
	bool deadAnim_ = false;
	float deadTimer_ = 0.0f;
	float deadTime_ = 0.5f;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
