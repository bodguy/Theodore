// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef WaveFrontObjMesh_h
#define WaveFrontObjMesh_h

#include "Graphics/Enumeration.h"
#include "Object/Component/sub/Mesh.h"

namespace Theodore {
  class WaveFrontObjMesh : public Mesh {
  public:
    WaveFrontObjMesh();
    virtual ~WaveFrontObjMesh() override;

    bool LoadObj(const std::string& filepath);

  private:
    void ProcessVertex(FILE* file, const std::vector<Vector2d>& uvsIn, const std::vector<Vector3d>& normalsIn, States states);
  };
}  // namespace Theodore

#endif /* ObjMesh_h */