#include "WaveFrontObjMesh.h"
#include "File.h"
#include "StringUtil.h"
#include "Time.h"
#include "Debug.h"
#include <string>

namespace Quark {
	WaveFrontObjMesh::WaveFrontObjMesh() : Mesh() {
	}

	WaveFrontObjMesh::~WaveFrontObjMesh() {
	}

	bool WaveFrontObjMesh::LoadObj(const std::string& filepath) {
		File file;
		file.Open(filepath, OpenMode::Read);
		if (!file.IsOpen()) {
			return false;
		}

		std::string line;
		std::vector<std::string> currentLine;
		std::vector<Vector2d> temp_uvs;
		std::vector<Vector3d> temp_normals;

		std::vector<std::string> vertex1;
		std::vector<std::string> vertex2;
		std::vector<std::string> vertex3;

		TimePoint start1 = Time::GetTime();

		while (true) {
			line = file.GetLine();
			currentLine.clear();
			StringUtil::Split(line, currentLine, ' ');

			if (StringUtil::StartsWith(line, "v ")) {
				mVertices.push_back(Vector3d(StringUtil::ParseFloat(currentLine[1]), StringUtil::ParseFloat(currentLine[2]), StringUtil::ParseFloat(currentLine[3])));
			} else if (StringUtil::StartsWith(line, "vt ")) {
				temp_uvs.push_back(Vector2d(StringUtil::ParseFloat(currentLine[1]), StringUtil::ParseFloat(currentLine[2])));
			} else if (StringUtil::StartsWith(line, "vn ")) {
				temp_normals.push_back(Vector3d(StringUtil::ParseFloat(currentLine[1]), StringUtil::ParseFloat(currentLine[2]), StringUtil::ParseFloat(currentLine[3])));
			} else if (StringUtil::StartsWith(line, "f ")) {
				// moving to the next section
				mUvs.resize(mVertices.size());
				mNormals.resize(mVertices.size());
				break;
			}
		}

		TimePoint end1 = Time::GetTime();
		Debug::Log("first while loop %fms", Time::GetInterval(start1, end1));
		TimePoint start2 = Time::GetTime();

		while (!line.empty()) {
			if (!StringUtil::StartsWith(line, "f ")) {
				line = file.GetLine();
				continue;
			}

			currentLine.clear();
			vertex1.clear();
			vertex2.clear();
			vertex3.clear();
			StringUtil::Split(line, currentLine, ' ');
			StringUtil::Split(currentLine[1], vertex1, '/');
			StringUtil::Split(currentLine[2], vertex2, '/');
			StringUtil::Split(currentLine[3], vertex3, '/');

			ProcessVertex(vertex1, temp_uvs, temp_normals);
			ProcessVertex(vertex2, temp_uvs, temp_normals);
			ProcessVertex(vertex3, temp_uvs, temp_normals);
			line = file.GetLine();
		}

		TimePoint end2 = Time::GetTime();
		Debug::Log("second while loop %fms", Time::GetInterval(start2, end2));

		file.Close();
		mSemantic = VertexSemantic::SemanticPosition | VertexSemantic::SemanticNormal | VertexSemantic::SemanticFaces;

		return true;
	}

	void WaveFrontObjMesh::ProcessVertex(const std::vector<std::string>& splitStr, const std::vector<Vector2d>& uvsIn, const std::vector<Vector3d>& normalsIn) {
		int currentVertexIndex = StringUtil::ParseInteger(splitStr[0]) - 1; // obj index start with 1 not 0
		mFaces.push_back(currentVertexIndex);
		
		if (!splitStr[1].empty()) {
			Vector2d currentUv = uvsIn[StringUtil::ParseInteger(splitStr[1]) - 1];
			mUvs[currentVertexIndex] = Vector2d(currentUv.x, 1.f - currentUv.y);
		}
		
		if (splitStr.size() == 3 && !splitStr[2].empty()) {
			Vector3d currentNormal = normalsIn[StringUtil::ParseInteger(splitStr[2]) - 1];
			mNormals[currentVertexIndex] = currentNormal;
		}
	}
}