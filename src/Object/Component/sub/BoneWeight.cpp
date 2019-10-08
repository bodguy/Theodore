// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "BoneWeight.h"
#include "Math/Math.h"

namespace Theodore {
  BoneWeight::BoneWeight() {
    boneIndex0 = 0;
    boneIndex1 = 0;
    boneIndex2 = 0;
    boneIndex3 = 0;
    weight0 = 0.f;
    weight1 = 0.f;
    weight2 = 0.f;
    weight3 = 0.f;
  }

  BoneWeight::~BoneWeight() {}

  bool BoneWeight::operator==(const BoneWeight& other) const {
    return (boneIndex0 == other.boneIndex0 && boneIndex1 == other.boneIndex1 && boneIndex2 == other.boneIndex2 && boneIndex3 == other.boneIndex3 && Math::IsEqual(weight0, other.weight0) &&
            Math::IsEqual(weight1, other.weight1) && Math::IsEqual(weight2, other.weight2) && Math::IsEqual(weight3, other.weight3));
  }

  bool BoneWeight::operator!=(const BoneWeight& other) const { return !(*this == other); }
}  // namespace Theodore