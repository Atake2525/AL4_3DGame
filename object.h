#pragma once
#include "3d/Camera.h"
#include "3d/Model.h"
#include "3d/WorldTransform.h"
#include "base/TextureManager.h"

class Object {
public:
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize();
	/// <summary>
	/// XV
	/// </summary>
	void Update();
	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw(KamataEngine::WorldTransform& worldTransform, KamataEngine::Camera& camera);

private:
	KamataEngine::Model* model_;
};
