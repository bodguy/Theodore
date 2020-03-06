// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "WaveFrontObjMesh.h"

#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <Object/Component/sub/Material.h>

#include "Helper/StringUtil.h"
#include "Helper/Debug.h"
#include "Platform/Time.h"
#include "Asset/Texture.h"

namespace Theodore {
	WaveFrontObjMesh::WaveFrontObjMesh() : Mesh(), meshGroup() {}

	WaveFrontObjMesh::~WaveFrontObjMesh() {}

	bool WaveFrontObjMesh::LoadMesh(const std::string& fileName, MeshParseOption parseOption) {
		if (!StringUtil::EndsWith(fileName, ".obj")) {
			Debug::Error("obj file should be .obj file format");
			return false;
		}

		std::ifstream inputStream(fileName);
		if(!inputStream) return false;

		std::vector<Vector3d> vertices;
		std::vector<Vector2d> texcoords;
		std::vector<Vector3d> normals;
		std::unordered_map<std::string, int> materialMap;
		PrimitiveGroup currentPrimitive;
		std::string currentObjectName;
		std::string currentMaterialName;
		Mesh currentMesh;
		int currentMaterialId = -1;
		auto pair = StringUtil::SplitPair(fileName, "\\/");
		baseName = pair.first;
		std::string pureFileName = pair.second;
		std::string lineBuffer;

		// preventing a empty file
		while(inputStream.peek() != -1) {
			StringUtil::GetLine(inputStream, lineBuffer);

			// Trim newline '\r\n' or '\n'
			if (!lineBuffer.empty()) {
				if (lineBuffer[lineBuffer.size() - 1] == '\n')
					lineBuffer.erase(lineBuffer.size() - 1);
			}
			if (!lineBuffer.empty()) {
				if (lineBuffer[lineBuffer.size() - 1] == '\r')
					lineBuffer.erase(lineBuffer.size() - 1);
			}

			// Skip if empty line.
			if (lineBuffer.empty()) {
				continue;
			}

			// Skip leading space.
			const char *token = lineBuffer.c_str(); // read only token
			token += strspn(token, " \t");

			if (token == nullptr) return false;
			if (token[0] == '\0') continue;  // empty line
			if (token[0] == '#') continue;  // comment line

			// vertex
			if (token[0] == 'v' && StringUtil::IsSpace(token[1])) {
				token += 2;
				Vector3d v;
				ParseReal3(v, &token);
				vertices.emplace_back(v);
				continue;
			}

			// normal
			if (token[0] == 'v' && token[1] == 'n' && StringUtil::IsSpace(token[2])) {
				token += 3;
				Vector3d vn;
				ParseReal3(vn, &token);
				normals.emplace_back(vn);
				continue;
			}

			// texcoord
			if (token[0] == 'v' && token[1] == 't' && StringUtil::IsSpace(token[2])) {
				token += 3;
				Vector2d vt;
				ParseReal2(vt, &token);
				if (parseOption & MeshParseOption::FLIP_UV) {
					vt.y = 1.f - vt.y;
				}
				texcoords.emplace_back(vt);
				continue;
			}

			// face
			if (token[0] == 'f' && StringUtil::IsSpace(token[1])) {
				token += 2;
				token += strspn(token, " \t"); // Skip leading space.

				Triangle triangle;
				triangle.vertexIndices.reserve(3);

				while (!StringUtil::IsNewLine(token[0])) {
					VertexIndex vi;
					if (!ParseIndices(&token, vertices.size(), normals.size(), texcoords.size(), &vi)) {
						return false;
					}

					// finish parse indices
					triangle.vertexIndices.emplace_back(vi);
					token += strspn(token, " \t\r"); // skip space
				}

				currentPrimitive.triangles.emplace_back(triangle);
				continue;
			}

			// use mtl
			if ((0 == strncmp(token, "usemtl", 6)) && StringUtil::IsSpace(token[6])) {
				token += 7;
				std::string newMaterialName = ParseString(&token);
				int newMaterialId = -1;
				// find material id
				if (materialMap.find(newMaterialName) != materialMap.end()) {
					newMaterialId = materialMap[newMaterialName];
				}

				// check current material and previous
				if (newMaterialName != currentMaterialName) {
					// when current object name is empty, then assign current material name as alternatives.
					if (currentObjectName.empty()) {
						currentObjectName = newMaterialName;
					}
					// return value not used
					ParsePrimitive(currentMesh, currentPrimitive, parseOption, currentMaterialId, vertices, texcoords, normals, currentObjectName, pureFileName);
					if (!currentMesh.IsVertexEmpty()) {
						meshGroup.mMeshes.emplace_back(currentMesh);
						// when successfully push a new mesh, then cache current material name.
						currentObjectName = newMaterialName;
					}
					// reset
					currentPrimitive = PrimitiveGroup();
					currentMesh = Mesh();
					// cache new material id
					currentMaterialId = newMaterialId;
					currentMaterialName = newMaterialName;
				}
				continue;
			}

			// load mtl
			if ((0 == strncmp(token, "mtllib", 6)) && StringUtil::IsSpace(token[6])) {
				token += 7;
				std::vector<std::string> materialFileNames;
				// parse multiple mtl filenames split by whitespace
				SplitWithToken(materialFileNames, " ", &token);
				// load just one available mtl file in the list
				for (auto& name : materialFileNames) {
					if (ParseMaterial(baseName + name, meshGroup.mMaterials, materialMap)) {
						break;
					}
				}
				continue;
			}

			// group name
			if (token[0] == 'g' && StringUtil::IsSpace(token[1])) {
				// return value not used
				ParsePrimitive(currentMesh, currentPrimitive, parseOption, currentMaterialId, vertices, texcoords, normals, currentObjectName, pureFileName);
				if (!currentMesh.IsVertexEmpty()) {
					meshGroup.mMeshes.emplace_back(currentMesh);
					currentObjectName = "";
				}

				// reset
				currentPrimitive = PrimitiveGroup();
				currentMesh = Mesh();

				token += 2;

				// assemble multi group name
				std::vector<std::string> names;
				while (!StringUtil::IsNewLine(token[0])) {
					names.emplace_back(ParseString(&token));
					token += strspn(token, " \t\r"); // skip space
				}

				if (!names.empty()) {
					std::stringstream ss;
					std::vector<std::string>::const_iterator it = names.begin();
					ss << *it++;
					for (; it != names.end(); it++) {
						ss << " " << *it;
					}
					currentObjectName = ss.str();
				}

				continue;
			}

			// object name
			if (token[0] == 'o' && StringUtil::IsSpace(token[1])) {
				// return value not used
				ParsePrimitive(currentMesh, currentPrimitive, parseOption, currentMaterialId, vertices, texcoords, normals, currentObjectName, pureFileName);
				if (!currentMesh.IsVertexEmpty()) {
					meshGroup.mMeshes.emplace_back(currentMesh);
					currentObjectName = "";
				}

				// reset
				currentPrimitive = PrimitiveGroup();
				currentMesh = Mesh();

				token += 2;
				currentObjectName = ParseString(&token);
				continue;
			}
		}

		bool result = ParsePrimitive(currentMesh, currentPrimitive, parseOption, currentMaterialId, vertices, texcoords, normals, currentObjectName, pureFileName);
		if (result || !currentMesh.IsVertexEmpty()) {
			meshGroup.mMeshes.emplace_back(currentMesh);
		}

		return true;
	}

	bool WaveFrontObjMesh::ParsePrimitive(Mesh& mesh, const PrimitiveGroup& primitive, MeshParseOption option, const int materialId,
											const std::vector<Vector3d>& vertices, const std::vector<Vector2d>& texcoords, const std::vector<Vector3d>& normals,
											const std::string& name, const std::string& default_name) {
		if (primitive.IsEmpty()) {
			return false;
		}
		mesh.name = name.empty() ? default_name : name;

		// make polygon
		unsigned int count = 0;
		for (const Triangle& face : primitive.triangles) {
			size_t npolys = face.vertexIndices.size();

			if (npolys < 3) {
				// face must have at least 3+ vertices.
				continue;
			}

			// triangulate only parsing flag is set and polygon has more than 3.
			if ((option & MeshParseOption::TRIANGULATE) && npolys != 3) {
				triangulate(mesh, vertices, npolys);
			} else {
				for (size_t f = 0; f < npolys; f++) {
					Vertex vtx;
					VertexIndex idx = face.vertexIndices[f];
					vtx.position = vertices[idx.v_idx];
					vtx.texcoord = (idx.vt_idx == -1 ? Vector2d() : texcoords[idx.vt_idx]);
					vtx.normal = (idx.vn_idx == -1 ? Vector3d() : normals[idx.vn_idx]);
					mesh.vertices.emplace_back(vtx);
				}

				if ((option & MeshParseOption::CALC_TANGENT) && npolys == 3) {
					calcTangent(mesh, count);
				}

				auto preCompute = (unsigned int)((mesh.vertices.size()) - npolys);
				for (size_t ff = 0; ff < npolys; ff++) {
					mesh.indices.emplace_back(preCompute + ff);
				}
				mesh.material_id = materialId;
				count++;
			}
		}

		return true;
	}

	bool WaveFrontObjMesh::ParseIndices(const char** token, int vsize, int vnsize, int vtsize, VertexIndex* result) {
		if (!result) return false;

		VertexIndex vi(-1);
		// i
		if (!FixIndex(atoi((*token)), vsize, &(vi.v_idx))) {
			return false;
		}
		(*token) += strcspn((*token), "/ \t\r"); // go to next slash
		// check if only have vertex
		if ((*token)[0] != '/') {
			(*result) = vi;
			return true;
		}
		(*token)++;

		// i//k
		//   +--- here
		if ((*token)[0] == '/') {
			(*token)++; // now then, token is pointing at 'k'
			if (!FixIndex(atoi((*token)), vnsize, &(vi.vn_idx))) {
				return false;
			}
			(*token) += strcspn((*token), "/ \t\r"); // go to next slash (although, it's not exist)
			(*result) = vi;
			return true;
		}

		// i/j/k or i/j
		//   +--- here
		if (!FixIndex(atoi((*token)), vtsize, &(vi.vt_idx))) {
			return false;
		}
		(*token) += strcspn((*token), "/ \t\r"); // go to next slash
		if ((*token)[0] != '/') {
			// it's i/j case
			(*result) = vi;
			return true;
		}

		// process last case
		// i/j/k
		(*token)++; // now then, token is pointing at 'k'
		if (!FixIndex(atoi((*token)), vnsize, &(vi.vn_idx))) {
			return false;
		}
		(*token) += strcspn((*token), "/ \t\r"); // go to next slash (although, it's not exist)
		(*result) = vi;

		return true;
	}

	TextureFace WaveFrontObjMesh::ParseTextureFace(const char** token) {
		(*token) += strspn((*token), " \t");
		const char *end = (*token) + strcspn((*token), " \t\r");
		TextureFace textureFace = TextureFace::TEX_2D;

		if ((0 == strncmp((*token), "cube_top", 8))) {
			textureFace = TextureFace::TEX_3D_CUBE_TOP;
		} else if ((0 == strncmp((*token), "cube_bottom", 11))) {
			textureFace = TextureFace::TEX_3D_CUBE_BOTTOM;
		} else if ((0 == strncmp((*token), "cube_left", 9))) {
			textureFace = TextureFace::TEX_3D_CUBE_LEFT;
		} else if ((0 == strncmp((*token), "cube_right", 10))) {
			textureFace = TextureFace::TEX_3D_CUBE_RIGHT;
		} else if ((0 == strncmp((*token), "cube_front", 10))) {
			textureFace = TextureFace::TEX_3D_CUBE_FRONT;
		} else if ((0 == strncmp((*token), "cube_back", 9))) {
			textureFace = TextureFace::TEX_3D_CUBE_BACK;
		} else if ((0 == strncmp((*token), "sphere", 6))) {
			textureFace = TextureFace::TEX_3D_SPHERE;
		}

		(*token) = end;
		return textureFace;
	}

	bool WaveFrontObjMesh::ParseTexture(Texture& tex, const char* token) {
		while (!StringUtil::IsNewLine((*token))) {
			token += strspn(token, " \t");  // skip space
			if ((0 == strncmp(token, "-clamp", 6)) && StringUtil::IsSpace((token[6]))) {
				token += 7;
				tex.textureOption.clamp = ParseOnOff(&token, true);
			} else if ((0 == strncmp(token, "-blendu", 7)) && StringUtil::IsSpace((token[7]))) {
				token += 8;
				tex.textureOption.blendu = ParseOnOff(&token, true);
			} else if ((0 == strncmp(token, "-blendv", 7)) && StringUtil::IsSpace((token[7]))) {
				token += 8;
				tex.textureOption.blendv = ParseOnOff(&token, true);
			} else if ((0 == strncmp(token, "-bm", 3)) && StringUtil::IsSpace((token[3]))) {
				token += 4;
				tex.textureOption.bump_multiplier = ParseReal(&token, 1.f);
			} else if ((0 == strncmp(token, "-boost", 6)) && StringUtil::IsSpace((token[6]))) {
				token += 7;
				tex.textureOption.sharpness = ParseReal(&token, 1.f);
			} else if ((0 == strncmp(token, "-mm", 3)) && StringUtil::IsSpace((token[3]))) {
				token += 4;
				tex.textureOption.brightness = ParseReal(&token, 0.f);
				tex.textureOption.contrast = ParseReal(&token, 1.f);
			} else if ((0 == strncmp(token, "-o", 2)) && StringUtil::IsSpace((token[2]))) {
				token += 3;
				ParseReal3(tex.textureOption.origin_offset, &token);
			} else if ((0 == strncmp(token, "-s", 2)) && StringUtil::IsSpace((token[2]))) {
				token += 3;
				ParseReal3(tex.textureOption.scale, &token, 1.f, 1.f, 1.f);
			} else if ((0 == strncmp(token, "-t", 2)) && StringUtil::IsSpace((token[2]))) {
				token += 3;
				ParseReal3(tex.textureOption.turbulence, &token);
			} else if ((0 == strncmp(token, "-imfchan", 8)) && StringUtil::IsSpace((token[8]))) {
				token += 9;
				token += strspn(token, " \t");
				const char *end = token + strcspn(token, " \t\r");
				if ((end - token) == 1) {  // Assume one char for -imfchan
					tex.textureOption.imfchan = (*token);
				}
				token = end;
			} else if ((0 == strncmp(token, "-type", 5)) && StringUtil::IsSpace((token[5]))) {
				token += 5;
				tex.textureOption.face_type = ParseTextureFace(&token);
			} else {
				// parse texture name at last.
				tex.name = ParseString(&token);
				if (tex.name.empty()) {
					return false;
				}
			}
		}

		return true;
	}

	bool WaveFrontObjMesh::LoadMaterial(std::vector<Material>& materials, std::unordered_map<std::string, int>& materialMap, std::istream& inputStream) {
		Material current_mat;
		bool has_d = false;
		std::string lineBuffer;

		// preventing a empty file
		while(inputStream.peek() != -1) {
			StringUtil::GetLine(inputStream, lineBuffer);

			// Trim trailing whitespace.
			if (!lineBuffer.empty()) {
				lineBuffer = lineBuffer.substr(0, lineBuffer.find_last_not_of(" \t") + 1);
			}

			// Trim newline '\r\n' or '\n'
			if (!lineBuffer.empty()) {
				if (lineBuffer[lineBuffer.size() - 1] == '\n')
					lineBuffer.erase(lineBuffer.size() - 1);
			}
			if (!lineBuffer.empty()) {
				if (lineBuffer[lineBuffer.size() - 1] == '\r')
					lineBuffer.erase(lineBuffer.size() - 1);
			}

			// Skip if empty line.
			if (lineBuffer.empty()) {
				continue;
			}

			// Skip leading space.
			const char *token = lineBuffer.c_str(); // read only token
			token += strspn(token, " \t");

			if (token == nullptr) return false;
			if (token[0] == '\0') continue;  // empty line
			if (token[0] == '#') continue;  // comment line

			// new mtl
			if ((0 == strncmp(token, "newmtl", 6)) && StringUtil::IsSpace(token[6])) {
				// save previous material
				if (!current_mat.name.empty()) {
					materialMap.insert(std::make_pair(current_mat.name, static_cast<int>(materials.size())));
					materials.emplace_back(current_mat);
				}

				// reset material
				current_mat = Material();
				has_d = false;

				// parse new mat name
				token += 7;
				current_mat.name = ParseString(&token);
				continue;
			}

			// ambient
			if (token[0] == 'K' && token[1] == 'a' && StringUtil::IsSpace(token[2])) {
				token += 2;
				Vector3d ambient;
				ParseReal3(ambient, &token);
				current_mat.ambient = ambient;
				continue;
			}

			// diffuse
			if (token[0] == 'K' && token[1] == 'd' && StringUtil::IsSpace(token[2])) {
				token += 2;
				Vector3d diffuse;
				ParseReal3(diffuse, &token);
				current_mat.diffuse = diffuse;
				continue;
			}

			// specular
			if (token[0] == 'K' && token[1] == 's' && StringUtil::IsSpace(token[2])) {
				token += 2;
				Vector3d specular;
				ParseReal3(specular, &token);
				current_mat.specular = specular;
				continue;
			}

			// transmittance
			if ((token[0] == 'K' && token[1] == 't' && StringUtil::IsSpace(token[2])) ||
					(token[0] == 'T' && token[1] == 'f' && StringUtil::IsSpace(token[2]))) {
				token += 2;
				Vector3d transmittance;
				ParseReal3(transmittance, &token);
				current_mat.transmittance = transmittance;
				continue;
			}

			// ior(index of refraction)
			if (token[0] == 'N' && token[1] == 'i' && StringUtil::IsSpace(token[2])) {
				token += 2;
				current_mat.ior = ParseReal(&token, 0.f);
				continue;
			}

			// emission
			if (token[0] == 'K' && token[1] == 'e' && StringUtil::IsSpace(token[2])) {
				token += 2;
				Vector3d emission;
				ParseReal3(emission, &token);
				current_mat.emission = emission;
				continue;
			}

			// shininess
			if (token[0] == 'N' && token[1] == 's' && StringUtil::IsSpace(token[2])) {
				token += 2;
				current_mat.shininess = ParseReal(&token, 0.f);
				continue;
			}

			// illum model
			if (0 == strncmp(token, "illum", 5) && StringUtil::IsSpace(token[5])) {
				token += 6;
				current_mat.illum = ParseInt(&token);
				continue;
			}

			// dissolve (the non-transparency of the material), The default is 1.0 (not transparent at all)
			if ((token[0] == 'd' && StringUtil::IsSpace(token[1]))) {
				token += 1;
				current_mat.dissolve = ParseReal(&token, 1.f);
				has_d = true;
				continue;
			}

			// dissolve (the transparency of the material): 1.0 - Tr, The default is 0.0 (not transparent at all)
			if (token[0] == 'T' && token[1] == 'r' && StringUtil::IsSpace(token[2])) {
				token += 2;
				if (!has_d) {
					current_mat.dissolve = 1.f - ParseReal(&token, 0.f);
				}
				continue;
			}

			// ambient texture
			if ((0 == strncmp(token, "map_Ka", 6)) && StringUtil::IsSpace(token[6])) {
				token += 7;
				Texture ambient;
				if (parseTexture(ambient, token)) {
					current_mat.texture_map.insert(std::make_pair(TextureType::AMBIENT, ambient));
				}
				continue;
			}

			// diffuse texture
			if ((0 == strncmp(token, "map_Kd", 6)) && StringUtil::IsSpace(token[6])) {
				token += 7;
				Texture diffuse;
				if (parseTexture(diffuse, token)) {
					current_mat.texture_map.insert(std::make_pair(TextureType::DIFFUSE, diffuse));
				}
				continue;
			}

			// specular texture
			if ((0 == strncmp(token, "map_Ks", 6)) && StringUtil::IsSpace(token[6])) {
				token += 7;
				Texture specular;
				if (parseTexture(specular, token)) {
					current_mat.texture_map.insert(std::make_pair(TextureType::SPECULAR, specular));
				}
				continue;
			}

			// specular highlight texture
			if ((0 == strncmp(token, "map_Ns", 6)) && StringUtil::IsSpace(token[6])) {
				token += 7;
				Texture specular_highlight;
				if (parseTexture(specular_highlight, token)) {
					current_mat.texture_map.insert(std::make_pair(TextureType::SPECULAR_HIGHLIGHT, specular_highlight));
				}
				continue;
			}

			// bump texture
			if (((0 == strncmp(token, "map_bump", 8)) && StringUtil::IsSpace(token[8])) ||
					((0 == strncmp(token, "map_Bump", 8)) && StringUtil::IsSpace(token[8]))) {
				token += 9;
				Texture bump;
				if (parseTexture(bump, token)) {
					bump.option.imfchan = 'l';
					current_mat.texture_map.insert(std::make_pair(TextureType::BUMP, bump));
				}
				continue;
			}

			// another name of bump map texture
			if ((0 == strncmp(token, "bump", 4)) && StringUtil::IsSpace(token[4])) {
				token += 5;
				Texture bump;
				if (parseTexture(bump, token)) {
					bump.option.imfchan = 'l';
					current_mat.texture_map.insert(std::make_pair(TextureType::BUMP, bump));
				}
				continue;
			}

			// alpha texture
			if ((0 == strncmp(token, "map_d", 5)) && StringUtil::IsSpace(token[5])) {
				token += 6;
				Texture alpha;
				if (parseTexture(alpha, token)) {
					current_mat.texture_map.insert(std::make_pair(TextureType::ALPHA, alpha));
				}
				continue;
			}

			// displacement texture
			if ((0 == strncmp(token, "disp", 4)) && StringUtil::IsSpace(token[4])) {
				token += 5;
				Texture displacement;
				if (parseTexture(displacement, token)) {
					current_mat.texture_map.insert(std::make_pair(TextureType::DISPLACEMENT, displacement));
				}
				continue;
			}

			// reflection map
			if ((0 == strncmp(token, "refl", 4)) && StringUtil::IsSpace(token[4])) {
				token += 5;
				Texture reflection;
				if (parseTexture(reflection, token)) {
					current_mat.texture_map.insert(std::make_pair(TextureType::REFLECTION, reflection));
				}
				continue;
			}
		}

		// flush last material
		materialMap.insert(std::make_pair(current_mat.name, static_cast<int>(materials.size())));
		materials.emplace_back(current_mat);

		return true;
	}

	bool WaveFrontObjMesh::ParseMaterial(const std::string& materialName, std::vector<Material>& materials, std::unordered_map<std::string, int>& materialMap) {
		std::ifstream inputStream(materialName);
		if (!inputStream) return false;
		return LoadMaterial(materials, materialMap, inputStream);
	}

	void WaveFrontObjMesh::SplitWithToken(std::vector<std::string>& elems, const char* delims, const char** token) {
		const char* end = (*token) + strcspn((*token), "\n\r");
		size_t offset = end - (*token);
		if (offset != 0) {
			char* dest = (char*)malloc(sizeof(char) * offset + 1);
			strncpy(dest, (*token), offset);
			*(dest + offset) = 0;
			const char* pch = strtok(dest, delims);
			while (pch != nullptr) {
				// trim relative path slash
				// e.g) ./vp.mtl -> vp.mtl
				elems.emplace_back(StringUtil::SplitPair(pch, "\\/").second);
				pch = strtok(nullptr, delims);
			}
			free(dest);
		}
	}

	std::string WaveFrontObjMesh::ParseString(const char **token) const {
		(*token) += strspn((*token), " \t");
		const char* end = (*token) + strcspn((*token), " \t\r");
		size_t offset = end - (*token);
		std::string str;
		if (offset != 0) {
			char* dest = (char*)malloc(sizeof(char) * offset + 1);
			strncpy(dest, (*token), offset);
			*(dest + offset) = 0;
			str.assign(dest);
			free(dest);
		}

		(*token) = end;
		return str;
	}

	float WaveFrontObjMesh::ParseReal(const char **token, float defaultValue) const {
		(*token) += strspn((*token), " \t");
		const char* end = (*token) + strcspn((*token), " \t\r");
		size_t offset = end - (*token);
		float f = defaultValue;
		if (offset != 0) {
			char* dest = (char*)malloc(sizeof(char) * offset + 1);
			strncpy(dest, (*token), offset);
			*(dest + offset) = 0;
			f = (float)atof(dest);
			free(dest);
		}

		(*token) = end;
		return f;
	}

	void WaveFrontObjMesh::ParseReal2(Vector2d &vt, const char **token, float defaultX, float defaultY) const {
		vt.x = ParseReal(token, defaultX);
		vt.y = ParseReal(token, defaultY);
	}

	void WaveFrontObjMesh::ParseReal3(Vector3d &vn, const char **token, float defaultX, float defaultY, float defaultZ) const {
		vn.x = ParseReal(token, defaultX);
		vn.y = ParseReal(token, defaultY);
		vn.z = ParseReal(token, defaultZ);
	}

	int WaveFrontObjMesh::ParseInt(const char **token) const {
		(*token) += strspn((*token), " \t");
		int i = atoi((*token));
		(*token) += strcspn((*token), " \t\r");
		return i;
	}

	bool WaveFrontObjMesh::ParseOnOff(const char **token, bool defaultValue) const {
		(*token) += strspn((*token), " \t");
		const char *end = (*token) + strcspn((*token), " \t\r");

		bool ret = defaultValue;
		if ((0 == strncmp((*token), "on", 2))) {
			ret = true;
		} else if ((0 == strncmp((*token), "off", 3))) {
			ret = false;
		}

		(*token) = end;
		return ret;
	}

	bool WaveFrontObjMesh::FixIndex(int idx, int n, int *ret) const {
		if (!ret || idx == 0) {
			return false;
		}

		if (idx > 0) {
			(*ret) = idx - 1;
		}

		if (idx < 0) {
			(*ret) = n + idx;
		}

		return true;
	}
}  // namespace Theodore