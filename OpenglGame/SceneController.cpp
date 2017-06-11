#define _CRT_SECURE_NO_WARNINGS

#include "SceneController.h"
#include "ParticleSystem.h"
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

vector<glm::vec3> boxPosition;
vector<glm::vec3> boxScale;
static int boxSum = 0;

vector<Model*> breadSet;
vector<ParticleSystem> breadEatenEffectSet;
vector<bool> isBreadEatenSet;
static int eatenBreadNum = 0;
static int closeToBreadIndex = -1;

static float angle = 0.0f;

GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, SkyboxSize / 2.f, 0.f, 1.0f };
GLfloat LightPosition2[] = { -SkyboxSize / 2.f, -SkyboxSize / 2.f, SkyboxSize / 2.f, 1.0f };
GLfloat LightPosition3[] = { SkyboxSize / 2.f, -SkyboxSize / 2.f, SkyboxSize / 2.f, 1.0f };
GLfloat LightPosition4[] = { 0.0f, -SkyboxSize / 2.f, -SkyboxSize / 2.f, 1.0f };

void drawRect(GLuint texture) {
	glEnable(GL_TEXTURE_2D);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);    //天空盒加环境光
	glEnable(GL_COLOR_MATERIAL);

	glBindTexture(GL_TEXTURE_2D, texture);  //选择纹理texture[status]   
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat point[4][2] = { { x1,y1 },{ x1,y2 },{ x2,y2 },{ x2,y1 } };
	int dir[4][2] = { { 1,0 },{ 1,1 },{ 0,1 },{ 0,0 } };
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++) {
		glTexCoord2iv(dir[i]);
		glVertex2fv(point[i]);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
}

struct Vertex {
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 texC;
	glm::vec3 tangent;
	glm::vec3 bitangent;

	Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t) {
		pos = p;	norm = n;	texC = t;
	}
};

GLuint CubeVAO, CubeVBO, CubeEBO;
vector<Vertex> CubeVertices;
vector<unsigned int> CubeIndices = {
	0, 1, 2, 0, 2 ,3,		//Front
	4, 5, 6, 4, 6, 7,		//Right
	8, 9, 10, 8 ,10, 11,	//Back
	12, 13, 14, 12, 14, 15,	//Left
	16, 17, 18, 16, 18, 19,	//Top
	20, 21, 22, 20, 22, 23	//Bottom
};

void initCube(Shader shader) {

	const GLfloat x = 0.5;
	const GLfloat y = 0.5;
	const GLfloat z = 0.5;

	//Front
	CubeVertices.push_back(Vertex(glm::vec3(x, y, z), glm::vec3(0, 0, 1), glm::vec2(1, 0)));
	CubeVertices.push_back(Vertex(glm::vec3(x, -y, z), glm::vec3(0, 0, 1), glm::vec2(1, 1)));
	CubeVertices.push_back(Vertex(glm::vec3(-x, -y, z), glm::vec3(0, 0, 1), glm::vec2(0, 1)));
	CubeVertices.push_back(Vertex(glm::vec3(-x, y, z), glm::vec3(0, 0, 1), glm::vec2(0, 0)));

	//Right
	CubeVertices.push_back(Vertex(glm::vec3(x, y, z), glm::vec3(1, 0, 0), glm::vec2(0, 0)));
	CubeVertices.push_back(Vertex(glm::vec3(x, y, -z), glm::vec3(1, 0, 0), glm::vec2(1, 0)));
	CubeVertices.push_back(Vertex(glm::vec3(x, -y, -z), glm::vec3(1, 0, 0), glm::vec2(1, 1)));
	CubeVertices.push_back(Vertex(glm::vec3(x, -y, z), glm::vec3(1, 0, 0), glm::vec2(0, 1)));
	
	//Top
	CubeVertices.push_back(Vertex(glm::vec3(x, y, z), glm::vec3(0, 1, 0), glm::vec2(0, 0)));
	CubeVertices.push_back(Vertex(glm::vec3(-x, y, z), glm::vec3(0, 1, 0), glm::vec2(0, 1)));
	CubeVertices.push_back(Vertex(glm::vec3(-x, y, -z), glm::vec3(0, 1, 0), glm::vec2(1, 1)));
	CubeVertices.push_back(Vertex(glm::vec3(x, y, -z), glm::vec3(0, 1, 0), glm::vec2(1, 0)));

	//Back
	CubeVertices.push_back(Vertex(glm::vec3(-x, -y, -z), glm::vec3(0, 0, -1), glm::vec2(1, 0)));
	CubeVertices.push_back(Vertex(glm::vec3(x, -y, -z), glm::vec3(0, 0, -1), glm::vec2(0, 0)));
	CubeVertices.push_back(Vertex(glm::vec3(x, y, -z), glm::vec3(0, 0, -1), glm::vec2(0, 1)));
	CubeVertices.push_back(Vertex(glm::vec3(-x, y, -z), glm::vec3(0, 0, -1), glm::vec2(1, 1)));
	
	//Left
	CubeVertices.push_back(Vertex(glm::vec3(-x, -y, -z), glm::vec3(-1, 0, 0), glm::vec2(0, 0)));
	CubeVertices.push_back(Vertex(glm::vec3(-x, y, -z), glm::vec3(-1, 0, 0), glm::vec2(0, 1)));
	CubeVertices.push_back(Vertex(glm::vec3(-x, y, z), glm::vec3(-1, 0, 0), glm::vec2(1, 1)));
	CubeVertices.push_back(Vertex(glm::vec3(-x, -y, z), glm::vec3(-1, 0, 0), glm::vec2(1, 0)));

	//Bottom
	CubeVertices.push_back(Vertex(glm::vec3(-x, -y, -z), glm::vec3(0, -1, 0), glm::vec2(0, 0)));
	CubeVertices.push_back(Vertex(glm::vec3(-x, -y, z), glm::vec3(0, -1, 0), glm::vec2(1, 0)));
	CubeVertices.push_back(Vertex(glm::vec3(x, -y, z), glm::vec3(0, -1, 0), glm::vec2(1, 1)));
	CubeVertices.push_back(Vertex(glm::vec3(x, -y, -z), glm::vec3(0, -1, 0), glm::vec2(0, 1)));

	//Tangent space stuff
	for (int i = 0; i < CubeIndices.size(); i += 3) {
		Vertex& a = CubeVertices[CubeIndices[i]];
		Vertex& b = CubeVertices[CubeIndices[i + 1]];
		Vertex& c = CubeVertices[CubeIndices[i + 2]];

		glm::vec3 edge1 = b.pos - a.pos;
		glm::vec3 edge2 = c.pos - a.pos;
		glm::vec2 deltaUV1 = b.texC - a.texC;
		glm::vec2 deltaUV2 = c.texC - a.texC;

		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent, bitangent;

		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = glm::normalize(tangent);

		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent = glm::normalize(bitangent);

		a.tangent = tangent;
		b.tangent = tangent;
		c.tangent = tangent;

		a.bitangent = bitangent;
		b.bitangent = bitangent;
		c.bitangent = bitangent;
	}

	glGenVertexArrays(1, &CubeVAO);
	glGenBuffers(1, &CubeVBO);
	glGenBuffers(1, &CubeEBO);

	glBindVertexArray(CubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * CubeVertices.size() , &CubeVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * CubeIndices.size(), &CubeIndices[0], GL_STATIC_DRAW);

	//Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//Normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));

	//Tex2D
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texC));

	//Tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	//Bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	glBindVertexArray(0);
}

void drawCube(Shader shader, GLuint diffuse, GLuint bump, GLuint spec) {

	//Textures
	glActiveTexture(GL_TEXTURE0);	//0th texture unit
	glUniform1i(glGetUniformLocation(shader.Program, "diffuse_map"), 0);	//Bind "diffuse_map" in shader to 0th texture unit.
	glBindTexture(GL_TEXTURE_2D, diffuse);	//Bind texture to texture unit.

	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(shader.Program, "bump_map"), 1);
	glBindTexture(GL_TEXTURE_2D, bump);

	glActiveTexture(GL_TEXTURE2);
	glUniform1i(glGetUniformLocation(shader.Program, "spec_map"), 2);
	glBindTexture(GL_TEXTURE_2D, spec);

	//Draw
	glBindVertexArray(CubeVAO);
	glDrawElements(GL_TRIANGLES, CubeIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Release Textures: in reverse order!
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_TEXTURE0);
}

void drawCube(GLuint texture) {
	glEnable(GL_TEXTURE_2D);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);    //盒子碰撞器加环境光
	glEnable(GL_COLOR_MATERIAL);
	int i, j;
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat z1 = -0.5, z2 = 0.5;

	//指定六个面的四个顶点，每个顶点用3个坐标值表示     
	//前 后 上 下 左 右  

	GLfloat point[6][4][3] = {
		{ { x1,y1,z1 },{ x1,y2,z1 },{ x2,y2,z1 },{ x2,y1,z1 } },
		{ { x1,y1,z2 },{ x2,y1,z2 },{ x2,y2,z2 },{ x1,y2,z2 } },
		{ { x1,y2,z1 },{ x1,y2,z2 },{ x2,y2,z2 },{ x2,y2,z1 } },
		{ { x1,y1,z1 },{ x2,y1,z1 },{ x2,y1,z2 },{ x1,y1,z2 } },
		{ { x2,y1,z1 },{ x2,y2,z1 },{ x2,y2,z2 },{ x2,y1,z2 } },
		{ { x1,y1,z1 },{ x1,y1,z2 },{ x1,y2,z2 },{ x1,y2,z1 } },
	};

	int dir[6][4][2] = {
		{ { 0,0 },{ 0,1 },{ 1,1 },{ 1,0 } },
		{ { 0,0 },{ 1,0 },{ 1,1 },{ 0,1 } },
		{ { 0,1 },{ 0,0 },{ 1,0 },{ 1,1 } },
		{ { 1,1 },{ 0,1 },{ 0,0 },{ 1,0 } },
		{ { 1,0 },{ 1,1 },{ 0,1 },{ 0,0 } },
		{ { 0,0 },{ 1,0 },{ 1,1 },{ 0,1 } },
	};

	for (i = 0; i < 6; i++) {
		glm::vec3 v1(point[i][0][0], point[i][0][1], point[i][0][2]);
		glm::vec3 v2(point[i][1][0], point[i][1][1], point[i][1][2]);
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
		for (j = 0; j < 4; j++) {
			glTexCoord2iv(dir[i][j]);
			glNormal3fv(glm::value_ptr(normal));
			glVertex3fv(point[i][j]);
		}
		glEnd();
	}

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
}

void drawSkybox(GLuint* texture) {
	//上    
	glPushMatrix();
	glTranslatef(0.0f, SkyboxSize / 2.0f, 0.0f);
	glRotatef(270, 1, 0, 0);

	glScalef(SkyboxSize, SkyboxSize, 1);
	drawRect(texture[2]);
	glPopMatrix();

	//下   
	glPushMatrix();
	glTranslatef(0.0f, -SkyboxSize / 2.0f, 0.0f);
	glRotatef(90, 1, 0, 0);

	glScalef(SkyboxSize, SkyboxSize, 1);
	drawRect(texture[3]);
	glPopMatrix();

	//左
	glPushMatrix();
	glTranslatef(-SkyboxSize / 2.0f, 0.0f, 0.0f);
	glRotatef(270, 0, 1, 0);
	glRotatef(180, 0, 0, 1);

	glScalef(SkyboxSize, SkyboxSize, 1);
	drawRect(texture[4]);
	glPopMatrix();

	//右
	glPushMatrix();
	glTranslatef(SkyboxSize / 2.0f, 0.0f, 0.0f);
	glRotatef(90, 0, 1, 0);
	glRotatef(180, 0, 0, 1);

	glScalef(SkyboxSize, SkyboxSize, 1);
	drawRect(texture[5]);
	glPopMatrix();

	//前
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -SkyboxSize / 2.0);
	glRotatef(180, 1, 0, 0);

	glScalef(SkyboxSize, SkyboxSize, 1);
	drawRect(texture[6]);
	glPopMatrix();

	//后
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, SkyboxSize / 2.0f);
	glRotatef(180, 0, 0, 1);

	glScalef(SkyboxSize, SkyboxSize, 1);
	drawRect(texture[7]);
	glPopMatrix();
}

void initSingleBoxCollider(glm::vec3 pos, glm::vec3 scalar) {
	boxPosition.push_back(pos);
	boxScale.push_back(scalar);
	boxSum++;
	isBreadEatenSet.push_back(false);
}

void initBoxCollidersProperty() {
	//1-0
	initSingleBoxCollider(glm::vec3(-60.f, -1.0f*roomSizeY / 2.0f + 2.5f, 60.f),
		glm::vec3(5, 5, 40));
	//2-1
	initSingleBoxCollider(glm::vec3(-70.f, -1.0f*roomSizeY / 2.0f + 7.5f, 40.f),
		glm::vec3(20, 5, 5));
	//3-2
	initSingleBoxCollider(glm::vec3(-80.f, -1.0f*roomSizeY / 2.0f + 12.5f, 0.f),
		glm::vec3(5, 5, 80));
	//4-1
	initSingleBoxCollider(glm::vec3(-50.f, -1.0f*roomSizeY / 2.0f + 7.5f, -40.f),
		glm::vec3(60, 5, 5));
	//5-2
	initSingleBoxCollider(glm::vec3(-20.f, -1.0f*roomSizeY / 2.0f + 12.5f, -20.f),
		glm::vec3(5, 5, 40));
	//5.1-3
	initSingleBoxCollider(glm::vec3(-30.f, -1.0f*roomSizeY / 2.0f + 17.5f, 0.f),
		glm::vec3(20, 5, 5));
	//6-4
	initSingleBoxCollider(glm::vec3(-40.f, -1.0f*roomSizeY / 2.0f + 22.5f, 10.f),
		glm::vec3(5, 5, 20));
	//7-3
	initSingleBoxCollider(glm::vec3(-10.f, -1.0f*roomSizeY / 2.0f + 17.5f, 20.f),
		glm::vec3(60, 5, 5));
	//8-4
	initSingleBoxCollider(glm::vec3(20.f, -1.0f*roomSizeY / 2.0f + 22.5f, -30.f),
		glm::vec3(5, 5, 100));
	//9-5
	initSingleBoxCollider(glm::vec3(-20.f, -1.0f*roomSizeY / 2.0f + 27.5f, -80.f),
		glm::vec3(80, 5, 5));
	//10-3
	initSingleBoxCollider(glm::vec3(50.f, -1.0f*roomSizeY / 2.0f + 17.5f, -80.f),
		glm::vec3(60, 5, 5));
	//11-2
	initSingleBoxCollider(glm::vec3(80.f, -1.0f*roomSizeY / 2.0f + 12.5f, -60.f),
		glm::vec3(5, 5, 40));
	//12-1
	initSingleBoxCollider(glm::vec3(60.f, -1.0f*roomSizeY / 2.0f + 7.5f, -40.f),
		glm::vec3(40, 5, 5));
	//13-2
	initSingleBoxCollider(glm::vec3(40.f, -1.0f*roomSizeY / 2.0f + 12.5f, 10.f),
		glm::vec3(5, 5, 100));
	//14-3
	initSingleBoxCollider(glm::vec3(20.f, -1.0f*roomSizeY / 2.0f + 17.5f, 60.f),
		glm::vec3(40, 5, 5));
	//15-2
	initSingleBoxCollider(glm::vec3(-20.f, -1.0f*roomSizeY / 2.0f + 12.5f, 60.f),
		glm::vec3(40, 5, 5));
	//16-4
	initSingleBoxCollider(glm::vec3(0.f, -1.0f*roomSizeY / 2.0f + 22.5f, 70.f),
		glm::vec3(5, 5, 20));
	//17-5
	initSingleBoxCollider(glm::vec3(40.f, -1.0f*roomSizeY / 2.0f + 27.5f, 80.f),
		glm::vec3(80, 5, 5));
	//18-6
	initSingleBoxCollider(glm::vec3(80.f, -1.0f*roomSizeY / 2.0f + 32.5f, 40.f),
		glm::vec3(5, 5, 80));

}

void setBoxColliderBoundary(FPSCamera* cam) {
	for (int i = 0; i < boxPosition.size(); i++) {
		cam->setSceneInnerBoundary(boxPosition[i].x - boxScale[i].x / 2.f,
			boxPosition[i].y - boxScale[i].y / 2.f,
			boxPosition[i].z - boxScale[i].z / 2.f,
			boxPosition[i].x + boxScale[i].x / 2.f,
			boxPosition[i].y + boxScale[i].y / 2.f,
			boxPosition[i].z + boxScale[i].z / 2.f);
	}
}

void drawBoxColliders(Shader shader, GLuint diffuse, GLuint bump, GLuint spec, FPSCamera* cam) {

	glUniform3fv(
		glGetUniformLocation(shader.Program, "lightPos"),
		1,
		LightPosition
	);

	glUniform3fv(
		glGetUniformLocation(shader.Program, "viewPos"),
		1,
		glm::value_ptr(cam->cameraPos)
	);

	float P[16];
	glGetFloatv(GL_PROJECTION_MATRIX, P);
	//Projection does not change
	glUniformMatrix4fv(
		glGetUniformLocation(shader.Program, "projection"),
		1,
		GL_FALSE,
		//glm::value_ptr(cam->projectionMatrix)
		P
	);

	for (int i = 0; i < boxPosition.size(); i++) {

		glm::mat4 boxPos = glm::translate(glm::mat4(1.0), glm::vec3(boxPosition[i].x, boxPosition[i].y, boxPosition[i].z));
		glm::mat4 boxScl = glm::scale(glm::mat4(1.0), glm::vec3(boxScale[i].x, boxScale[i].y, boxScale[i].z));

		glUniformMatrix4fv(
			glGetUniformLocation(shader.Program, "model"),
			1,
			GL_FALSE,
			glm::value_ptr(boxPos * boxScl)
		);

		glUniformMatrix4fv(
			glGetUniformLocation(shader.Program, "view"),
			1,
			GL_FALSE,
			glm::value_ptr(cam->viewMatrix)
		);

		drawCube(shader, diffuse, bump, spec);
	}
}

void drawBoxColliders(GLuint* texture) {
	for (int i = 0; i < boxPosition.size(); i++) {
		glStencilMask(0x00);
		glPushMatrix();
		glTranslatef(boxPosition[i].x, boxPosition[i].y, boxPosition[i].z);
		glScalef(boxScale[i].x, boxScale[i].y, boxScale[i].z);
		drawCube(texture[1]);
		glPopMatrix();
	}	
}


void initBreadModels() {
	int breadSum = boxSum;

	Model* myBreadModel = new Model;
	if (!myBreadModel->importModel("ModelRes/Bread/Bread.obj"))
		cout << "Import model error!" << endl;

	for (int i = 0; i < breadSum; i++) {
		breadSet.push_back(myBreadModel);
	}

	//Init Bread Eaten Effect
	for (int i = 0; i < breadSum; i++) {
		ParticleSystem eaten(150, GravityAcceler);
		eaten.init();
		breadEatenEffectSet.push_back(eaten);
	}
}

void drawBreadModels() {
	for (int i = 0; i < breadSet.size(); i++) {
		if (!isBreadEatenSet[i]) {
			glPushMatrix();

			// 设置模板缓冲为可写状态，把较小的箱子放入模板缓冲（设为1）
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			glTranslatef(boxPosition[i].x, boxPosition[i].y + 10.f, boxPosition[i].z);
			glRotatef(angle, 0.f, 1.f, 0.f);
			glScalef(5.f, 5.f, 5.f);
			breadSet[i]->renderTheModel(0.5f, false);
			glPopMatrix();
		}

		//模板检测，绘制边缘
		if (i == closeToBreadIndex) {
			glPushMatrix();

			// 设置模板缓冲为不可写状态，只绘制 != 1 的部分
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);    //边框不会被其他物体覆盖

			glTranslatef(boxPosition[i].x, boxPosition[i].y + 9.92f, boxPosition[i].z);
			glRotatef(angle, 0.f, 1.f, 0.f);
			glScalef(6.2f, 6.2f, 6.2f);
			breadSet[i]->renderTheModel(0.5f, true);

			glStencilMask(0xFF);
			glEnable(GL_DEPTH_TEST);

			glPopMatrix();
		}
	}
	angle += 1.5f;
}

void playBreadEatenEffect (FPSCamera* cam) {
	for (int i = 0; i < breadSet.size(); i++) {
		if (isBreadEatenSet[i]) {
			glPushMatrix();
			glm::vec3 pPos = boxPosition[i] + cam->getForward() * 10.f;
			glTranslatef(pPos.x, pPos.y + 7.f, pPos.z);
			breadEatenEffectSet[i].simulate(0.01);
			breadEatenEffectSet[i].render();
			glPopMatrix();
		}
	}
}

void deleteBreadModels() {
	for (int i = 0; i < breadSet.size(); i++)
		delete breadSet[i];
}

void detectBreadBeingEaten(FPSCamera* cam) {
	for (int i = 0; i < breadSet.size(); i++) {
		if (!isBreadEatenSet[i]) {
			glm::vec3 breadPos(boxPosition[i].x, boxPosition[i].y + 10.f, boxPosition[i].z);

			//先检测是否靠近面包
			if (cam->detectPlayerCloseToBread(breadPos, CloseToBreadDistance)) {
				closeToBreadIndex = i;

				//再检测是否能吃掉面包
				if (cam->detectPlayerEatingBread(breadPos, EatBreadDistance)) {
					isBreadEatenSet[i] = true;
					eatenBreadNum++;
					closeToBreadIndex = -1;
					break;
				}
				break;
			}
			else
				closeToBreadIndex = -1;
		}
	}
}

float redAmbient[4] = { 1, 0, 0, 1 };
float redDiffuse[4] = { 1, 0, 0, 1 };
float redSpecular[4] = { 1, 0, 0, 1 };
float redEmission[4] = { 1, 0, 0, 1 };

void applyRedMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, redEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
}

float whiteAmbient[4] = { 1, 1, 1, 1 };
float whiteDiffuse[4] = { 1, 1, 1, 1 };
float whiteSpecular[4] = { 1, 1, 1, 1 };
float whiteEmission[4] = { 1, 1, 1, 1 };

void applyWhiteMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, whiteAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whiteDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, whiteEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
}

float blackAmbient[4] = { 0, 0, 0, 1 };
float blackDiffuse[4] = { 0, 0, 0, 1 };
float blackSpecular[4] = { 0, 0, 0, 1 };
float blackEmission[4] = { 0, 0, 0, 1 };

void applyBlackMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
}

#define FONT_ComicSansMS "Comic Sans MS"
#define FONT_KaiTi "楷体"
#define FONT_FangSong "仿宋"

string GameTitle = "Bread Eating Game";
string GameRule = "Game Rule: ";
string GameRuleCtrl = "Control:\n   1. Press 'w/a/s/d' to move.\n   2. Press 'space' to jump.\n   3. Use 'mouse' to look around.";
string GameRuleTarget = "Target:\n    Eat as more bread as possible!";
string GameStartTitle = "Start";
string GameMaker = "——由第26组制作";
string GameVictory = "Win!!";

//选择font
void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

//绘制英文文字
#define MAX_CHAR 128
void drawEnString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	lists = glGenLists(MAX_CHAR);
	wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);

	//if (isFirstCall == 1) { // 如果是第一次调用，执行初始化, 为每一个ASCII字符产生一个显示列表
	//	isFirstCall = 0;

	//	// 申请MAX_CHAR个连续的显示列表编号
	//	lists = glGenLists(MAX_CHAR);

	//	// 把每个字符的绘制命令都装到对应的显示列表中
	//	wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	//}

	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);

	glDeleteLists(lists, MAX_CHAR);
}

//绘制中文文字
void drawCNString(const char* str) {
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint list = glGenLists(1);

	// 计算字符的个数
	// 如果是双字节字符的（比如中文字符），两个字节才算一个字符
	// 否则一个字节算一个字符
	len = 0;
	for (i = 0; str[i] != '\0'; ++i) {
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}

	// 将混合字符转化为宽字符
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';

	// 逐个输出字符
	for (i = 0; i < len; ++i) {
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}

	// 回收所有临时资源
	free(wstring);
	glDeleteLists(list, 1);
}

void drawMenuSceneUIText(FPSCamera* cam) {
	char strBuffer[200];

	//顶部标题框
	glPushMatrix();
	applyWhiteMaterial();
	glBegin(GL_POLYGON);
	glVertex2f(cam->cameraPos.x - 18.f, cam->cameraPos.y + 25.f);
	glVertex2f(cam->cameraPos.x - 18.f, cam->cameraPos.y + 31.f);
	glVertex2f(cam->cameraPos.x + 18.f, cam->cameraPos.y + 31.f);
	glVertex2f(cam->cameraPos.x + 18.f, cam->cameraPos.y + 25.f);
	glEnd();
	glPopMatrix();

	selectFont(40, ANSI_CHARSET, FONT_ComicSansMS);

	//顶部标题
	glPushMatrix();
	applyBlackMaterial();
	glRasterPos3f(cam->cameraPos.x - 0.86f, cam->cameraPos.y + 1.48f, cam->cameraPos.z - 5.f);
	drawEnString(GameTitle.c_str());
	glPopMatrix();

	applyWhiteMaterial();

	selectFont(36, ANSI_CHARSET, FONT_ComicSansMS);
	//游戏规则
	glPushMatrix();
	glRasterPos3f(cam->cameraPos.x - 1.2f, cam->cameraPos.y + 0.8f, cam->cameraPos.z - 5.f);
	drawEnString(GameRule.c_str());
	glPopMatrix();

	//控制方式
	glPushMatrix();
	glRasterPos3f(cam->cameraPos.x - 1.f, cam->cameraPos.y + 0.5f, cam->cameraPos.z - 5.f);
	const char * GameRuleCtrlc = GameRuleCtrl.c_str();
	sprintf(strBuffer, "%s", GameRuleCtrlc);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)strBuffer);
	glPopMatrix();

	//目标
	glPushMatrix();
	glRasterPos3f(cam->cameraPos.x - 1.f, cam->cameraPos.y - 0.3f, cam->cameraPos.z - 5.f);
	const char * GameRuleTargetc = GameRuleTarget.c_str();
	sprintf(strBuffer, "%s", GameRuleTargetc);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)strBuffer);
	glPopMatrix();

	//Start按钮框
	glPushMatrix();
	applyWhiteMaterial();
	glBegin(GL_POLYGON);
	glVertex2f(cam->cameraPos.x - 7.f, cam->cameraPos.y - 18.f);
	glVertex2f(cam->cameraPos.x - 7.f, cam->cameraPos.y - 24.f);
	glVertex2f(cam->cameraPos.x + 7.f, cam->cameraPos.y - 24.f);
	glVertex2f(cam->cameraPos.x + 7.f, cam->cameraPos.y - 18.f);
	glEnd();
	glPopMatrix();

	selectFont(40, ANSI_CHARSET, FONT_ComicSansMS);
	//Start标题
	glPushMatrix();
	applyBlackMaterial();
	glRasterPos3f(cam->cameraPos.x - 0.25f, cam->cameraPos.y - 1.24f, cam->cameraPos.z - 5.f);
	drawEnString(GameStartTitle.c_str());
	glPopMatrix();

	
	glPushMatrix();
	selectFont(24, GB2312_CHARSET, FONT_KaiTi);
	//selectFont(24, GB2312_CHARSET, FONT_FangSong);
	applyWhiteMaterial();
	glRasterPos3f(cam->cameraPos.x + 0.5f, cam->cameraPos.y - 1.7f, cam->cameraPos.z - 5.f);
	drawCNString(GameMaker.c_str());
	glPopMatrix();

}

#define VictoryTextSizeLow 60    //闪烁文字最小字号
#define VictoryTextSizeUp 200    //闪烁文字最大字号
#define VictoryTextSizeGap 6     //闪烁文字放缩规模

string GameSceneUIText = "Bread: ";
static int victoryTextSize = VictoryTextSizeLow;
int dSize = VictoryTextSizeGap;

void drawGameSceneUIText(FPSCamera* cam, int x, int y) {
	glm::mat4 screenScale = glm::scale(glm::mat4(1.0), glm::vec3(1.0 / 600, -1.0 / 600, 1));

	glm::vec4 camCo = screenScale * glm::vec4(x, y, -1.3, 1);
	//cout << glm::to_string(camCo) << endl;
	glm::vec4 camCoD = camCo + glm::vec4(-0.48, 0.47, 0, 0);
	//cout << glm::to_string(camCoD) << endl;

	//why to add this ??????
	drawEnString("tt");

	glm::vec4 textPosD = glm::inverse(cam->viewMatrix) *  camCoD;
	textPosD = textPosD / textPosD[3];

	glPushMatrix();
	//cout << "Camera Pos: " << glm::to_string(cam->cameraPos) << endl;
	//cout << "UI Canvas: " << glm::to_string(UISurfaceCenter) << endl;

	selectFont(36, ANSI_CHARSET, FONT_ComicSansMS);
	applyBlackMaterial();
	//glRasterPos3f(uiCanvasCen.x, uiCanvasCen.y, uiCanvasCen.z);
	glRasterPos3f(textPosD.x, textPosD.y, textPosD.z);

	char strBuffer[80];
	const char * UIText1c = GameSceneUIText.c_str();
	string UIText2 = " / ";
	const char * UIText2c = UIText2.c_str();
	sprintf(strBuffer, "%s%d%s%d", UIText1c, eatenBreadNum, UIText2c, boxSum);
	//glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)strBuffer);
	drawEnString(strBuffer);
	glPopMatrix(); 

	//胜利，绘制 "Win!"
	//if (eatenBreadNum == boxSum) {
	if (eatenBreadNum == 3) {
		glm::vec4 textPos = glm::inverse(cam->viewMatrix) *  camCo;
		textPos = textPos / textPos[3];

		victoryTextSize += dSize;
		selectFont(victoryTextSize, ANSI_CHARSET, FONT_ComicSansMS);
		glPushMatrix();
		applyRedMaterial();
		glRasterPos3f(textPos.x - 0.2f, textPos.y, textPos.z);
		drawEnString(GameVictory.c_str());
		glPopMatrix();

		if (victoryTextSize >= VictoryTextSizeUp)
			dSize = -VictoryTextSizeGap;
		else if (victoryTextSize <= VictoryTextSizeLow)
			dSize = VictoryTextSizeGap;
	}
}

void setupLights() {
	glClearDepth(1.0f);				// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// 启用光照计算  
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT2, GL_POSITION, LightPosition2);

	glEnable(GL_LIGHT3);
	glLightfv(GL_LIGHT3, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT3, GL_POSITION, LightPosition3);

	glEnable(GL_LIGHT4);
	glLightfv(GL_LIGHT4, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT4, GL_POSITION, LightPosition4);

}