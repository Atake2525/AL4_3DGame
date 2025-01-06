#include "GameOver.h"
#include "imgui.h"
#include "kMath.h"
#define _USE_MATH_DEFINE
#include "math.h"

using namespace KamataEngine;

void GameOver::Initialize() {
	finished_ = false;
	dxCommon_ = KamataEngine::DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	buttonSound_ = audio_->LoadWave("button.wav");

	textureHandle_ = TextureManager::Load("clearSprite.png");
	backGroundColor_ = {0.0f, 0.0f, 0.0f, 1.0f};
	clearBackGround_ = Sprite::Create(textureHandle_, Vector2{0.0f, 0.0f}, backGroundColor_);
	gameOverText_= Model::CreateFromOBJ("gameOver");
	pushSpaceText_ = Model::CreateFromOBJ("pushSpace");

	worldTransform_.Initialize();
	worldTransform_.rotation_.y = 180.0f * (float(M_PI) / 180.0f);
	camera_.Initialize();
	canPush = false;
	timer = 0.0f;
}

void GameOver::Update() {
#ifdef _DEBUG
	ImGui::Begin("state");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 1.0f);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.1f);
	ImGui::End();
#endif // _DEBUG

	timer += 1.0f / 60;
	if (timer >= time) {
		canPush = true;
	}
	if (canPush) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			audio_->PlayWave(buttonSound_);
			audio_->StopWave(buttonSound_);
			finished_ = true;
		}
	}

	worldTransform_.UpdateMatirx();
	camera_.TransferMatrix();
}

void GameOver::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	clearBackGround_->Draw();

	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	KamataEngine::Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	gameOverText_->Draw(worldTransform_, camera_);
	if (canPush) {
		pushSpaceText_->Draw(worldTransform_, camera_);
	}

	// 3Dオブジェクト描画後処理
	KamataEngine::Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();

#pragma endregion
}