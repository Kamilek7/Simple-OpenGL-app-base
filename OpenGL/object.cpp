#include "object.h"

ingameObject::ingameObject(const char* name, modelImporter* importer)
{
	position = { 0.0,0.0,0.0 };
	time = 0.0;
	model = glModel(name, importer);
}

void ingameObject::process(float dt, Shader& shader, Camera &camera)
{
	time += dt;
	position.x = sin(time);
	model.updatePosition(shader, position);
	model.draw(shader, camera);
}
