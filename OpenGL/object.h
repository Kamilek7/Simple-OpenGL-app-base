#ifndef OBJECT_H
#define OBJECT_H

#include "model.h"
class Object
{
	float time;
	Model model;
	glm::vec3 position;

public:
	Object(const char* name);
	void process(float dt, Shader& shader, Camera& camera);
};
#endif
