#ifndef MODEL_H
#define MODEL_H

#include <json/json.hpp>
#include "mesh.h"
#include "import.h"

using json = nlohmann::json;

class glModel
{
public:
	glModel() {};
	glModel(const char* filer, modelImporter* importer);
	void updatePosition(Shader& shader, glm::vec3 position);
	void draw(Shader& shader, Camera& camera);
private:
	const char* file;
	std::vector<unsigned char> data;
	
	json JSON;

	std::vector<Mesh> meshes;
	std::vector<glm::vec3> tranlations;
	std::vector<glm::quat> rotations;
	std::vector<glm::vec3> scales;
	std::vector<glm::mat4> matrices;

	std::vector <std::string> loadedNames;
	std::vector <Texture> loadedTex;

	std::vector <unsigned char> getData();
	std::vector <float> getFloats(json accessor);
	std::vector <GLuint> getIndices(json accessor);
	std::vector <Texture> getTextures();

	std::vector<glm::vec2> groupFloatVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatVec4(std::vector<float> floatVec);

	std::vector<Vertex> assembleVert(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUV);

	void loadMesh(unsigned int indMesh);

	void crawlNode(unsigned int index, glm::mat4 matrix = glm::mat4(1.0f));

};
#endif