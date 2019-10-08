// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Asset.h"

namespace Theodore {
  Asset::Asset() : mType(AssetType::Undefined), mRefCount(0), mBaseName(""), mFullName(""), mIsManaged(false) {}

  Asset::~Asset() {}

  void Asset::AddReference() { mRefCount++; }

  void Asset::RemoveReference() { mRefCount--; }

  std::string Asset::BaseName(const std::string& path) {
    const size_t last_slash_idx = path.find_last_of("\\/");
    if (std::string::npos != last_slash_idx) {
      return path.substr(last_slash_idx + 1);
    }

    return std::string("");
  }

  void Asset::SetAssetName(const std::string& assetName) {
    mBaseName = BaseName(assetName);
    mFullName = assetName;
  }

  bool Asset::IsManaged() { return mIsManaged; }
}  // namespace Theodore
