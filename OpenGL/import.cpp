#include "import.h"

#include <iostream>

void modelImporter::loadModel(const char* file)
{
	m_pScene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

}
