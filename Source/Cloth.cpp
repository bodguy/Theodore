#include "Cloth.h"
#include "File.h"
#include "Enumeration.h"
#include "Graphics.h"
#include "Matrix4x4.h"
#include "Debug.h"
#include "Math.h"
#include "Time.h"

namespace Quark {
	float Cloth::Damping = 0.01f;
	Cloth::Cloth(int width, int height)
		:mEnabled(true), mUseGravity(true), mClothSolverFrequency(15), mWidth(width), mHeight(height), mTransform(), mVao(), mBuffer(Enumeration::BufferVertex), mProgram() {
		mParticles.resize(width * height);

		// creating particles in a grid of particles from (0,0,0) to (width,-height,0)
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				Vector3d pos = Vector3d(1.f * (x / (float)width), -1.f * (y / (float)height), 0.f);
				mParticles[y * width + x] = Particle(pos);
			}
		}

		// Connecting immediate neighbor
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (x<width - 1) AddConstraint(GetParticle(x, y), GetParticle(x + 1, y));
				if (y<height - 1) AddConstraint(GetParticle(x, y), GetParticle(x, y + 1));
				if (x<width - 1 && y<height - 1) AddConstraint(GetParticle(x, y), GetParticle(x + 1, y + 1));
				if (x<width - 1 && y<height - 1) AddConstraint(GetParticle(x + 1, y), GetParticle(x, y + 1));
			}
		}

		// Connecting secondary neighbors
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (x<width - 2) AddConstraint(GetParticle(x, y), GetParticle(x + 2, y));
				if (y<height - 2) AddConstraint(GetParticle(x, y), GetParticle(x, y + 2));
				if (x<width - 2 && y<height - 2) AddConstraint(GetParticle(x, y), GetParticle(x + 2, y + 2));
				if (x<width - 2 && y<height - 2) AddConstraint(GetParticle(x + 2, y), GetParticle(x, y + 2));
			}
		}

		for (int i = 0; i< 3; i++) {
			//GetParticle(0 + i, 0)->OffsetPos(Vector3d(0.5, 0.0, 0.0));
			GetParticle(0 + i, 0)->SetMovable(false);

			//GetParticle(0 + i, 0)->OffsetPos(Vector3d(-0.5, 0.0, 0.0));
			GetParticle(width - 1 - i, 0)->SetMovable(false);
		}

		using namespace Enumeration;
		Shader vs(VertexShader), fs(FragmentShader);
		File shaderFile;
		shaderFile.Open("Core/Shaders/gizmo/vs.glsl", Read);
		if (shaderFile.IsOpen()) {
			vs.Compile(shaderFile.ReadAllText());
			shaderFile.Close();
		}
		shaderFile.Open("Core/Shaders/gizmo/fs.glsl", Read);
		if (shaderFile.IsOpen()) {
			fs.Compile(shaderFile.ReadAllText());
			shaderFile.Close();
		}

		mProgram.AttachShader(vs);
		mProgram.AttachShader(fs);
		mProgram.Link();

		for (int x = 0; x < mWidth - 1; x++) {
			for (int y = 0; y < mHeight - 1; y++) {
				mStream.Vec3(GetParticle(x + 1, y)->GetPosition());
				mStream.Vec3(GetParticle(x, y)->GetPosition());
				mStream.Vec3(GetParticle(x, y + 1)->GetPosition());

				mStream.Vec3(GetParticle(x + 1, y + 1)->GetPosition());
				mStream.Vec3(GetParticle(x + 1, y)->GetPosition());
				mStream.Vec3(GetParticle(x, y + 1)->GetPosition());
			}
		}
		mBuffer.Data(mStream.Pointer(), mStream.Size(), Enumeration::BufferUsage::DynamicDraw);
		mVao.BindAttribute(mProgram.GetAttribute("position"), mBuffer, 3, 3 * sizeof(float), 0);

		mTransform.SetPosition(Vector3d(-0.8f, 0.5f, 0.f));
		mTransform.SetLossyScale(Vector3d(1.f, 2.f, 1.f));
	}

	Cloth::~Cloth() {
	}

	float Cloth::GetDamping() {
		return Cloth::Damping;
	}

	Particle* Cloth::GetParticle(int x, int y) {
		return &mParticles[y * mWidth + x];
	}

	void Cloth::AddConstraint(Particle* p1, Particle* p2) {
		mConstraint.push_back(Constraint(p1, p2));
	}

	Vector3d Cloth::calcTriangleNormal(Particle *p1, Particle *p2, Particle *p3) {
		Vector3d pos1 = p1->GetPosition();
		Vector3d pos2 = p2->GetPosition();
		Vector3d pos3 = p3->GetPosition();

		Vector3d v1 = pos2 - pos1;
		Vector3d v2 = pos3 - pos1;

		return Vector3d::CrossProduct(v1, v2);
	}

	void Cloth::addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const Vector3d& direction) {
		Vector3d normal = calcTriangleNormal(p1, p2, p3);
		Vector3d d = normal.Normalize();
		Vector3d force = normal * Vector3d::DotProduct(d, direction);
		p1->AddForce(force);
		p2->AddForce(force);
		p3->AddForce(force);
	}

	void Cloth::WindForce(const Vector3d& direction) {
		for (int x = 0; x < mWidth - 1; x++) {
			for (int y = 0; y < mHeight - 1; y++) {
				addWindForcesForTriangle(GetParticle(x + 1, y), GetParticle(x, y), GetParticle(x, y + 1), direction);
				addWindForcesForTriangle(GetParticle(x + 1, y + 1), GetParticle(x + 1, y), GetParticle(x, y + 1), direction);
			}
		}
	}

	void Cloth::Render(const Camera& cam) {
		mProgram.Use();
		mProgram.SetUniform(mProgram.GetUniform("model"), mTransform.GetLocalToWorldMatrix());
		mProgram.SetUniform(mProgram.GetUniform("view"), cam.GetWorldToCameraMatrix());
		mProgram.SetUniform(mProgram.GetUniform("projection"), cam.GetProjectionMatrix());
		mProgram.SetUniform(mProgram.GetUniform("color"), Color::Lerp(Color::purple, Color::green, Math::PingPong(Time::ElapsedTime() / 2.f, 1.f)));

		InputStream stream;
		for (int x = 0; x < mWidth - 1; x++) {
			for (int y = 0; y < mHeight - 1; y++) {
				stream.Vec3(GetParticle(x + 1, y)->GetPosition());
				stream.Vec3(GetParticle(x, y)->GetPosition());
				stream.Vec3(GetParticle(x, y + 1)->GetPosition());

				stream.Vec3(GetParticle(x + 1, y + 1)->GetPosition());
				stream.Vec3(GetParticle(x + 1, y)->GetPosition());
				stream.Vec3(GetParticle(x, y + 1)->GetPosition());
			}
		}

		mBuffer.SubData(stream.Pointer(), 0, stream.Size());
		mVao.BindAttribute(mProgram.GetAttribute("position"), mBuffer, 3, 3 * sizeof(float), 0);
		Graphics::DrawArrays(mVao, Enumeration::Primitive::Triangles, 0, 6 * mWidth * mHeight);
		//Graphics::DrawArraysInstanced(mVao, Enumeration::Primitive::Triangles, 0, 6, mWidth * mHeight);
	}

	void Cloth::Update(float dt) {
		if (mEnabled) {
			for (int i = 0; i < mClothSolverFrequency; i++) {
				for (Constraint& constraint : mConstraint) {
					constraint.Satisfy();
				}
			}

			for (Particle& particle : mParticles) {
				if (mUseGravity) {
					particle.AddForce(Vector3d(0.f, -0.1f, 0.f) * dt);
				}
				particle.Update(dt);
			}
		}
	}
}