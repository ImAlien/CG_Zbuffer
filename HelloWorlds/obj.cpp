

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
//#define TINYOBJLOADER_USE_MAPBOX_EARCUT

#include "obj.h"
#include "tiny_obj_loader.h"
#include <algorithm>
using namespace std;
void Obj::loadFile(string filename) {
	int n = filename.size();
	int idx = -1;
	for (int i = n - 1; i >= 0; i--) {
		if (filename[i] == '/') {
			idx = i; break;
		}
	}
	std::string pathname = "./";
	if (idx > 0) {
		pathname = filename.substr(0, idx);
	}
	loadFile(pathname, filename.substr(idx + 1));
}
void Obj::loadFile(string filepath, string filename) {
	string inputfile = filename;
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = filepath; // Path to material files

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(inputfile, reader_config)) {
		if (!reader.Error().empty()) {
			cerr << "TinyObjReader: " << reader.Error();
		}
		//exit(1);
	}

	if (!reader.Warning().empty()) {
		cout << "TinyObjReader: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();
	//cout << shapes.size() << endl;
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		int faces_nums = shapes[s].mesh.num_face_vertices.size();
		cout << faces_nums << endl;
		for (size_t f = 0; f < faces_nums; f++) {
			Face cur_face;
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				double vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				double vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				double vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
				cur_face.points.push_back(Point(vx, vy, vz));
				cur_face.y_max = max(cur_face.y_max, vy);
				cur_face.y_min = min(cur_face.y_min, vy);
				g_xmax = max(g_xmax, vx);
				g_xmin = min(g_xmin, vx);
				g_ymin = min(g_ymin, vy);
				g_ymax = max(g_ymax, vy);
				// Check if `normal_index` is zero or positive. negative = no normal data
				if (idx.normal_index >= 0) {
					tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
					tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
					tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
				}

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				if (idx.texcoord_index >= 0) {
					tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}

				// Optional: vertex colors
				// tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
				// tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
				// tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
			}
			index_offset += fv;
			faces.push_back(cur_face);
			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}
}