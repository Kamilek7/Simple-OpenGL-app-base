#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <vector>

class EBO
{
public:
	GLuint buffer;
	EBO(std::vector<GLuint>& indices);

	void bind();
	void unbind();
	void remove();
};

#endif