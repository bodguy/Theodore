/**
  @brief datastructure for skinning animation
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef BoneWeight_h
#define BoneWeight_h

namespace Theodore {
  class BoneWeight {
  public:
    BoneWeight();
    ~BoneWeight();

    bool operator==(const BoneWeight& other) const;
    bool operator!=(const BoneWeight& other) const;

    int boneIndex0, boneIndex1, boneIndex2, boneIndex3;
    float weight0, weight1, weight2, weight3;
  };
}  // namespace Theodore

#endif /* BoneWeight_h */