#ifndef TagManager_h
#define TagManager_h

#include <vector>

namespace Theodore {
  class TagManager {
  public:
    TagManager();
    ~TagManager();

    static void AddTag(unsigned int tag);
    static void RemoveTag(unsigned int tag);
    static void AddLayer(unsigned int layer);
    static void RemoveLayer(unsigned int layer);

  private:
    std::vector<unsigned int> mTags;
    std::vector<unsigned int> mLayers;

    static TagManager* instance;
  };
}  // namespace Theodore

#endif
