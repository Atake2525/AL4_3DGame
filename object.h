#pragma once
#include "3d/Camera.h"
#include "3d/Model.h"
#include "3d/WorldTransform.h"
#include "base/TextureManager.h"

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

private:
	KamataEngine::Model* caveModel_;
	KamataEngine::Model* mountainModel_;

};
