// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "TagManager.h"

namespace Theodore {
  TagManager* TagManager::instance = nullptr;
  TagManager::TagManager() {
    instance = this;
    mTags.clear();
    mLayers.clear();
  }

  TagManager::~TagManager() {}

  void TagManager::AddTag(unsigned int tag) {}

  void TagManager::RemoveTag(unsigned int tag) {}

  void TagManager::AddLayer(unsigned int layer) {}

  void TagManager::RemoveLayer(unsigned int layer) {}
}  // namespace Theodore