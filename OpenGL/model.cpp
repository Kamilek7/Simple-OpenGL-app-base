#include "model.h"

glModel::glModel(const char* filer, modelImporter* importer)
{
	importer->loadModel(filer);
	meshes = importer->getMeshes();
	importer->clear();
}

void glModel::updatePosition(Shader& shader, glm::vec3 position)
{
	glUniform3f(glGetUniformLocation(shader.program, "additionalPosition"), position.x, position.y, position.z);
}
void glModel::draw(Shader& shader, Camera& camera)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Mesh::Draw(shader, camera);
	}
}
