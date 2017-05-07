#include "SceneController.h"
#include <iostream>
using namespace std;

void drawRect(GLuint texture) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);  //选择纹理texture[status]   
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat point[4][2] = { { x1,y1 },{ x1,y2 },{ x2,y2 },{ x2,y1 } };
	int dir[4][2] = { { 1,1 },{ 1,0 },{ 0,0 },{ 0,1 } };
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++) {
		glTexCoord2iv(dir[i]);
		glVertex2fv(point[i]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawCube(GLuint texture) {
	glEnable(GL_TEXTURE_2D);
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
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
		for (j = 0; j < 4; j++) {
			glTexCoord2iv(dir[i][j]);
			glVertex3fv(point[i][j]);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}