// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Asset.h"

namespace Theodore {
  Asset::Asset() : assetType(AssetType::Undefined), referenceCount(0), baseName(""), fullName(""), isManaged(false) {}

  Asset::~Asset() {}

  void Asset::AddReference() { referenceCount++; }

  void Asset::RemoveReference() { referenceCount--; }

  std::string Asset::BaseName(const std::string& path) {
    const size_t last_slash_idx = path.find_last_of("\\/");
    if (std::string::npos != last_slash_idx) {
      return path.substr(last_slash_idx + 1);
    }

    return std::string("");
  }

	std::string Asset::GetAssetName() const {
		return fullName;
  }

  void Asset::SetAssetName(const std::string& assetName) {
		baseName = BaseName(assetName);
		fullName = assetName;
  }

  bool Asset::IsManaged() { return isManaged; }
}  // namespace Theodore
