#include "Object.h"
#include "Vector3d.h"
#include "crc32.h"

namespace Theodore {
	std::atomic<uint32_t> Unique_id::type_id;
	Object::Object(const std::string& name) :mName(name), mHashValue(CRC32_STR(name.c_str())) {

	}

	Object::~Object() {

	}

	bool Object::operator==(const Object& rhs) const {
		return mHashValue == rhs.mHashValue  && this->CompareEquality(rhs);
	}

	bool Object::operator!=(const Object& rhs) const {
		return !(*this == rhs && this->CompareEquality(rhs));
	}

	const std::string& Object::ToString() const {
		return mName;
	}

	const uint32_t Object::GetHashCode() const {
		return mHashValue;
	}

	uint32_t Object::GetInstanceID() const {
		return mInstanceId;
	}

	void Object::SetName(const std::string& name) {
		mName = name;
		mHashValue = CRC32_STR(name.c_str());
	}

	bool Object::Destroy(Object* obj) {
		if (!obj)
			return false;

		return obj->Destroy();
	}

	Object* Object::Instantiate(Object* original) {
		// incomplete
		return nullptr;
	}

	Object* Object::Instantiate(Object* original, const Vector3d& position) {
		// incomplete
		return nullptr;
	}

	Object* Object::Instantiate(Object* original, const Vector3d& position, const Vector3d& rotation) {
		// incomplete
		return nullptr;
	}
}