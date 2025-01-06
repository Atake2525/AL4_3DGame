#include "Player.h"
#include <imgui.h>
#include "algorithm"
using namespace KamataEngine;

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

// ワールド座標を取得
Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド座標の平行同成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::Initialize(Model* model, KamataEngine::Camera* camera, const Vector3& position) {
	input_ = KamataEngine::Input::GetInstance();
	audio_ = Audio::GetInstance();
	shotSound_ = audio_->LoadWave("shot.wav");
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	objectColor_.Initialize();
	camera_ = camera;
	bulletMode_ = Model::CreateFromOBJ("Bullet");
	hptextureHandle_ = TextureManager::Load("hp.png");
	hpSprite_ = Sprite::Create(hptextureHandle_, Vector2{0.0f, 690.0f});
	delayTimer_ = 0.0f;
	isDead_ = false;
	hp_ = 100;
}

void Player::Update() {
	wolk();
	//Rotate();
	Attack();

	// ダメージを受けたら実行
	if (isDamage_) {
		float nowHp = float(100 - hp_);
		float hpPos = 0 - nowHp;
		hpSprite_->SetPosition(Vector2{hpPos, 690.0f});

		damageDelayTimer_ += 1.0f / 60 / damageDelayTime_ * 10.0f;
		if (damageDelayTimer_ >= 10.0f) {
			isDamage_ = false;
			damageDelayTimer_ = 0.0f;
			inDamageDrawCounter_ = 0;
		}
		inDamageDrawCounter_ += 1;
	}
	if (hp_ <= 0) {
		isDead_ = true;
	}

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	worldTransform_.UpdateMatirx();
	//worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//worldTransform_.TransferMatrix();
#ifdef _DEBUG
	ImGui::Begin("PlayerState");
	ImGui::DragFloat3("pos", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.1f);
	ImGui::End();
#endif // _DEBUG
}

void Player::wolk() { 
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_A) && input_->PushKey(DIK_D)) {
		move.x = 0.0f;
	}

	/*if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_S) && input_->PushKey(DIK_W)) {
		move.y = 0.0f;
	}*/

	worldTransform_.translation_ += move;
	// 範囲を超えないように処理
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);
}

void Player::Rotate() {
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_Q)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_E)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE) && !isAttack_) {
		audio_->PlayWave(shotSound_);
		// 弾の速度
		const float kBulletSpeed = 2.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(bulletMode_, GetWorldPosition(), velocity, worldTransform_.parent_->rotation_);

		// 弾を登録する
		bullets_.push_back(newBullet);
		isAttack_ = true;
	} else if (isAttack_) {
		delayTimer_ += 1.0f / 60;
		if (delayTimer_ >= delayTime) { 
			isAttack_ = false; 
			delayTimer_ = 0.0f;
			audio_->StopWave(shotSound_);
		}
	}
}


void Player::Draw(KamataEngine::Camera& camera) { 
	if (isDamage_ && !isDead_) {
		if (inDamageDrawCount_ & inDamageDrawCounter_) {
			model_->Draw(worldTransform_, camera, &objectColor_);
		}
	} else {
		model_->Draw(worldTransform_, camera, &objectColor_);
	}
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}
}

void Player::OnCollision() { 
	hp_ -= 20;
	isDamage_ = true;
}

void Player::SetParent(const WorldTransform* parent) {
	// 親関係を結ぶ
	worldTransform_.parent_ = parent;
}

void Player::DrawSpeite() { hpSprite_->Draw(); }