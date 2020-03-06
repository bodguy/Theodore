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

    std::string GetAssetName() const;
    void SetAssetName(const std::string& assetName);
    bool IsManaged();

  private:
    void AddReference();
    void RemoveReference();
    static std::string BaseName(const std::string& path);

  protected:
    AssetType assetType;
    unsigned int referenceCount;
    std::string baseName;
    std::string fullName;
    bool isManaged;
  };
}  // namespace Theodore

#endif /* Asset_h */
