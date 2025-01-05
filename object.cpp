#include "Object.h"
#include "RailCamera.h"
#include "kMath.h"
#include <cassert>

using namespace KamataEngine;

void Object::Initialize() { 
	caveModel_ = Model::CreateFromOBJ("tunnel");
	mountainModel_ = Model::CreateFromOBJ("mount1");
	clearSpriteTextureHandle_ = TextureManager::Load("clearSprite.png");
	clearSpriteTextureColor_ = {0.0f, 0.0f, 0.0f, 0.0f};
	clearSprite_ = Sprite::Create(clearSpriteTextureHandle_, Vector2{0.0f, 0.0f}, clearSpriteTextureColor_);
	//mountainModel_[1] = Model::CreateFromOBJ("mount/2");
}

void Object::Update() {
	if (railCamera_->IsComplete()) {
		float timer = railCamera_->GetFinishTimer();
		clearSpriteTextureColor_.w = easeInOut(timer, 0.0f, 1.0f);
	}
	clearSprite_->SetColor(clearSpriteTextureColor_);
}

void Object::Draw(WorldTransform& worldTransform, Camera& camera) { 
	caveModel_->Draw(worldTransform, camera);
	/*for (int i = 0; i < 2; i++) {
		mountainModel_[i]->Draw(worldTransform, camera);
	}*/
	mountainModel_->Draw(worldTransform, camera);
}

void Object::DrawSprite() { clearSprite_->Draw(); }
