#include "object.h"

Object::Object(const char* name)
{
	position = { 0.0,0.0,0.0 };
	time = 0.0;
	model = Model(name);
}

void Object::process(float dt, Shader& shader, Camera &camera)
{
	time += dt;
	position.x = sin(time);
	model.updatePosition(shader, position);
	model.draw(shader, camera);
}
