#ifndef Object_h
#define Object_h

#include <atomic>
#include <string>

namespace Theodore {
	class Unique_id {
	public:
		Unique_id() :mInstanceId(++type_id) {}

	private:
		static std::atomic<uint32_t> type_id;

	protected:
		uint32_t mInstanceId;
	};

	class Vector3d;
	class Object : public Unique_id {
		friend class GameObject;
	public:
		explicit Object(const std::string& name);
		virtual ~Object();

		// Delete copy constructor and assign operator.
		Object(const Object& rhs) = delete;
		Object& operator=(const Object& rhs) = delete;

		// Compare operator.
		bool operator==(const Object& rhs) const;
		bool operator!=(const Object& rhs) const;
		// Get object with name.
		const std::string& ToString() const;
		// Get object with hash value.
		const uint32_t GetHashCode() const;
		// Get object's unique id.
		uint32_t GetInstanceID() const;
		// Set name of object. 
		void SetName(const std::string& name);

		static bool Destroy(Object* obj);
		static Object* Instantiate(Object* original);
		static Object* Instantiate(Object* original, const Vector3d& position);
		static Object* Instantiate(Object* original, const Vector3d& position, const Vector3d& rotation);

	private:
		virtual void Update(double deltaTime) = 0;
		virtual void Render() = 0;
		virtual bool CompareEquality(const Object& rhs) const = 0;
		virtual bool Destroy() = 0;

	protected:
		uint32_t mHashValue;
		std::string mName;
	};
}

#endif /* Object_h */