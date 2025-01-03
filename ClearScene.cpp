#include "ClearScene.h"

using namespace KamataEngine;

void ClearScene::Initialize() { finished_ = false; }

void ClearScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void ClearScene::Draw() {}