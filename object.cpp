#include "Object.h"
#include <cassert>

using namespace KamataEngine;

void Object::Initialize() {
	
}

void Object::Update() {}

void Object::Draw(WorldTransform& worldTransform, Camera& camera) { model_->Draw(worldTransform, camera); }
