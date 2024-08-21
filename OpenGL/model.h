#ifndef MODEL_H
#define MODEL_H

#include "import.h"


class glModel
{
public:
	glModel() {};
	glModel(const char* filer, modelImporter* importer);
	void updatePosition(Shader& shader, glm::vec3 position);
	void draw(Shader& shader, Camera& camera);
private:
	std::vector<Mesh> meshes;

};
#endif