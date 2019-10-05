/**
  @brief wavefront obj file parser
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef WaveFrontObjMesh_h
#define WaveFrontObjMesh_h

#include "../Graphics/Enumeration.h"
#include "../Object/Component/sub/Mesh.h"

namespace Theodore {
  class WaveFrontObjMesh : public Mesh {
  public:
    WaveFrontObjMesh();
    virtual ~WaveFrontObjMesh();

    bool LoadObj(const std::string& filepath);

  private:
    void ProcessVertex(FILE* file, const std::vector<Vector2d>& uvsIn, const std::vector<Vector3d>& normalsIn, States states);
  };
}  // namespace Theodore

#endif /* ObjMesh_h */