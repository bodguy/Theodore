/**
  @brief top-level interface in entire scene objects
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Object_h
#define Object_h

#include <atomic>
#include <string>

namespace Theodore {
  class UniqueId {
    public:
    UniqueId() : mInstanceId(++type_id) {}

    private:
    static std::atomic<uint32_t> type_id;

    protected:
    uint32_t mInstanceId;
  };

  class Vector3d;
  class Quaternion;
  class GameObject;
  class Object : public UniqueId {
    friend class GameObject;

    public:
    explicit Object(const std::string& name);
    virtual ~Object();

    // Delete assign operator.
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

    static bool Destroy(GameObject* obj);
    static GameObject* Instantiate(GameObject* original);
    static GameObject* Instantiate(GameObject* original, const Vector3d& position);
    static GameObject* Instantiate(GameObject* original, const Vector3d& position, const Quaternion& rotation);

    private:
    virtual void FixedUpdate(float deltaTime){};
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual bool CompareEquality(const Object& rhs) const = 0;
    virtual bool Destroy() = 0;

    protected:
    Object(const Object& rhs); // copy constructor

    uint32_t mHashValue;
    std::string mName;
  };
} // namespace Theodore

#endif /* Object_h */
