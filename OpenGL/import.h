#ifndef IMPORTERERR_H
#define IMPORTERERR_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class modelImporter
{
	const aiScene* m_pScene;
	Assimp::Importer importer;

public:
	modelImporter() {};
	void loadModel(const char* file);
};

#endif // !1

