#include "RailCamera.h"
#include "imgui.h"
#include "kMath.h"

using namespace KamataEngine;

void RailCamera::Initialize(const WorldTransform& worldTransform) {
	// ワールドトランスフォームの初期化
	worldTransform_.matWorld_ = worldTransform.matWorld_;
	worldTransform_.rotation_ = worldTransform.rotation_;
	firstPos_ = worldTransform_.translation_;
	// カメラの初期化
	camera.Initialize();
}

void RailCamera::Update() { 

	Move();

	worldTransform_.translation_ = translation_;
	worldTransform_.rotation_ = rotation_;
	// スケール、回転、平行移動を合成して行列を計算する
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// カメラオブジェクトのワールド行列からビュー行列を計算する
	camera.matView = Inverse(worldTransform_.matWorld_);


	ImGui::Begin("Camera");
	ImGui::DragFloat3("Translation", &translation_.x, 0.1f);
	ImGui::DragFloat3("Rotation", &rotation_.x, 0.1f);
	ImGui::End();

}

bool RailCamera::MoveAmount(Vector3 movePoint, float moveTime) { 
	if (!isCheckTranslate_) {
		firstPos_ = worldTransform_.translation_;
		isCheckTranslate_ = true;
	}

	moveTimer_ += 1.0f / 60 / moveTime;
	translation_.x = easeInOut(moveTimer_, firstPos_.x, firstPos_.x + movePoint.x);
	translation_.y = easeInOut(moveTimer_, firstPos_.y, firstPos_.y + movePoint.y);
	translation_.z = easeInOut(moveTimer_, firstPos_.z, firstPos_.z + movePoint.z);

	if (moveTimer_ >= 1.0f) {
		isCheckTranslate_ = false;
		moveTimer_ = 0.0f;
		return true;
	}
	return false;
}

bool RailCamera::RotateAmount(Vector3 rotatePoint, float rotateTime) {
	if (!isCheckRotate_) {
		firstRotate_ = worldTransform_.rotation_;
		isCheckRotate_ = true;
	}

	rotateTimer_ += 1.0f / 60 / rotateTime;
	rotation_.x = easeInOut(rotateTimer_, firstRotate_.x, firstRotate_.x + rotatePoint.x);
	rotation_.y = easeInOut(rotateTimer_, firstRotate_.y, firstRotate_.y + rotatePoint.y);
	rotation_.z = easeInOut(rotateTimer_, firstRotate_.z, firstRotate_.z + rotatePoint.z);

	if (rotateTimer_ >= 1.0f) {
		isCheckRotate_ = false;
		rotateTimer_ = 0.0f;
		return true;
	}
	return false;
}

void RailCamera::Move() {
	if (moveStage == 0) {
		if (MoveAmount(Vector3{0.0f, 0.0f, 1500.0f}, 30.0f)) {
			moveStage = 1;
		}
	}
}