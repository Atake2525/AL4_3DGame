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

	void Move();

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera camera;

	KamataEngine::Vector3 translation_ = {0.0f, 0.0f, 0.0f};

	KamataEngine::Vector3 rotation_ = {0.0f, 0.0f, 0.0f};

	// カメラ移動
	// 平行移動, 回転に必要な宣言
	float moveTimer_ = 0.0f;
	float rotateTimer_ = 0.0f;

	bool isCheckTranslate_ = false;
	bool isCheckRotate_ = false;

	KamataEngine::Vector3 firstPos_ = {0.0f, 0.0f, 0.0f};
	KamataEngine::Vector3 firstRotate_ = {0.0f, 0.0f, 0.0f};

	int moveStage = 0;
};
