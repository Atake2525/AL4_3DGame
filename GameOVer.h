#pragma once
#include "2d/Sprite.h"
#include "3d/Camera.h"
#include "3d/Model.h"
#include "3d/WorldTransform.h"
#include "base/DirectXCommon.h"
#include "base/TextureManager.h"
#include "input/Input.h"
#include "audio/Audio.h"

class GameOver {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:
	// 終了フラグ
	bool finished_ = false;

	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;
	uint32_t buttonSound_ = 0;

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;

	KamataEngine::Sprite* clearBackGround_ = nullptr;
	uint32_t textureHandle_ = 0;
	KamataEngine::Model* gameOverText_ = nullptr;
	KamataEngine::Model* pushSpaceText_ = nullptr;

	KamataEngine::Vector4 backGroundColor_;

	float timer = 0.0f;
	float time = 1.0f;

	bool canPush = false;
};
