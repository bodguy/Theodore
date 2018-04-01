#include "Asset.h"

namespace Quark {
    Asset::Asset() : mRefCount(0), mType(Enumeration::Undefined) {
    }
    
    Asset::~Asset() {
    }
    
    void Asset::AddReference() {
        mRefCount++;
    }
    
    void Asset::RemoveReference() {
        mRefCount--;
    }
}
