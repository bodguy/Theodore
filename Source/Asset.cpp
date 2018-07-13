#include "Asset.h"

namespace Theodore {
    Asset::Asset() : mRefCount(0), mType(AssetType::Undefined), mIsManaged(false) {
    }
    
    Asset::~Asset() {
    }
    
    void Asset::AddReference() {
        mRefCount++;
    }
    
    void Asset::RemoveReference() {
        mRefCount--;
    }

	void Asset::SetAssetName(const std::string& filename) {
		size_t lastPos = filename.find_last_of('/') + 1;
		mName = filename.substr(lastPos, filename.length());
		mFilePath = filename.substr(0, lastPos);
	}

	bool Asset::IsManaged() {
		return mIsManaged;
	}
}
