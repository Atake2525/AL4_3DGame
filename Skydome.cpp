#include "Skydome.h"
#include "cassert"

using namespace KamataEngine;

void Skydome::Initialize(Model* model) { 
	assert(model);
	model_ = model;
}

void Skydome::Update() {

}

void Skydome::Draw(WorldTransform& worldTransform, Camera& camera, ObjectColor* objectColor) { 
	model_->Draw(worldTransform, camera, objectColor);

}