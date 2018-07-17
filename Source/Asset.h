/**
	@brief represents every resource type
	@author bodguy
	@date 17.07.17
	@todo 
	@bug 
*/

#ifndef Asset_h
#define Asset_h

#include "Enumeration.h"
#include <string>

namespace Theodore {
    class Asset {
        friend class AssetManager;
    public:
        Asset();
        virtual ~Asset();

		void SetAssetName(const std::string& filename);
		bool IsManaged();

    private:
        void AddReference();
        void RemoveReference();
        
    protected:
        AssetType mType;
        unsigned int mRefCount;
        std::string mName;
		std::string mFilePath;
		bool mIsManaged;
    };
}

#endif /* Asset_h */
