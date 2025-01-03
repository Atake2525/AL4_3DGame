#include "Enemy.h"
#include <cassert>
#include "Player.h"
#include "GameScene.h"

using namespace KamataEngine;

Enemy::~Enemy() {
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity, float delTime) { 
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("uvChecker.png");
	modelBullet_ = Model::CreateFromOBJ("Bullet");

	delTime_ = delTime;

	ApproachVelocity = velocity;
	
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	InitializeFirePhase();
}

void Enemy::Update() { 
	// デスフラグの立った弾を削除
	movePhase();
	moveStage_ = railCamera_->GetMoveStage();

	delTime_--;
	if (delTime_ <= 0.0f) {
		isDelete_ = true;
	}

	// ダメージを受けたら実行
	if (isDamage_) {
		damageDelayTimer_ += 1.0f / 60 / damageDelayTime_ * 10.0f;
		if (damageDelayTimer_ >= 10.0f) {
			isDamage_ = false;
			damageDelayTimer_ = 0.0f;
			inDamageDrawCounter_ = 0;
		}
		inDamageDrawCounter_ += 1;
	}

	// HPが0になったら実行
	if (hp_ <= 0) {
		isDead_ = true;
	}
	//worldTransform_.translation_ -= velocity_;
	worldTransform_.UpdateMatirx();
}

void Enemy::Fire() {
	assert(player_);
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	//Vector3 velocity(0, 0, kBulletSpeed);

	// 速度ベクトルを自機の向きに合わせて回転させる
	//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	Vector3 plWorldPos = player_->GetWorldPosition();
	Vector3 EmWorldPos = GetWorldPosition();

	Vector3 BulletWorldPos = plWorldPos - EmWorldPos;
	if (moveStage_ == 0) {
		plWorldPos.z -= BulletWorldPos.z / 2;
	} else if (moveStage_ == 1) {
		plWorldPos.x -= BulletWorldPos.x / 2;
	} else if (moveStage_ == 2) {
		plWorldPos.z -= BulletWorldPos.z / 2;
	} else if (moveStage_ == 3) {
		plWorldPos.x -= BulletWorldPos.x / 2;
	}
	BulletWorldPos = plWorldPos - EmWorldPos;
	BulletWorldPos = Normalize(BulletWorldPos);
	Vector3 velocity = BulletWorldPos * kBulletSpeed;

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(modelBullet_, worldTransform_.translation_, velocity, railCamera_->GetWorldTransform().rotation_);

	// 弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::movePhase() {
	switch (phase_) {
	case Phase::Approach:
	default:
		// 移動(ベクトルを加算)
		worldTransform_.translation_ += ApproachVelocity;
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		// 移動(ベクトルを加算)
		worldTransform_.translation_ += LeaveVelocity;
		break;
	}
	if (!isDamage_) {
		fireTimer_--;
		if (fireTimer_ == 0) {
			Fire();
			fireTimer_ = kFireInterval;
		}
	}
}

void Enemy::InitializeFirePhase() {
	// 発射タイマーを初期化
	fireTimer_ = kFireInterval;
}

void Enemy::Draw(Camera& camera, ObjectColor* objectColor) {
	if (isDamage_) {
		if (inDamageDrawCount_ & inDamageDrawCounter_) {
			model_->Draw(worldTransform_, camera, objectColor); 
		}
	} else {
		model_->Draw(worldTransform_, camera, objectColor);
	}
}

void Enemy::OnCollision() { 
	hp_ -= 35.0f;
	isDamage_ = true;
}