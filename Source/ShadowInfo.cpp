#include "ShadowInfo.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Platform.h"
#include "AssetManager.h"
#include "Texture2D.h"
#include "Utility.h"

namespace Theodore {
	ShadowInfo::ShadowInfo() {
		/*
		frame = new FrameBuffer(Platform::GetWidth(), Platform::GetHeight());
		Texture2D* rawTex = AssetManager::RequestTexture("DepthMap", Platform::GetWidth(), Platform::GetHeight(), TextureFormat::Depth, nullptr);
		rawTex->SetFilter(FilterMode::Nearest);
		rawTex->SetWrapMode(WrapMode::Repeat);
		frame->AttachTexture(rawTex, Attachment::Depth);
		frame->Create(false);

		shadowProgram = Shader::Find("Shadow");
		*/
	}

	ShadowInfo::~ShadowInfo() {
		//SafeDealloc(frame);
	}
}