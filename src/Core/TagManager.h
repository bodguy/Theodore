// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

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
    std::vector<unsigned int> tags;
    std::vector<unsigned int> layers;

    static TagManager* instance;
  };
}  // namespace Theodore

#endif
