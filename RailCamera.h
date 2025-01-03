#pragma once
#include "3d/WorldTransform.h"
#include "3d/Camera.h"
#include "math/MathUtility.h"
#include "kMath.h"

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const KamataEngine::WorldTransform& worldTransform);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	KamataEngine::Camera& GetCamera() { return camera; }

	const KamataEngine::WorldTransform& GetWorldTransform() { return worldTransform_; }

	bool MoveAmount(KamataEngine::Vector3 movePoint, float moveTime);

	bool RotateAmount(KamataEngine::Vector3 rotatePoint, float rotateTime);

	bool TranslationAmount(KamataEngine::Vector3 translationPoint, float translationTime);

	bool RotateTranslationAmount(KamataEngine::Vector3 rotatePoint, KamataEngine::Vector3 translationPoint, float Time);

	KamataEngine::Vector3 MoveAmountVoid(KamataEngine::Vector3 movePoint, float moveTime);

	KamataEngine::Vector3 RotateAmountVoid(KamataEngine::Vector3 rotatePoint, float rotateTime);

	KamataEngine::Vector3 TranslationAmountVoid(KamataEngine::Vector3 translationPoint, float translationTime);

	void Move();

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera camera;

	KamataEngine::Vector3 translation_ = {0.0f, 0.0f, 0.0f};

	KamataEngine::Vector3 moveTranslation_ = {0.0f, 0.0f, 1.0f};

	KamataEngine::Vector3 rotation_ = {0.0f, 0.0f, 0.0f};

	void ResetTranslate() { translateEnd_ = false; }
	void ResetRotate() { rotateEnd_ = false; }
	void ResetTranslation() { translationEnd_ = false; }
	void ResetRotateTranslation() { rotateTrasnlationEnd_ = false; }
	void ResetAllAmount();

	// カメラ移動
	// 平行移動, 回転に必要な宣言
	float moveTimer_ = 0.0f;
	float rotateTimer_ = 0.0f;
	float translationTimer_ = 0.0f;
	float rotateTranslationTimer_ = 0.0f;

	bool isCheckTranslate_ = false;
	bool isCheckRotate_ = false;
	bool isCheckTranslation_ = false;
	bool isCheckRotateTranslation_ = false;

	bool translateEnd_ = false;
	bool rotateEnd_ = false;
	bool translationEnd_ = false;
	bool rotateTrasnlationEnd_ = false;

	KamataEngine::Vector3 firstPos_ = {0.0f, 0.0f, 0.0f};
	KamataEngine::Vector3 firstRotate_ = {0.0f, 0.0f, 0.0f};
	KamataEngine::Vector3 firstTranslation_ = {0.0f, 0.0f, 0.0f};

	int moveStage_ = 0;
};
