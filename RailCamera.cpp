#include "RailCamera.h"
#include "imgui.h"
#include "kMath.h"
#include "GameScene.h"

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
	finish_ = gameScene_->IsComplete();

	Move();

	worldTransform_.translation_ = translation_;
	worldTransform_.rotation_ = rotation_;
	// スケール、回転、平行移動を合成して行列を計算する
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// カメラオブジェクトのワールド行列からビュー行列を計算する
	camera.matView = Inverse(worldTransform_.matWorld_);

#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::DragFloat3("Translation", &translation_.x, 0.1f);
	ImGui::DragFloat3("Rotation", &rotation_.x, 0.1f);
	ImGui::End();
#endif // DEBUG
}

bool RailCamera::MoveAmount(Vector3 movePoint, float moveTime) { 
	if (!translateEnd_) {
		if (!isCheckTranslate_) {
			firstPos_ = translation_;
			isCheckTranslate_ = true;
		}

		moveTimer_ += 1.0f / 60 / moveTime;
		translation_.x = easeInOut(moveTimer_, firstPos_.x, firstPos_.x + movePoint.x);
		translation_.y = easeInOut(moveTimer_, firstPos_.y, firstPos_.y + movePoint.y);
		translation_.z = easeInOut(moveTimer_, firstPos_.z, firstPos_.z + movePoint.z);

		if (moveTimer_ >= 1.0f) {
			isCheckTranslate_ = false;
			moveTimer_ = 0.0f;
			translateEnd_ = true;
			return true;
		}
		return false;
	} else {
		return true;
	}
}

bool RailCamera::RotateAmount(Vector3 rotatePoint, float rotateTime) {
	if (!rotateEnd_) {
		if (!isCheckRotate_) {
			firstRotate_ = worldTransform_.rotation_;
			isCheckRotate_ = true;
		}
		Vector3 rotation;
		rotation.x = rotatePoint.x * (float(M_PI) / 180.0f);
		rotation.y = rotatePoint.y * (float(M_PI) / 180.0f);
		rotation.z = rotatePoint.z * (float(M_PI) / 180.0f);

		rotateTimer_ += 1.0f / 60 / rotateTime;
		rotation_.x = easeInOut(rotateTimer_, firstRotate_.x, firstRotate_.x + rotation.x);
		rotation_.y = easeInOut(rotateTimer_, firstRotate_.y, firstRotate_.y + rotation.y);
		rotation_.z = easeInOut(rotateTimer_, firstRotate_.z, firstRotate_.z + rotation.z);

		if (rotateTimer_ >= 1.0f) {
			isCheckRotate_ = false;
			rotateTimer_ = 0.0f;
			rotateEnd_ = true;
			return true;
		}
		return false;
	} else {
		return true;
	}
}

bool RailCamera::TranslationAmount(Vector3 translationPoint, float translatoinTime) {
	if (!translationEnd_) {
		if (!isCheckTranslation_) {
			firstTranslation_ = moveTranslation_;
			isCheckTranslation_ = true;
		}

		translationTimer_ += 1.0f / 60 / translatoinTime;
		moveTranslation_.x = easeInOut(translationTimer_, firstTranslation_.x, translationPoint.x);
		moveTranslation_.y = easeInOut(translationTimer_, firstTranslation_.y, translationPoint.y);
		moveTranslation_.z = easeInOut(translationTimer_, firstTranslation_.z, translationPoint.z);

		if (translationTimer_ >= 1.0f) {
			isCheckTranslation_ = false;
			translationTimer_ = 0.0f;
			translationEnd_ = true;
			return true;
		}
		return false;
	} else {
		return true;
	}
}

bool RailCamera::RotateTranslationAmount(Vector3 rotatePoint, Vector3 translationPoint, float Time) {
	if (!rotateTrasnlationEnd_) {
		if (!isCheckRotateTranslation_) {
			firstTranslation_ = moveTranslation_;
			firstRotate_ = worldTransform_.rotation_;
			isCheckRotateTranslation_ = true;
		}
		Vector3 rotation;
		rotation.x = rotatePoint.x * (float(M_PI) / 180.0f);
		rotation.y = rotatePoint.y * (float(M_PI) / 180.0f);
		rotation.z = rotatePoint.z * (float(M_PI) / 180.0f);

		rotateTranslationTimer_ += 1.0f / 60 / Time;

		rotation_.x = easeInOut(rotateTranslationTimer_, firstRotate_.x, firstRotate_.x + rotation.x);
		rotation_.y = easeInOut(rotateTranslationTimer_, firstRotate_.y, firstRotate_.y + rotation.y);
		rotation_.z = easeInOut(rotateTranslationTimer_, firstRotate_.z, firstRotate_.z + rotation.z);

		moveTranslation_.x = easeInOut(rotateTranslationTimer_, firstTranslation_.x, translationPoint.x);
		moveTranslation_.y = easeInOut(rotateTranslationTimer_, firstTranslation_.y, translationPoint.y);
		moveTranslation_.z = easeInOut(rotateTranslationTimer_, firstTranslation_.z, translationPoint.z);

		if (rotateTranslationTimer_ >= 1.0f) {
			isCheckRotateTranslation_ = false;
			rotateTranslationTimer_ = 0.0f;
			rotateTrasnlationEnd_ = true;
			return true;
		}
		return false;
	} else {
		return true;
	}
}

Vector3 RailCamera::MoveAmountVoid(Vector3 movePoint, float moveTime) {
	if (!isCheckTranslate_) {
		firstPos_ = worldTransform_.translation_;
		isCheckTranslate_ = true;
	}
	Vector3 result;

	moveTimer_ += 1.0f / 60 / moveTime;
	result.x = easeInOut(moveTimer_, firstPos_.x, firstPos_.x + movePoint.x);
	result.y = easeInOut(moveTimer_, firstPos_.y, firstPos_.y + movePoint.y);
	result.z = easeInOut(moveTimer_, firstPos_.z, firstPos_.z + movePoint.z);

	if (moveTimer_ >= 1.0f) {
		isCheckTranslate_ = false;
		moveTimer_ = 0.0f;
	}
	return result;
}

Vector3 RailCamera::RotateAmountVoid(Vector3 rotatePoint, float rotateTime) {
	if (!isCheckRotate_) {
		firstRotate_ = worldTransform_.rotation_;
		isCheckRotate_ = true;
	}
	Vector3 rotation;
	rotation.x = rotatePoint.x * (float(M_PI) / 180.0f);
	rotation.y = rotatePoint.y * (float(M_PI) / 180.0f);
	rotation.z = rotatePoint.z * (float(M_PI) / 180.0f);
	Vector3 result;

	rotateTimer_ += 1.0f / 60 / rotateTime;
	result.x = easeInOut(rotateTimer_, firstRotate_.x, firstRotate_.x + rotation.x);
	result.y = easeInOut(rotateTimer_, firstRotate_.y, firstRotate_.y + rotation.y);
	result.z = easeInOut(rotateTimer_, firstRotate_.z, firstRotate_.z + rotation.z);

	if (rotateTimer_ >= 1.0f) {
		isCheckRotate_ = false;
		rotateTimer_ = 0.0f;
	}
	return result;
}

Vector3 RailCamera::TranslationAmountVoid(Vector3 translationPoint, float translatoinTime) {
	if (!isCheckTranslation_) {
		firstTranslation_ = moveTranslation_;
		isCheckTranslation_ = true;
	}
	Vector3 result;

	translationTimer_ += 1.0f / 60 / translatoinTime;
	result.x = easeInOut(translationTimer_, firstTranslation_.x, translationPoint.x);
	result.y = easeInOut(translationTimer_, firstTranslation_.y, translationPoint.y);
	result.z = easeInOut(translationTimer_, firstTranslation_.z, translationPoint.z);

	if (translationTimer_ >= 1.0f) {
		isCheckTranslation_ = false;
		translationTimer_ = 0.0f;
	}
	return result;
}

void RailCamera::ResetAllAmount() {
	ResetTranslate();
	ResetRotate();
	ResetTranslation();
	ResetRotateTranslation();
}

void RailCamera::Move() { 
	if (!finish_) {
		if (moveStage_ == 0) {
			if (translation_.z >= 1770.2f && RotateTranslationAmount(Vector3{0.0f, -90.0f, 0.0f}, Vector3{-1.0f, 0.0f, 0.0f}, 1.0f)) {
				moveStage_ = 1;
				ResetAllAmount();
			}
		} else if (moveStage_ == 1) {
			if (translation_.x <= -1770.2f && RotateTranslationAmount(Vector3{0.0f, -90.0f, 0.0f}, Vector3{0.0f, 0.0f, -1.0f}, 1.0f)) {
				moveStage_ = 2;
				ResetAllAmount();
			}
		} else if (moveStage_ == 2) {
			if (translation_.z <= 29.8f && RotateTranslationAmount(Vector3{0.0f, -90.0f, 0.0f}, Vector3{1.0f, 0.0f, 0.0f}, 1.0f)) {
				moveStage_ = 3;
				ResetAllAmount();
			}
		} else if (moveStage_ == 3) {
			if (translation_.x >= -29.8f && RotateTranslationAmount(Vector3{0.0f, -90.0f, 0.0f}, Vector3{0.0f, 0.0f, 1.0f}, 1.0f)) {
				moveStage_ = 0;
				ResetAllAmount();
			}
		}
	}
		translation_ += moveTranslation_;
}