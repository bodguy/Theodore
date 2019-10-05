#include "ShadowInfo.h"
#include "../../../Asset/AssetManager.h"
#include "../../../Asset/Shader.h"
#include "../../../Asset/Texture2D.h"
#include "../../../Graphics/FrameBuffer.h"
#include "../../../Helper/Utility.h"
#include "../../../Platform/Platform.h"

namespace Theodore {
  ShadowInfo::ShadowInfo() {
    /*
    frame = new FrameBuffer(Platform::GetWidth(), Platform::GetHeight());
    Texture2D* rawTex = AssetManager::RequestTexture("DepthMap", Platform::GetWidth(),
    Platform::GetHeight(), TextureFormat::Depth, nullptr);
    rawTex->SetFilter(FilterMode::Nearest);
    rawTex->SetWrapMode(WrapMode::Repeat);
    frame->AttachTexture(rawTex, Attachment::Depth);
    frame->Create(false);

    shadowProgram = Shader::Find("Shadow");
    */
  }

  ShadowInfo::~ShadowInfo() {
    // SafeDealloc(frame);
  }
} // namespace Theodore