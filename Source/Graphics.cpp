#include "Graphics.h"
#include "Color.h"
#include "VertexBuffer.h"
#include "FrameBuffer.h"
#include "Debug.h"
#include "Platform.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

namespace Quark {
	Graphics::Graphics() {
	}

	Graphics::~Graphics() {
	}

    void Graphics::Clear(const Color& color, Enumeration::BufferBits bits) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(bits);
    }
    
    void Graphics::BindTexture(unsigned int unit, Texture* texture) {
        glActiveTexture(GL_TEXTURE0 + unit);
        if (texture) {
            glBindTexture(texture->GetDimension(), texture->GetTextureID());
        } else {
            glBindTexture(GL_TEXTURE_2D, NULL);
        }
    }
    
    void Graphics::BindFrameBuffer(FrameBuffer* buffer) {
        if (buffer) {
            glBindFramebuffer(GL_FRAMEBUFFER, buffer->GetFrameBufferID());
            glViewport(0, 0, buffer->GetWidth(), buffer->GetHeight());
        } else {
            glBindFramebuffer(GL_FRAMEBUFFER, NULL);
        }
    }
    
    void Graphics::DrawArrays(const VertexArray& vao, Enumeration::Primitive mode, unsigned int offset, unsigned int vertices) {
        glBindVertexArray(vao.GetArrayID());
        glDrawArrays(mode, offset, vertices);
        glBindVertexArray(0);
    }
    
    void Graphics::DrawElements(const VertexArray& vao, Enumeration::Primitive mode, unsigned int offset, unsigned int count, unsigned int type) {
        glBindVertexArray(vao.GetArrayID());
        glDrawElements(mode, count, type, reinterpret_cast<const void*>(offset));
        glBindVertexArray(0);
    }
    
    void Graphics::DrawArraysInstanced(const VertexArray& vao, Enumeration::Primitive mode, unsigned int offset, unsigned int vertices, unsigned int count) {
        glBindVertexArray(vao.GetArrayID());
        glDrawArraysInstanced(mode, offset, vertices, count);
        glBindVertexArray(0);
    }
    
    void Graphics::DrawElementsInstanced(const VertexArray& vao, Enumeration::Primitive mode, unsigned int offset, unsigned int count, unsigned int type, unsigned int primcount) {
        glBindVertexArray(vao.GetArrayID());
        glDrawElementsInstanced(mode, count, type, reinterpret_cast<const void*>(offset), primcount);
        glBindVertexArray(0);
    }
    
    void Graphics::GetLastError() {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            switch (err) {
                case GL_INVALID_ENUM: // 0x0500
                    Debug::Log("GL_INVALID_ENUM\n");
                    break;
                case GL_INVALID_VALUE: // 0x0501
                    Debug::Log("GL_INVALID_VALUE\n");
                    break;
                case GL_INVALID_OPERATION: // 0x0502
                    Debug::Log("GL_INVALID_OPERATION\n");
                    break;
                case GL_STACK_OVERFLOW: // 0x0503
                    Debug::Log("GL_STACK_OVERFLOW\n");
                    break;
                case GL_STACK_UNDERFLOW: // 0x0504
                    Debug::Log("GL_STACK_UNDERFLOW\n");
                    break;
                case GL_OUT_OF_MEMORY: // 0x0505
                    Debug::Log("GL_OUT_OF_MEMORY\n");
                    break;
            }
        }
    }
    
    void Graphics::SetPolygonMode(Enumeration::FillMode value) {
        glPolygonMode( GL_FRONT_AND_BACK, value );
    }
    
    void Graphics::Enable(Enumeration::Capabilities value) {
        glEnable(value);
    }
    
    void Graphics::Disable(Enumeration::Capabilities value) {
        glDisable(value);
    }

	void Graphics::PatchParameter(Enumeration::PatchParameters param, int value) {
		glPatchParameteri(param, value);
	}
	
	void Graphics::PatchParameter(Enumeration::PatchParameters param, float* values) {
		glPatchParameterfv(param, values);
	}

	void Graphics::ScreenToImage(const std::string& filename, Enumeration::ImageType type) {
		int w = Platform::GetWidth();
		int	h = Platform::GetHeight();
		char *data = new char[w * h * 3];
		glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);

		char* tmpline = new char[w * 3];

		const int linewidth = w * 3;

		//flip the image
		//for (int y = 0; y < (h / 2); y++) {
		//	std::copy(data + y * linewidth, data + y * linewidth + linewidth, tmpline);
		//	std::copy(data + (h - y) * linewidth, data + (h - y) * linewidth + linewidth, data + y * linewidth);
		//	std::copy(tmpline, tmpline + linewidth, data + (h - y) * linewidth);
		//}

		switch (type) {
		case Enumeration::IMAGE_PNG:
			stbi_write_png(filename.data(), w, h, 3, data, w * 3); 
			break;
		case Enumeration::IMAGE_BMP:
			stbi_write_bmp(filename.data(), w, h, 3, data); 
			break;
		case Enumeration::IMAGE_TGA:
			stbi_write_tga(filename.data(), w, h, 3, data); 
			break;
		}

		delete tmpline;
		delete data;
	}

#if 0
	// OpenGL 2.1 version rendering functions implements
	void Graphics::Vertex(const Vector3d& vector) {
		glVertex3f(vector.x, vector.y, vector.z);
	}

	void Graphics::Point(const Vector3d& vector, const Color& color, float thickness) {
		SetColor(color);
		glPointSize(thickness);
		glBegin(GL_POINT);
		Vertex(vector);
		glEnd();
	}

	void Graphics::Line(const Vector3d& start, const Vector3d& end, const Color& color, float thickness) {
		glLineWidth(thickness);
		SetColor(color);
		glBegin(GL_LINE_LOOP);
		Vertex(start);
		Vertex(end);
		glEnd();
	}

	void Graphics::Triangle(const Vector3d& a, const Vector3d& b, const Vector3d& c, const Color& color, float thickness) {
		Line(a, b, color, thickness);
		Line(b, c, color, thickness);
		Line(c, a, color, thickness);
	}

	void Graphics::SolidTriangle(const Vector3d& a, const Vector3d& b, const Vector3d& c, const Color& color) {
		SetColor(color);
		glBegin(GL_TRIANGLES);
		Vertex(a);
		Vertex(b);
		Vertex(c);
		glEnd();
	}

	void Graphics::Circle(const Vector3d& center, float radius, const Color& color, float thickness) {
		SetColor(color);
		glLineWidth(thickness);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < Math::degrees; i++) {
			const float rad = i * Math::degrees_to_radians;
			Vertex(Vector3d(center.x + std::cos(rad)*radius, center.y + std::sin(rad)*radius));
		}
		glEnd();
	}

	void Graphics::Quad(const Vector3d& center, float w, float h, const Color& color, float thickness) {
		float halfWidth = w * 0.5f;
		float halfHeight = h * 0.5f;

		Line(Vector3d(-halfWidth + center.x, halfHeight + center.y),
			Vector3d(halfWidth + center.x, halfHeight + center.y), color, thickness);
		Line(Vector3d(halfWidth + center.x, halfHeight + center.y),
			Vector3d(halfWidth + center.x, -halfHeight + center.y), color, thickness);
		Line(Vector3d(halfWidth + center.x, -halfHeight + center.y),
			Vector3d(-halfWidth + center.x, -halfHeight + center.y), color, thickness);
		Line(Vector3d(-halfWidth + center.x, -halfHeight + center.y),
			Vector3d(-halfWidth + center.x, halfHeight + center.y), color, thickness);
	}

	void Graphics::SolidQuad(const Vector3d& center, float w, float h, const Color& color) {
		SetColor(color);

		float halfWidth = w * 0.5f;
		float halfHeight = h * 0.5f;

		glBegin(GL_QUADS);
		Vertex(Vector3d(-halfWidth + center.x, halfHeight + center.y));
		Vertex(Vector3d(halfWidth + center.x, halfHeight + center.y));
		Vertex(Vector3d(halfWidth + center.x, -halfHeight + center.y));
		Vertex(Vector3d(-halfWidth + center.x, -halfHeight + center.y));
		glEnd();
	}

	void Graphics::TexturedQuad(const Vector3d& center, float w, float h, const Vector3d& offset, const Vector3d& scale, const Color& color) {
		SetColor(color);
		float halfWidth = w*0.5f;
		float halfHeight = h*0.5f;

		glBegin(GL_QUADS);
		// UL
		glTexCoord2f(offset.x, offset.y);
		Vertex(Vector3d(-halfWidth + center.x, halfHeight + center.y));

		//UR
		glTexCoord2f(offset.x + scale.x, offset.y);
		Vertex(Vector3d(halfWidth + center.x, halfHeight + center.y));

		//LR
		glTexCoord2f(offset.x + scale.x, offset.y + scale.y);
		Vertex(Vector3d(halfWidth + center.x, -halfHeight + center.y));

		//LL
		glTexCoord2f(offset.x, offset.y + scale.y);
		Vertex(Vector3d(-halfWidth + center.x, -halfHeight + center.y));
		glEnd();
	}

	void Graphics::Polygon(const std::vector<Vector3d>& verts, const Color& color, float thickness) {
		glLineWidth(thickness);
		SetColor(color);
		glBegin(GL_LINE_LOOP);
		for (unsigned int i = 0; i < verts.size() - 1; i++) {
			Vertex(verts[i]);
			Vertex(verts[i + 1]);
		}
		glEnd();
	}

	void Graphics::SetDimensions(unsigned int w, unsigned int h) {
		glViewport(0, 0, w, h);
		Set2DGraphics(w, h);
	}

	void Graphics::SetMatrixMode(Enumeration::MatrixMode mode) {
		switch (mode) {
		case Enumeration::MODELVIEW:
			glMatrixMode(GL_MODELVIEW);
			break;
		case Enumeration::PROJECTION:
			glMatrixMode(GL_PROJECTION);
			break;
		}
	}

	void Graphics::PopMatrix() {
		glPopMatrix();
	}

	void Graphics::PushMatirx() {
		glPushMatrix();
	}

	void Graphics::Translate(const Vector3d& pos) {
		glTranslatef(pos.x, pos.y, 0.f);
	}

	void Graphics::Scale(const Vector3d& scale) {
		glScalef(scale.x, scale.y, 1.f);
	}

	void Graphics::Rotate(float r) {
		glRotatef(r, 0.f, 0.f, 1.f);
	}

	void Graphics::SetColor(const Color& color) {
		glColor4f(color.r, color.g, color.b, color.a);
	}

	void Graphics::Flush() {
		glFlush();
	}

	void Graphics::Set2DGraphics(int w, int h) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, w, h, 0.0, 0.0, 1.0);
	}

	void Graphics::SetBlendMode(Enumeration::BlendMode type) {
		glEnable(GL_BLEND);
		switch (type) {
		case Enumeration::BLEND_ALPHA:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case Enumeration::BLEND_ADDITIVE:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case Enumeration::BLEND_MULTIPLY:
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		}
	}
#endif
}
