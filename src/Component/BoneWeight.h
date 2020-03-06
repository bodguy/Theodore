// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef BoneWeight_h
#define BoneWeight_h

namespace Theodore {
  class BoneWeight {
  public:
    BoneWeight();
    ~BoneWeight();

    bool operator==(const BoneWeight& other) const;
    bool operator!=(const BoneWeight& other) const;

    int boneIndex0;
    int boneIndex1;
		int boneIndex2;
		int boneIndex3;
    float weight0;
		float weight1;
		float weight2;
		float weight3;
  };
}  // namespace Theodore

#endif /* BoneWeight_h */