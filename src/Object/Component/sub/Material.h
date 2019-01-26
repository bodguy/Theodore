/**
  @brief mesh material
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Material_h
#define Material_h

#include "../../../Math/Color.h"
#include <vector>

namespace Theodore {
  class Pipeline;
  class Texture;
  class FrameBuffer;
  class Material {
  public:
    Material(Pipeline* program);
    Material(Color ambient, Color diffse, Color specular, float shininess);
    ~Material();

    Pipeline* shader;
    Texture* texture0;
    Texture* texture1;
    Texture* texture2;
    Texture* texture3;
    Texture* texture4;
    FrameBuffer* renderTexture;

    Color ambient;
    Color diffuse;
    Color specular;
    float shininess;

    static const Material emerald;
    static const Material jade;
    static const Material obsidian;
    static const Material pearl;
    static const Material ruby;
    static const Material turquoise;
    static const Material brass;
    static const Material bronze;
    static const Material chrome;
    static const Material copper;
    static const Material gold;
    static const Material silver;
    static const Material black_plastic;
    static const Material black_rubber;
  };
}

#endif /* Material_h */