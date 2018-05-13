#ifndef Enumeration_h
#define Enumeration_h

#include "os_types.h"
#include GLEW_INCLUDE_DIR

namespace Quark {
    enum class Primitive {
        Triangles = GL_TRIANGLES,
        Lines = GL_LINES,
        Points = GL_POINTS,
        TriangleStrip = GL_TRIANGLE_STRIP,
        LineStrip = GL_LINE_STRIP,
        TriangleFan = GL_TRIANGLE_FAN,
        LineLoop = GL_LINE_LOOP,
		Patches = GL_PATCHES
    };
        
    enum class BufferBits {
        ColorBits = GL_COLOR_BUFFER_BIT,
        DepthBits = GL_DEPTH_BUFFER_BIT,
        StencilBits = GL_STENCIL_BUFFER_BIT
    };
        
    enum class FillMode {
        Point = GL_POINT,
        WireFrame = GL_LINE,
        Solid = GL_FILL
    };
        
    enum class CompareMode {
        Always = GL_ALWAYS,
        Equal = GL_EQUAL,
        NotEqual = GL_NOTEQUAL,
        Less = GL_LESS,
        LessEqaul = GL_LEQUAL,
        Greater = GL_GREATER,
        GreaterEqual = GL_GEQUAL
    };
        
    enum class CullMode {
        CounterClockWise = GL_CCW,
        ClockWise = GL_CW
    };
        
    enum class BufferUsage {
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
        
    enum class BufferType {
        BufferVertex = GL_ARRAY_BUFFER,
        BufferIndex = GL_ELEMENT_ARRAY_BUFFER,
        BufferUniform = GL_UNIFORM_BUFFER,
		BufferStorage = GL_SHADER_STORAGE_BUFFER
    };
        
    enum class FilterMode {
        Nearest = GL_NEAREST, // texture pixels become blocky up close. it is equal to GL_NEAREST.
        Bilinear = GL_LINEAR, // texture samples are averaged. it is eqaul to GL_LINEAR.
        Trilinear = GL_LINEAR_MIPMAP_LINEAR // texture samples are averaged and also blended between mipmap levels. it is equal to GL_LINEAR_MIPMAP_LINEAR.
    };
        
    enum class WrapMode {
        Repeat = GL_REPEAT, // Tiles the texture, creating a repeating pattern.
        Clamp = GL_CLAMP, // Clamps the texture to the last pixel at the border.
        ClampEdge = GL_CLAMP_TO_EDGE
    };
        
    enum class TextureDimension {
        None = -1,
        Tex2D = GL_TEXTURE_2D,
        Tex3D = GL_TEXTURE_3D,
        MSAATex2D = GL_TEXTURE_2D_MULTISAMPLE,
        CubeMap = GL_TEXTURE_CUBE_MAP
    };
        
    enum class TextureFormat {
        RGB24 = 3,
        RGBA32 = 4,
        Red8,
        Green8,
        Blue8
    };
        
    enum class AssetType {
        Undefined = -1,
        TextureType,
        AudioType,
        FontType,
		TextShaderType,
		BinaryShaderType,
		MeshType
    };
        
    enum class OpenMode {
        Read = 0,  // Read only
        Write, // Clear file contents
        ReadWrite,
        Append, // Write to end of file
        ReadBinary,
        WriteBinary,
        ReadWriteBinary
    };
        
    enum class LightType {
        Unknown = -1,
        DirectionalLight,
        PointLight,
        SpotLight
    };
        
	enum class ShaderType {
		VertexShader = GL_VERTEX_SHADER,
		FragmentShader = GL_FRAGMENT_SHADER,
		GeometryShader = GL_GEOMETRY_SHADER,
		TessControlShader = GL_TESS_CONTROL_SHADER,
		TessEvalShader = GL_TESS_EVALUATION_SHADER,
		ComputeShader = GL_COMPUTE_SHADER
	};
        
    enum class VertexSemantic {
		SemanticNone = 0,
        SemanticPosition = 1 << 0, // binary 0001
        SemanticColor = 1 << 1, // binary 0010
        SemanticTexCoord = 1 << 2, // binary 0100
        SemanticNormal = 1 << 3, // binary 1000
        SemanticBiNormal = 1 << 4, // binary 0001 0000
        SemanticTangent = 1 << 5 // binary 0010 0000
    };
        
    enum class TextureUnit {
        DiffuseMap,
        NormalMap,
        SpecularMap,
        ShadowMap,
        EnvironmentMap
    };
        
    enum class CubemapFace {
        PositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        NegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        PositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        NegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        PositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        NegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };
        
    enum class LockMode {
        LockRead = GL_READ_ONLY,
        LockWrite = GL_WRITE_ONLY,
        LockReadWrite = GL_READ_WRITE
    };
        
    enum class Capabilities {
        DepthTest = GL_DEPTH_TEST,
        StencilTest = GL_STENCIL_TEST,
        Blending = GL_BLEND,
        AlphaTest = GL_ALPHA_TEST,
        MSAA = GL_MULTISAMPLE,
        Culling = GL_CULL_FACE,
        CubemapSeamless = GL_TEXTURE_CUBE_MAP_SEAMLESS, // available only if the GL version is 3.2 or greater.
		PointSize = GL_PROGRAM_POINT_SIZE
    };
        
    enum class WindowStyle {
        Resizable,
        NonResizeable
    };

	enum class PatchParameters {
		PatchVertices = GL_PATCH_VERTICES,
		DefaultOuterLevel = GL_PATCH_DEFAULT_OUTER_LEVEL,
		DefaultInnerLevel = GL_PATCH_DEFAULT_INNER_LEVEL
	};

	enum class Space {
		Self = 0,
		World
	};

	enum class PrimitiveType {
		Plane = 0,
		Cube,
		Sphere,
		Capsule,
		Cylinder
	};

	enum class GizmoType {
		Translation,
		Scale,
		Rotation
	};

	enum class IndexFormat {
		UInt32 = GL_UNSIGNED_INT,
		UInt16 = GL_UNSIGNED_SHORT
	};

	enum class CullFace {
		Front = GL_FRONT,
		Back = GL_BACK,
		Both = GL_FRONT_AND_BACK
	};

	enum class BlendMode {
		BLEND_ALPHA,
		BLEND_ADDITIVE,
		BLEND_MULTIPLY
	};

	// only for OpenGL 2.1 below compatibility
	enum class MatrixMode {
		MODELVIEW,
		PROJECTION
	};

	enum class ImageType {
		IMAGE_PNG,
		IMAGE_BMP,
		IMAGE_TGA
	};
    
	inline BufferBits operator |(const BufferBits a, const BufferBits b) {
		return static_cast<BufferBits>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline BufferBits operator |=(BufferBits a, const BufferBits b) {
		return static_cast<BufferBits>(a = a | b);
	}

	inline BufferBits operator &(const BufferBits a, const BufferBits b) {
		return static_cast<BufferBits>(static_cast<int>(a) & static_cast<int>(b));
	}

	inline VertexSemantic operator |(const VertexSemantic a, const VertexSemantic b) {
		return static_cast<VertexSemantic>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline VertexSemantic operator |=(VertexSemantic a, const VertexSemantic b) {
		return static_cast<VertexSemantic>(a = a | b);
	}

	inline VertexSemantic operator &(const VertexSemantic a, const VertexSemantic b) {
		return static_cast<VertexSemantic>(static_cast<int>(a) & static_cast<int>(b));
	}
}

#endif /* Enumeration_h */
