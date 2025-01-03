#include "Object.h"
#include <cassert>

using namespace KamataEngine;

void Object::Initialize() { 
	caveModel_ = Model::CreateFromOBJ("tunnel");
	mountainModel_ = Model::CreateFromOBJ("mount1");
	//mountainModel_[1] = Model::CreateFromOBJ("mount/2");
}

void Object::Update() {}

void Object::Draw(WorldTransform& worldTransform, Camera& camera) { 
	caveModel_->Draw(worldTransform, camera);
	/*for (int i = 0; i < 2; i++) {
		mountainModel_[i]->Draw(worldTransform, camera);
	}*/
	mountainModel_->Draw(worldTransform, camera);
}
