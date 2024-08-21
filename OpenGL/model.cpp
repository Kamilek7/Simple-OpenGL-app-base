#include "model.h"

glModel::glModel(const char* filer, modelImporter* importer)
{
	std::string output = getContents(filer);
	JSON = json::parse(output);

	file = filer;
	data = getData();
	
	importer->loadModel(filer);

	crawlNode(0);
}

void glModel::updatePosition(Shader& shader, glm::vec3 position)
{
	glUniform3f(glGetUniformLocation(shader.program, "additionalPosition"), position.x, position.y, position.z);
}
void glModel::draw(Shader& shader, Camera& camera)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Mesh::Draw(shader, camera, matrices[i]);
	}
}

std::vector<unsigned char> glModel::getData()
{
	std::string bytesText;
	std::string uri = JSON["buffers"][0]["uri"];

	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytesText = getContents((fileDirectory + uri).c_str());

	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
	return data;
}
std::vector<float> glModel::getFloats(json accessor)
{
	std::vector <float> floatVec;

	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int acccBytesOffset = accessor.value("bytesOffset", 0);
	std::string type = accessor["type"];
	
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int vertNum=1;
	if (type == "VEC2") vertNum = 2;
	else if (type == "VEC3") vertNum = 3;
	else if (type == "VEC4") vertNum = 4;

	int begin = byteOffset + acccBytesOffset;
	int len = count * 4 * vertNum;
	for (unsigned int i = begin; i < begin + len; i)
	{
		unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
		float val;
		std::memcpy(&val, bytes, sizeof(float));
		floatVec.push_back(val);
	}
	return floatVec;
}
std::vector<GLuint> glModel::getIndices(json accessor)
{
	std::vector<GLuint> indices;
	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int abo = accessor.value("byteOffset", 0);
	unsigned int type = accessor["componentType"];

	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int offset = bufferView["byteOffset"];

	unsigned int begin = offset + abo;

	if (type == 5125)
	{
		for (unsigned int i = begin; i < offset + abo + count * 4; i+=4)
		{
			unsigned char bytes[] = { data[i], data[i+1], data[i+2], data[i+3] };
			unsigned int val;
			std::memcpy(&val, bytes, sizeof(unsigned int));
			indices.push_back(val);
		}
	}
	else if (type == 5123)
	{
		for (unsigned int i = begin; i < offset + abo + count * 2; i+=2)
		{
			unsigned char bytes[] = { data[i], data[i+1] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (type == 5122)
	{
		for (unsigned int i = begin; i < offset + abo + count * 2; i+=2)
		{
			unsigned char bytes[] = { data[i], data[i+1] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}

	return indices;
 }

std::vector<Texture> glModel::getTextures()
{
	std::vector<Texture> textures;
	std::string fileStr = std::string(file);
	std::string dir = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	int unit = 0;
	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		std::string texPath = JSON["images"][i]["uri"];

		bool check = true;
		for (unsigned int j = 0; j < loadedNames.size(); j++)
		{
			if (loadedNames[j] == texPath)
			{
				textures.push_back(loadedTex[j]);
				check = false;
				break;
			}
		}
		if (check)
		{
			if (texPath.find("baseColor") != std::string::npos)
			{
				Texture diffuse = Texture((dir + texPath).c_str(), "diffuse", unit++);
				textures.push_back(diffuse);
				loadedTex.push_back(diffuse);
				loadedNames.push_back(texPath);
			}
			else if (texPath.find("metallicRoughNess") != std::string::npos)
			{
				Texture spec = Texture((dir + texPath).c_str(), "specular", unit++);
				textures.push_back(spec);
				loadedTex.push_back(spec);
				loadedNames.push_back(texPath);
			}
		}

	}
	return textures;
}

std::vector <glm::vec2> glModel::groupFloatVec2(std::vector<float> floatVec)
{
	std::vector <glm::vec2> vectors;
	for (int i = 0; i < floatVec.size(); i+=2)
		vectors.push_back(glm::vec2(floatVec[i+1], floatVec[i]));
	return vectors;
}
std::vector <glm::vec3> glModel::groupFloatVec3(std::vector<float> floatVec)
{
	std::vector <glm::vec3> vectors;
	for (int i = 0; i < floatVec.size(); i+=3)
		vectors.push_back(glm::vec3(floatVec[i+2], floatVec[i+1] , floatVec[i]));
	return vectors;
}
std::vector <glm::vec4> glModel::groupFloatVec4(std::vector<float> floatVec)
{
	std::vector <glm::vec4> vectors;
	for (int i = 0; i < floatVec.size(); i+=4)
		vectors.push_back(glm::vec4(floatVec[i+3], floatVec[i+2], floatVec[i+1], floatVec[i]));
	return vectors;
}

std::vector<Vertex> glModel::assembleVert(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2>texUV)
{
	std::vector<Vertex> vertices;
	for (unsigned int i = 0; i < positions.size(); i++)
		vertices.push_back(Vertex{ positions[i], normals[i], glm::vec3(1.0f,1.0f,1.0f), texUV[i] });
	return vertices;
}

void glModel::loadMesh(unsigned int indMesh)
{
	unsigned int posInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

	

	std::vector <float> posVec = getFloats(JSON["accessors"][posInd]);
	std::vector <float> normalVec = getFloats(JSON["accessors"][normalInd]);
	std::vector <float> texVec = getFloats(JSON["accessors"][texInd]);

	std::vector<glm::vec3> positions = groupFloatVec3(posVec);
	std::vector<glm::vec3> normals = groupFloatVec3(normalVec);
	std::vector<glm::vec2> texUVs = groupFloatVec2(texVec);

	std::vector<Vertex> vertices = assembleVert(positions, normals, texUVs);
	std::vector<GLuint> indices = getIndices(JSON["accessors"][indInd]);
	std::vector<Texture> textures = getTextures();

	meshes.push_back(Mesh(vertices, indices, textures));
}

void glModel::crawlNode(unsigned int index, glm::mat4 matrix)
{
	json node = JSON["nodes"][index];
	glm::vec3 trans = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		float transval[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
			transval[i] = (node["translation"][i]);
		trans = glm::make_vec3(transval);
	}
	glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		float rotval[4] = { node["rotation"][3], node["rotation"][0], node["rotation"][1], node["rotation"][2] };
		rot = glm::make_quat(rotval);
	}
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		float scaleval[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleval[i] = (node["scale"][i]);
		scale = glm::make_vec3(scaleval);
	}
	glm::mat4 matr = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float mat[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			mat[i] = (node["matrix"][i]);
		matr = glm::make_mat4(mat);
	}

	glm::mat4 t = glm::mat4(1.0f);
	glm::mat4 r = glm::mat4(1.0f);
	glm::mat4 s = glm::mat4(1.0f);

	t = glm::translate(t, trans);
	r = glm::mat4_cast(rot);
	s = glm::scale(s, scale);

	glm::mat4 matNext = matrix * matr * t * r * s;

	if (node.find("mesh") != node.end())
	{
		tranlations.push_back(trans);
		rotations.push_back(rot);
		scales.push_back(scale);
		matrices.push_back(matNext);

		loadMesh(node["mesh"]);
	}
	if (node.find("children") != node.end())
	{
		for (unsigned int i = 0; i < node["children"].size(); i++)
			crawlNode(node["children"][i], matNext);
	}
}
