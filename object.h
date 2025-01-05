#pragma once
#include "3d/Camera.h"
#include "3d/Model.h"
#include "3d/WorldTransform.h"
#include "base/TextureManager.h"
#include "2d/Sprite.h"

class RailCamera;

class Object {
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
	void Draw(KamataEngine::WorldTransform& worldTransform, KamataEngine::Camera& camera);
	/// <summary>
	/// スプライトの描画
	/// </summary>
	void DrawSprite();

	void SetRailCamera(RailCamera* railcamera) { railCamera_ = railcamera; }

private:
	KamataEngine::Model* caveModel_;
	KamataEngine::Model* mountainModel_;
	
	KamataEngine::Sprite* clearSprite_;
	uint32_t clearSpriteTextureHandle_ = 0;
	KamataEngine::Vector4 clearSpriteTextureColor_;

	RailCamera* railCamera_ = nullptr;
};
