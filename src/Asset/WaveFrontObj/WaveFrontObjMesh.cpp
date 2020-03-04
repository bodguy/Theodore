// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "WaveFrontObjMesh.h"

#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <cassert>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <type_traits>

#include "Helper/Debug.h"
#include "Platform/Time.h"

namespace Theodore {
	struct Scene {
		Scene() : meshes(), materials(), base_dir() {
			meshes.clear();
			materials.clear();
		}
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
		std::string base_dir;
	};

	inline bool parsePrimitive(Mesh& mesh, const PrimitiveGroup& primitive, MeshParseOption option, const int material_id,
														 const std::vector<vec3>& verts, const std::vector<vec2>& texcoords, const std::vector<vec3>& normals,
														 const std::string& name, const std::string& default_name) {
		if (primitive.is_empty()) {
			return false;
		}
		mesh.name = name.empty() ? default_name : name;

		// make polygon
		unsigned int count = 0;
		for (const Face& face : primitive.faces) {
			size_t npolys = face.vertex_indices.size();

			if (npolys < 3) {
				// face must have at least 3+ vertices.
				continue;
			}

			// triangulate only parsing flag is set and polygon has more than 3.
			if ((option & MeshParseOption::TRIANGULATE) && npolys != 3) {
				triangulate(mesh, verts, npolys);
			} else {
				for (size_t f = 0; f < npolys; f++) {
					Vertex vtx;
					VertexIndex idx = face.vertex_indices[f];
					vtx.position = verts[idx.v_idx];
					vtx.texcoord = (idx.vt_idx == -1 ? vec2() : texcoords[idx.vt_idx]);
					vtx.normal = (idx.vn_idx == -1 ? vec3() : normals[idx.vn_idx]);
					mesh.vertices.emplace_back(vtx);
				}

				if ((option & MeshParseOption::CALC_TANGENT) && npolys == 3) {
					calcTangent(mesh, count);
				}

				auto preCompute = (unsigned int)((mesh.vertices.size()) - npolys);
				for (size_t ff = 0; ff < npolys; ff++) {
					mesh.indices.emplace_back(preCompute + ff);
				}
				mesh.material_id = material_id;
				count++;
			}
		}

		return true;
	}

	inline bool parseIndices(const char** token, int vsize, int vnsize, int vtsize, VertexIndex* ret) {
		if (!ret) {
			return false;
		}

		VertexIndex vi(-1);
		// i
		if (!fixIndex(atoi((*token)), vsize, &(vi.v_idx))) {
			return false;
		}
		(*token) += strcspn((*token), "/ \t\r"); // go to next slash
		// check if only have vertex
		if ((*token)[0] != '/') {
			(*ret) = vi;
			return true;
		}
		(*token)++;

		// i//k
		//   +--- here
		if ((*token)[0] == '/') {
			(*token)++; // now then, token is pointing at 'k'
			if (!fixIndex(atoi((*token)), vnsize, &(vi.vn_idx))) {
				return false;
			}
			(*token) += strcspn((*token), "/ \t\r"); // go to next slash (although, it's not exist)
			(*ret) = vi;
			return true;
		}

		// i/j/k or i/j
		//   +--- here
		if (!fixIndex(atoi((*token)), vtsize, &(vi.vt_idx))) {
			return false;
		}
		(*token) += strcspn((*token), "/ \t\r"); // go to next slash
		if ((*token)[0] != '/') {
			// it's i/j case
			(*ret) = vi;
			return true;
		}

		// process last case
		// i/j/k
		(*token)++; // now then, token is pointing at 'k'
		if (!fixIndex(atoi((*token)), vnsize, &(vi.vn_idx))) {
			return false;
		}
		(*token) += strcspn((*token), "/ \t\r"); // go to next slash (although, it's not exist)
		(*ret) = vi;

		return true;
	}


	inline void split(std::vector<std::string>& elems, const char* delims, const char** token) {
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
				elems.emplace_back(splitDelims(pch, "\\/").second);
				pch = strtok(nullptr, delims);
			}
			free(dest);
		}
	}

	inline TextureFace parseTextureFace(const char** token, TextureFace default_value) {
		(*token) += strspn((*token), " \t");
		const char *end = (*token) + strcspn((*token), " \t\r");
		TextureFace tft = default_value;

		if ((0 == strncmp((*token), "cube_top", 8))) {
			tft = TextureFace::TEX_3D_CUBE_TOP;
		} else if ((0 == strncmp((*token), "cube_bottom", 11))) {
			tft = TextureFace::TEX_3D_CUBE_BOTTOM;
		} else if ((0 == strncmp((*token), "cube_left", 9))) {
			tft = TextureFace::TEX_3D_CUBE_LEFT;
		} else if ((0 == strncmp((*token), "cube_right", 10))) {
			tft = TextureFace::TEX_3D_CUBE_RIGHT;
		} else if ((0 == strncmp((*token), "cube_front", 10))) {
			tft = TextureFace::TEX_3D_CUBE_FRONT;
		} else if ((0 == strncmp((*token), "cube_back", 9))) {
			tft = TextureFace::TEX_3D_CUBE_BACK;
		} else if ((0 == strncmp((*token), "sphere", 6))) {
			tft = TextureFace::TEX_3D_SPHERE;
		}

		(*token) = end;
		return tft;
	}

	inline bool parseTexture(Texture& tex, const char* token) {
		while (!is_new_line((*token))) {
			token += strspn(token, " \t");  // skip space
			if ((0 == strncmp(token, "-clamp", 6)) && is_space((token[6]))) {
				token += 7;
				tex.option.clamp = parseOnOff(&token, true);
			} else if ((0 == strncmp(token, "-blendu", 7)) && is_space((token[7]))) {
				token += 8;
				tex.option.blendu = parseOnOff(&token, true);
			} else if ((0 == strncmp(token, "-blendv", 7)) && is_space((token[7]))) {
				token += 8;
				tex.option.blendv = parseOnOff(&token, true);
			} else if ((0 == strncmp(token, "-bm", 3)) && is_space((token[3]))) {
				token += 4;
				tex.option.bump_multiplier = parseReal(&token, 1.f);
			} else if ((0 == strncmp(token, "-boost", 6)) && is_space((token[6]))) {
				token += 7;
				tex.option.sharpness = parseReal(&token, 1.f);
			} else if ((0 == strncmp(token, "-mm", 3)) && is_space((token[3]))) {
				token += 4;
				tex.option.brightness = parseReal(&token, 0.f);
				tex.option.contrast = parseReal(&token, 1.f);
			} else if ((0 == strncmp(token, "-o", 2)) && is_space((token[2]))) {
				token += 3;
				parseReal3(tex.option.origin_offset, &token);
			} else if ((0 == strncmp(token, "-s", 2)) && is_space((token[2]))) {
				token += 3;
				parseReal3(tex.option.scale, &token, 1.f, 1.f, 1.f);
			} else if ((0 == strncmp(token, "-t", 2)) && is_space((token[2]))) {
				token += 3;
				parseReal3(tex.option.turbulence, &token);
			} else if ((0 == strncmp(token, "-imfchan", 8)) && is_space((token[8]))) {
				token += 9;
				token += strspn(token, " \t");
				const char *end = token + strcspn(token, " \t\r");
				if ((end - token) == 1) {  // Assume one char for -imfchan
					tex.option.imfchan = (*token);
				}
				token = end;
			} else if ((0 == strncmp(token, "-type", 5)) && is_space((token[5]))) {
				token += 5;
				tex.option.face_type = parseTextureFace(&token, TextureFace::TEX_2D);
			} else {
				// parse texture name at last.
				tex.name = parseString(&token);
				if (tex.name.empty()) {
					return false;
				}
			}
		}

		return true;
	}

	inline bool loadMtl(std::vector<Material>& materials, std::unordered_map<std::string, int>& material_map, std::istream& ifs) {
		Material current_mat;
		bool has_d = false;
		std::string line_buf;

		// preventing a empty file
		while(ifs.peek() != -1) {
			getLine(ifs, line_buf);

			// Trim trailing whitespace.
			if (!line_buf.empty()) {
				line_buf = line_buf.substr(0, line_buf.find_last_not_of(" \t") + 1);
			}

			// Trim newline '\r\n' or '\n'
			if (!line_buf.empty()) {
				if (line_buf[line_buf.size() - 1] == '\n')
					line_buf.erase(line_buf.size() - 1);
			}
			if (!line_buf.empty()) {
				if (line_buf[line_buf.size() - 1] == '\r')
					line_buf.erase(line_buf.size() - 1);
			}

			// Skip if empty line.
			if (line_buf.empty()) {
				continue;
			}

			// Skip leading space.
			const char *token = line_buf.c_str(); // read only token
			token += strspn(token, " \t");

			if (token == nullptr) return false;
			if (token[0] == '\0') continue;  // empty line
			if (token[0] == '#') continue;  // comment line

			// new mtl
			if ((0 == strncmp(token, "newmtl", 6)) && is_space((token[6]))) {
				// save previous material
				if (!current_mat.name.empty()) {
					material_map.insert(std::make_pair(current_mat.name, static_cast<int>(materials.size())));
					materials.emplace_back(current_mat);
				}

				// reset material
				current_mat = Material();
				has_d = false;

				// parse new mat name
				token += 7;
				current_mat.name = parseString(&token);
				continue;
			}

			// ambient
			if (token[0] == 'K' && token[1] == 'a' && is_space((token[2]))) {
				token += 2;
				vec3 ambient;
				parseReal3(ambient, &token);
				current_mat.ambient = ambient;
				continue;
			}

			// diffuse
			if (token[0] == 'K' && token[1] == 'd' && is_space((token[2]))) {
				token += 2;
				vec3 diffuse;
				parseReal3(diffuse, &token);
				current_mat.diffuse = diffuse;
				continue;
			}

			// specular
			if (token[0] == 'K' && token[1] == 's' && is_space((token[2]))) {
				token += 2;
				vec3 specular;
				parseReal3(specular, &token);
				current_mat.specular = specular;
				continue;
			}

			// transmittance
			if ((token[0] == 'K' && token[1] == 't' && is_space((token[2]))) ||
					(token[0] == 'T' && token[1] == 'f' && is_space((token[2])))) {
				token += 2;
				vec3 transmittance;
				parseReal3(transmittance, &token);
				current_mat.transmittance = transmittance;
				continue;
			}

			// ior(index of refraction)
			if (token[0] == 'N' && token[1] == 'i' && is_space((token[2]))) {
				token += 2;
				current_mat.ior = parseReal(&token, 0.f);
				continue;
			}

			// emission
			if (token[0] == 'K' && token[1] == 'e' && is_space(token[2])) {
				token += 2;
				vec3 emission;
				parseReal3(emission, &token);
				current_mat.emission = emission;
				continue;
			}

			// shininess
			if (token[0] == 'N' && token[1] == 's' && is_space(token[2])) {
				token += 2;
				current_mat.shininess = parseReal(&token, 0.f);
				continue;
			}

			// illum model
			if (0 == strncmp(token, "illum", 5) && is_space(token[5])) {
				token += 6;
				current_mat.illum = parseInt(&token);
				continue;
			}

			// dissolve (the non-transparency of the material), The default is 1.0 (not transparent at all)
			if ((token[0] == 'd' && is_space(token[1]))) {
				token += 1;
				current_mat.dissolve = parseReal(&token, 1.f);
				has_d = true;
				continue;
			}

			// dissolve (the transparency of the material): 1.0 - Tr, The default is 0.0 (not transparent at all)
			if (token[0] == 'T' && token[1] == 'r' && is_space(token[2])) {
				token += 2;
				if (!has_d) {
					current_mat.dissolve = 1.f - parseReal(&token, 0.f);
				}
				continue;
			}

			// ambient texture
			if ((0 == strncmp(token, "map_Ka", 6)) && is_space(token[6])) {
				token += 7;
				Texture ambient;
				if (parseTexture(ambient, token)) {
					current_mat.texture_map.insert(std::make_pair(TextureType::AMBIENT, ambient));
				}
				continue;
			}

			// diffuse texture
			if ((0 == strncmp(token, "map_Kd", 6)) && is_space(token[6])) {
				token += 7;
				Texture diffuse;
				if (parseTexture(diffuse, token)) {
					current_mat.texture_map.insert(std::make_pair(TextureType::DIFFUSE, diffuse));
				}
				continue;
			}

			// specular texture
			if ((0 == strncmp(token, "map_Ks", 6)) && is_space(token[6])) {
				token += 7;
				Texture specular;
				if (parseTexture(specular, token)) {
					current_mat.texture_map.insert(std::make_pair(TextureType::SPECULAR, specular));
				}
				continue;
			}

			// specular highlight texture
			if ((0 == strncmp(token, "map_Ns", 6)) && is_space(token[6])) {
				token += 7;
				Texture specular_highlight;
				if (parseTexture(specular_highlight, token)) {
					current_mat.texture_map.insert(std::make_pair(TextureType::SPECULAR_HIGHLIGHT, specular_highlight));
				}
				continue;
			}

			// bump texture
			if (((0 == strncmp(token, "map_bump", 8)) && is_space(token[8])) ||
					((0 == strncmp(token, "map_Bump", 8)) && is_space(token[8]))) {
				token += 9;
				Texture bump;
				if (parseTexture(bump, token)) {
					bump.option.imfchan = 'l';
					current_mat.texture_map.insert(std::make_pair(TextureType::BUMP, bump));
				}
				continue;
			}

			// another name of bump map texture
			if ((0 == strncmp(token, "bump", 4)) && is_space(token[4])) {
				token += 5;
				Texture bump;
				if (parseTexture(bump, token)) {
					bump.option.imfchan = 'l';
					current_mat.texture_map.insert(std::make_pair(TextureType::BUMP, bump));
				}
				continue;
			}

			// alpha texture
			if ((0 == strncmp(token, "map_d", 5)) && is_space(token[5])) {
				token += 6;
				Texture alpha;
				if (parseTexture(alpha, token)) {
					current_mat.texture_map.insert(std::make_pair(TextureType::ALPHA, alpha));
				}
				continue;
			}

			// displacement texture
			if ((0 == strncmp(token, "disp", 4)) && is_space(token[4])) {
				token += 5;
				Texture displacement;
				if (parseTexture(displacement, token)) {
					current_mat.texture_map.insert(std::make_pair(TextureType::DISPLACEMENT, displacement));
				}
				continue;
			}

			// reflection map
			if ((0 == strncmp(token, "refl", 4)) && is_space(token[4])) {
				token += 5;
				Texture reflection;
				if (parseTexture(reflection, token)) {
					current_mat.texture_map.insert(std::make_pair(TextureType::REFLECTION, reflection));
				}
				continue;
			}
		}

		// flush last material
		material_map.insert(std::make_pair(current_mat.name, static_cast<int>(materials.size())));
		materials.emplace_back(current_mat);

		return true;
	}

	inline bool parseMtl(const std::string& mtl_dir, std::vector<Material>& materials, std::unordered_map<std::string, int>& material_map) {
		std::ifstream ifs(mtl_dir);
		if (!ifs) {
			return false;
		}
		loadMtl(materials, material_map, ifs);
		return true;
	}

	bool loadObj(const std::string& path, Scene& scene, MeshParseOption parse_option) {
		if (!endsWith(path, ".obj")) {
			return false;
		}

		std::ifstream ifs(path);
		if(!ifs) {
			return false;
		}

		std::vector<vec3> vertices;
		std::vector<vec2> texcoords;
		std::vector<vec3> normals;
		std::unordered_map<std::string, int> material_map;
		PrimitiveGroup current_prim;
		std::string current_object_name;
		std::string current_material_name;
		Mesh current_mesh;
		int current_material_id = -1;
		std::pair<std::string, std::string> pair = splitDelims(path, "\\/");
		scene.base_dir = pair.first;
		std::string filename = pair.second;
		std::string line_buf;

		// preventing a empty file
		while(ifs.peek() != -1) {
			getLine(ifs, line_buf);

			// Trim newline '\r\n' or '\n'
			if (!line_buf.empty()) {
				if (line_buf[line_buf.size() - 1] == '\n')
					line_buf.erase(line_buf.size() - 1);
			}
			if (!line_buf.empty()) {
				if (line_buf[line_buf.size() - 1] == '\r')
					line_buf.erase(line_buf.size() - 1);
			}

			// Skip if empty line.
			if (line_buf.empty()) {
				continue;
			}

			// Skip leading space.
			const char *token = line_buf.c_str(); // read only token
			token += strspn(token, " \t");

			if (token == nullptr) return false;
			if (token[0] == '\0') continue;  // empty line
			if (token[0] == '#') continue;  // comment line

			// vertex
			if (token[0] == 'v' && is_space((token[1]))) {
				token += 2;
				vec3 v;
				parseReal3(v, &token);
				vertices.emplace_back(v);
				continue;
			}

			// normal
			if (token[0] == 'v' && token[1] == 'n' && is_space((token[2]))) {
				token += 3;
				vec3 vn;
				parseReal3(vn, &token);
				normals.emplace_back(vn);
				continue;
			}

			// texcoord
			if (token[0] == 'v' && token[1] == 't' && is_space((token[2]))) {
				token += 3;
				vec2 vt;
				parseReal2(vt, &token);
				if (parse_option & MeshParseOption::FLIP_UV) {
					vt.y = 1.f - vt.y;
				}
				texcoords.emplace_back(vt);
				continue;
			}

			// face
			if (token[0] == 'f' && is_space((token[1]))) {
				token += 2;
				token += strspn(token, " \t"); // Skip leading space.

				Face f;
				f.vertex_indices.reserve(3);

				while (!is_new_line(token[0])) {
					VertexIndex vi;
					if (!parseIndices(&token, vertices.size(), normals.size(), texcoords.size(), &vi)) {
						return false;
					}

					// finish parse indices
					f.vertex_indices.emplace_back(vi);
					token += strspn(token, " \t\r"); // skip space
				}

				current_prim.faces.emplace_back(f);
				continue;
			}

			// use mtl
			if ((0 == strncmp(token, "usemtl", 6)) && is_space((token[6]))) {
				token += 7;
				std::string new_material_name = parseString(&token);
				int new_material_id = -1;
				// find material id
				if (material_map.find(new_material_name) != material_map.end()) {
					new_material_id = material_map[new_material_name];
				}

				// check current material and previous
				if (new_material_name != current_material_name) {
					// when current object name is empty, then assign current material name as alternatives.
					if (current_object_name.empty()) {
						current_object_name = new_material_name;
					}
					parsePrimitive(current_mesh, current_prim, parse_option, current_material_id, vertices, texcoords, normals, current_object_name, filename); // return value not used
					if (!current_mesh.vertices.empty()) {
						scene.meshes.emplace_back(current_mesh);
						// when successfully push a new mesh, then cache current material name.
						current_object_name = new_material_name;
					}
					// reset
					current_prim = PrimitiveGroup();
					current_mesh = Mesh();
					// cache new material id
					current_material_id = new_material_id;
					current_material_name = new_material_name;
				}
				continue;
			}

			// load mtl
			if ((0 == strncmp(token, "mtllib", 6)) && is_space((token[6]))) {
				token += 7;
				std::vector<std::string> mtl_file_names;
				// parse multiple mtl filenames split by whitespace
				split(mtl_file_names, " ", &token);
				// load just one available mtl file in the list
				for (std::string& name : mtl_file_names) {
					if (parseMtl(scene.base_dir + name, scene.materials, material_map)) {
						break;
					}
				}
				continue;
			}

			// group name
			if (token[0] == 'g' && is_space((token[1]))) {
				parsePrimitive(current_mesh, current_prim, parse_option, current_material_id, vertices, texcoords, normals, current_object_name, filename); // return value not used
				if (!current_mesh.vertices.empty()) {
					scene.meshes.emplace_back(current_mesh);
					current_object_name = "";
				}

				// reset
				current_prim = PrimitiveGroup();
				current_mesh = Mesh();

				token += 2;

				// assemble multi group name
				std::vector<std::string> names;
				while (!is_new_line(token[0])) {
					names.emplace_back(parseString(&token));
					token += strspn(token, " \t\r"); // skip space
				}

				if (!names.empty()) {
					std::stringstream ss;
					std::vector<std::string>::const_iterator it = names.begin();
					ss << *it++;
					for (; it != names.end(); it++) {
						ss << " " << *it;
					}
					current_object_name = ss.str();
				}

				continue;
			}

			// object name
			if (token[0] == 'o' && is_space((token[1]))) {
				parsePrimitive(current_mesh, current_prim, parse_option, current_material_id, vertices, texcoords, normals, current_object_name, filename); // return value not used
				if (!current_mesh.vertices.empty()) {
					scene.meshes.emplace_back(current_mesh);
					current_object_name = "";
				}

				// reset
				current_prim = PrimitiveGroup();
				current_mesh = Mesh();

				token += 2;
				current_object_name = parseString(&token);
				continue;
			}
		}

		bool ret = parsePrimitive(current_mesh, current_prim, parse_option, current_material_id, vertices, texcoords, normals, current_object_name, filename);
		if (ret || !current_mesh.vertices.empty()) {
			scene.meshes.emplace_back(current_mesh);
		}

		return true;
	}

}  // namespace Theodore