#include "Ground.h"
#include <cassert>

using namespace KamataEngine;

void Ground::Initialize(Model* model) { 
	assert(model);
	model_ = model;
}

void Ground::Update() {

}

void Ground::Draw(WorldTransform& worldTransform, Camera& camera) { model_->Draw(worldTransform, camera); }
