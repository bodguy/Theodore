#ifndef TagManager_h
#define TagManager_h

#include <vector>

namespace Theodore {
  class TagManager {
  public:
    TagManager();
    ~TagManager();

    static void AddTag(uint32_t tag);
    static void RemoveTag(uint32_t tag);
    static void AddLayer(uint32_t layer);
    static void RemoveLayer(uint32_t layer);

  private:
    std::vector<uint32_t> mTags;
    std::vector<uint32_t> mLayers;

    static TagManager* instance;
  };
}

#endif
