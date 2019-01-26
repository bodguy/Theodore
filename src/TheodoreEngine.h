/**
  @brief all in one heder file
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef TheodoreEngine_h
#define TheodoreEngine_h

#include "./Math/Color.h"
#include "./Math/Math.h"
#include "./Math/Matrix4x4.h"
#include "./Math/Quaternion.h"
#include "./Math/Vector2d.h"
#include "./Math/Vector3d.h"
#include "./Math/Vector4d.h"

#include "./Asset/MSAATexture2D.h"
#include "./Asset/Shader.h"
#include "./Asset/Texture.h"
#include "./Asset/Texture2D.h"
#include "./Geometry/Plane.h"
#include "./Geometry/Ray.h"
#include "./Graphics/FrameBuffer.h"
#include "./Graphics/Graphics.h"
#include "./Graphics/RenderBuffer.h"
#include "./Graphics/VertexBuffer.h"
#include "./Object/Component/Camera.h"
#include "./Object/Component/Light.h"
#include "./Object/Component/sub/Material.h"

#include "./Asset/AssetManager.h"
#include "./Graphics/Enumeration.h"
#include "./Helper/Debug.h"
#include "./Helper/File.h"
#include "./Helper/StringUtil.h"
#include "./Helper/Utility.h"
#include "./Math/Random.h"
#include "./Platform/Input.h"
#include "./Platform/Platform.h"
#include "./Platform/Time.h"

#include "./Asset/WaveFrontObjMesh.h"
#include "./Geometry/Bounds.h"
#include "./Object/Application.h"
#include "./Object/Component/Transform.h"
#include "./Object/Component/Collider/BoxCollider.h"
#include "./Object/Component/Collider/SphereCollider.h"
#include "./Object/Component/Component.h"
#include "./Object/Component/CubemapRenderer.h"
#include "./Object/Component/Gizmo.h"
#include "./Object/Component/MeshRenderer.h"
#include "./Object/Component/Physics/RigidBody2D.h"
#include "./Object/Component/SkinnedMeshRenderer.h"
#include "./Object/Component/SpriteRenderer.h"
#include "./Object/Component/sub/BoneWeight.h"
#include "./Object/Component/sub/KeyFrame.h"
#include "./Object/Component/sub/Mesh.h"
#include "./Object/Component/sub/ShapeGenerator.h"
#include "./Object/Component/sub/Sprite.h"
#include "./Object/GameObject.h"
#include "./Object/Scene.h"
#include "./Object/SceneManager.h"

using namespace Theodore;

#endif /* TheodoreEngine_h */
