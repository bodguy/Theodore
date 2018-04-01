#ifndef Cubemap_h
#define Cubemap_h

#include <list>
#include "Enumeration.h"

namespace Quark {
	class VertexDataBuffer; class Buffer; class VertexArray; class Program; class Matrix4x4;
	class Cubemap {
	public:
		// right, left, top, bottom, back, front
		Cubemap(std::list<std::string>& filelist, Enumeration::TextureFormat format);
		~Cubemap();

		void Render();
		void SetMatrix(const Matrix4x4& view, const Matrix4x4& proj);

	private:
		VertexDataBuffer* mVertexData;
		Buffer* mVertexBuffer;
		VertexArray* mVertexArray;
		Program* mCubeProgram;
		unsigned int mCubemapID;
	};
}

#endif /* Cubemap_h */
