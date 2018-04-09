#ifndef Enumeration_h
#define Enumeration_h

#include "os_types.h"
#include GLEW_INCLUDE_DIR

namespace Quark {
    namespace Enumeration {
        enum Primitive {
            Triangles = GL_TRIANGLES,
            Lines = GL_LINES,
            Points = GL_POINTS,
            TriangleStrip = GL_TRIANGLE_STRIP,
            LineStrip = GL_LINE_STRIP,
            TriangleFan = GL_TRIANGLE_FAN,
            LineLoop = GL_LINE_LOOP,
			Patches = GL_PATCHES
        };
        
        enum BufferBits {
            ColorBits = GL_COLOR_BUFFER_BIT,
            DepthBits = GL_DEPTH_BUFFER_BIT,
            StencilBits = GL_STENCIL_BUFFER_BIT
        };
        
        enum FillMode {
            Point = GL_POINT,
            WireFrame = GL_LINE,
            Solid = GL_FILL
        };
        
        enum CompareMode {
            Always = GL_ALWAYS,
            Equal = GL_EQUAL,
            NotEqual = GL_NOTEQUAL,
            Less = GL_LESS,
            LessEqaul = GL_LEQUAL,
            Greater = GL_GREATER,
            GreaterEqual = GL_GEQUAL
        };
        
        enum CullMode {
            CounterClockWise = GL_CCW,
            ClockWise = GL_CW
        };
        
        enum BufferUsage {
            StreamDraw = GL_STREAM_DRAW,
            StreamRead = GL_STREAM_READ,
            StreamCopy = GL_STREAM_COPY,
            StaticDraw = GL_STATIC_DRAW,
            StaticRead = GL_STATIC_READ,
            StaticCopy = GL_STATIC_COPY,
            DynamicDraw = GL_DYNAMIC_DRAW,
            DynamicRead = GL_DYNAMIC_READ,
            DynamicCopy = GL_DYNAMIC_COPY
        };
        
        enum BufferType {
            BufferVertex = GL_ARRAY_BUFFER,
            BufferIndex = GL_ELEMENT_ARRAY_BUFFER,
            BufferUniform = GL_UNIFORM_BUFFER
        };
        
        enum FilterMode {
            Nearest = GL_NEAREST, // texture pixels become blocky up close. it is equal to GL_NEAREST.
            Bilinear = GL_LINEAR, // texture samples are averaged. it is eqaul to GL_LINEAR.
            Trilinear = GL_LINEAR_MIPMAP_LINEAR // texture samples are averaged and also blended between mipmap levels. it is equal to GL_LINEAR_MIPMAP_LINEAR.
        };
        
        enum WrapMode {
            Repeat = GL_REPEAT, // Tiles the texture, creating a repeating pattern.
            Clamp = GL_CLAMP, // Clamps the texture to the last pixel at the border.
            ClampEdge = GL_CLAMP_TO_EDGE
        };
        
        enum TextureDimension {
            None = -1,
            Tex2D = GL_TEXTURE_2D,
            Tex3D = GL_TEXTURE_3D,
            MSAATex2D = GL_TEXTURE_2D_MULTISAMPLE,
            Cube = GL_TEXTURE_CUBE_MAP
        };
        
        enum TextureFormat {
            RGB24 = 3,
            RGBA32 = 4,
            Red8,
            Green8,
            Blue8
        };
        
        enum AssetType {
            Undefined = -1,
            TextureType,
            AudioType,
            FontType
        };
        
        enum OpenMode {
            Read = 0,  // Read only
            Write, // Clear file contents
            ReadWrite,
            Append, // Write to end of file
            ReadBinary,
            WriteBinary,
            ReadWriteBinary
        };
        
        enum LightType {
            Unknown = -1,
            DirectionalLight,
            PointLight,
            SpotLight
        };
        
		enum ShaderType {
			VertexShader = GL_VERTEX_SHADER,
			FragmentShader = GL_FRAGMENT_SHADER,
			GeometryShader = GL_GEOMETRY_SHADER,
			TessControlShader = GL_TESS_CONTROL_SHADER,
			TessEvalShader = GL_TESS_EVALUATION_SHADER,
			ComputeShader = GL_COMPUTE_SHADER
		};
        
        enum VertexSemantic {
            SemanticPosition = 0,
            SemanticColor,
            SemanticTexCoord,
            SemanticNormal,
            SemanticBiNormal,
            SemanticTangent
        };
        
        enum TextureUnit {
            DiffuseMap,
            NormalMap,
            SpecularMap,
            ShadowMap,
            EnvironmentMap
        };
        
        enum CubemapFace {
            PositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            NegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            PositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            NegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            PositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            NegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
        };
        
        enum LockMode {
            LockRead = GL_READ_ONLY,
            LockWrite = GL_WRITE_ONLY,
            LockReadWrite = GL_READ_WRITE
        };
        
        enum Capabilities {
            DepthTest = GL_DEPTH_TEST,
            StencilTest = GL_STENCIL_TEST,
            Blending = GL_BLEND,
            AlphaTest = GL_ALPHA_TEST,
            MSAA = GL_MULTISAMPLE,
            Culling = GL_CULL_FACE,
            CubemapSeamless = GL_TEXTURE_CUBE_MAP_SEAMLESS // available only if the GL version is 3.2 or greater.
        };
        
        enum WindowStyle {
            Resizable,
            NonResizeable
        };

		enum PatchParameters {
			PatchVertices = GL_PATCH_VERTICES,
			DefaultOuterLevel = GL_PATCH_DEFAULT_OUTER_LEVEL,
			DefaultInnerLevel = GL_PATCH_DEFAULT_INNER_LEVEL
		};
    }
    
    inline Enumeration::BufferBits operator|(Enumeration::BufferBits lft, Enumeration::BufferBits rht) {
        return (Enumeration::BufferBits)((int)lft | (int)rht);
    }
}

#endif /* Enumeration_h */
