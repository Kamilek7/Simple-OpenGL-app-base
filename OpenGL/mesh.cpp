#include "mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;
	vao.bind();

	VBO VBO(vertices);
	EBO EBO(indices);

	vao.linkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.linkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao.linkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	vao.linkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	vao.unbind();
	VBO.unbind();
	EBO.unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera, glm::mat4 matrix, glm::vec3 trans, glm::quat rot, glm::vec3 scale)
{
	shader.on();
	vao.bind();
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].bind();
	}
	glUniform3f(glGetUniformLocation(shader.program, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.matrix(shader, "camMatrix");
	glm::mat4 t = glm::mat4(1.0f);
	glm::mat4 r = glm::mat4(1.0f);
	glm::mat4 s = glm::mat4(1.0f);

	t = glm::translate(t, trans);
	r = glm::mat4_cast(rot);
	s = glm::scale(s, scale);

	glUniformMatrix4fv(glGetUniformLocation(shader.program, "translation"), 1, GL_FALSE, glm::value_ptr(t));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "rotation"), 1, GL_FALSE, glm::value_ptr(r));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "scale"), 1, GL_FALSE, glm::value_ptr(s));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
