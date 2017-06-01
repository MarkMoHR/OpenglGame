#pragma once

#ifndef MODEL_H  
#define MODEL_H 

#include <fstream>
#include <iostream>
#include <string.h>
#include <map>

#include <gl/freeglut.h>
#include <IL/il.h>

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


using namespace std;

class Model {
public:
	Model();
	~Model();

	bool importModel(const string& pFile);        //加载模型

	void renderTheModel(float scale);             //渲染模型入口

private:
	//加载texture
	bool loadTextures(const aiScene* scene, const string modelPath);

	//递归渲染模型
	void recursiveRender(const struct aiScene *sc, const struct aiNode* nd, float scale);

	//给模型添加texture
	void applyMaterial(const aiMaterial *mtl);

private:
	const aiScene* scene = NULL;
	GLuint scene_list = 0;
	aiVector3D scene_min, scene_max, scene_center;

	map<string, GLuint*> textureIdMap;    // map image filenames to textureIds
	GLuint* textureIds;					  // pointer to texture Array

};

#endif

