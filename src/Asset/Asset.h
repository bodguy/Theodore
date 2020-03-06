// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Asset_h
#define Asset_h

#include <string>
#include "Graphics/Enumeration.h"

namespace Theodore {
  class Asset {
    friend class AssetManager;

  public:
    Asset();
    virtual ~Asset();

    std::string GetFilePath() const;
    void SetAssetName(const std::string& name);
    bool IsManaged();

  private:
    void AddReference();
    void RemoveReference();

  protected:
    AssetType assetType;
    unsigned int referenceCount;
    std::string fileName;
    std::string filePath;
    bool isManaged;
  };
}  // namespace Theodore

#endif /* Asset_h */
