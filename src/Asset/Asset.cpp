// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Asset.h"
#include "Helper/StringUtil.h"

namespace Theodore {
  Asset::Asset() : assetType(AssetType::Undefined), referenceCount(0), fileName(""), filePath(""), isManaged(false) {}

  Asset::~Asset() {}

  void Asset::AddReference() { referenceCount++; }

  void Asset::RemoveReference() { referenceCount--; }

	std::string Asset::GetFilePath() const {
		return filePath;
  }

  void Asset::SetAssetName(const std::string& name) {
		fileName = StringUtil::Base(name);
		filePath = name;
  }

  bool Asset::IsManaged() { return isManaged; }
}  // namespace Theodore
