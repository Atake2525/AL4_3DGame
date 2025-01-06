#pragma once
#include "3d/WorldTransform.h"
#include "3d/Camera.h"
#include "3d/Model.h"
#include "math/MathUtility.h"
#include "kMath.h"
#include "base/TextureManager.h"
#include "EnemyBullet.h"
#include <list>
#include "audio/Audio.h"

// 行動フェーズ
enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

// 前方宣言
class Player;
class GameScene;
class RailCamera;

class Enemy {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity, float delTime);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(KamataEngine::Camera& camera, KamataEngine::ObjectColor* objectColor);

	void SetPlayer(Player* player) { player_ = player; }

	KamataEngine::Vector3 GetWorldPosition();

	// 発射間隔
	static const int kFireInterval = 120;

	void InitializeFirePhase();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	void SetRailCamera(RailCamera* railCamera) { railCamera_ = railCamera; }

	bool IsDead() { return isDead_; }

	bool IsDelete() { return isDelete_; }

	bool IsDamage() { return isDamage_; }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;
	KamataEngine::Model* modelBullet_ = nullptr;

	// HP
	float hp_ = 100.0f;
	// 生きているかのフラグ
	bool isDead_ = false;

	// ダメージを受けたかどうか
	bool isDamage_ = false;
	// ダメージを受けた後の無敵時間
	float damageDelayTime_ = 1.0f;
	float damageDelayTimer_ = 0.0f;

	// ダメージを受けた時に敵を描画する時間
	int inDamageDrawCount_ = 1;
	// カウンター
	uint8_t inDamageDrawCounter_ = 0;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;
	int moveStage_ = 0;

	uint32_t textureHandle_ = 0;

	// 移動速度
	KamataEngine::Vector3 velocity_ = {0.0f, 0.0f, 0.08f};


	// 行動
	void movePhase();
	Phase phase_ = Phase::Approach;
	KamataEngine::Vector3 ApproachVelocity = {0.0f, 0.0f, 0.0f};
	KamataEngine::Vector3 LeaveVelocity = {-0.1f, 0.05f, 0.02f};

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	// 発射タイマー
	int32_t fireTimer_ = 0;
	
	uint32_t deadsound_ = 0;

	// 自キャラ
	Player* player_ = nullptr;

	// 消滅までの時間
	float delTime_ = 0.0f;
	bool isDelete_ = false;
};


