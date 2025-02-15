#pragma once
#include "input/Input.h"
#include "base/DirectXCommon.h"
#include "3d/Model.h"
#include "2d/Sprite.h"
#include "3d/WorldTransform.h"
#include "3d/Camera.h"
#include "audio/Audio.h"
#include "base/TextureManager.h"

class TitleScene {
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
	KamataEngine::Audio* audio_ = nullptr;

	uint32_t buttonSound_ = 0;

	KamataEngine::DirectXCommon* dxCommon_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;

	KamataEngine::Sprite* clearBackGround_ = nullptr;
	uint32_t textureHandle_ = 0;
	KamataEngine::Model* gameStartText_ = nullptr;
	KamataEngine::Model* pushSpaceText_ = nullptr;

	KamataEngine::Vector4 backGroundColor_;

	float timer = 0.0f;
	float time = 1.0f;

	bool canPush = false;
};
