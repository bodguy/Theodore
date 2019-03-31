#include "TagManager.h"

namespace Theodore {
  TagManager* TagManager::instance = nullptr;
  TagManager::TagManager() {
    instance = this;
    mTags.clear();
    mLayers.clear();
  }

  TagManager::~TagManager() {

  }

  void TagManager::AddTag(uint32_t tag) {

  }

  void TagManager::RemoveTag(uint32_t tag) {

  }

  void TagManager::AddLayer(uint32_t layer) {

  }

  void TagManager::RemoveLayer(uint32_t layer) {

  }
}