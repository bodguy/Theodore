#ifndef BoneWeight_h
#define BoneWeight_h

namespace Quark {
	class BoneWeight {
	public:
		BoneWeight();
		~BoneWeight();

		bool operator == (const BoneWeight& other) const;
		bool operator != (const BoneWeight& other) const;

		int boneIndex0, boneIndex1, boneIndex2, boneIndex3;
		float weight0, weight1, weight2, weight3;
	};
}

#endif /* BoneWeight_h */