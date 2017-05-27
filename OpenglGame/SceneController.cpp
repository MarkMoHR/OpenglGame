#include "SceneController.h"
#include <iostream>
#include <vector>
using namespace std;

vector<glm::vec3> boxPosition;
vector<glm::vec3> boxScale;

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

void drawSkybox(GLuint* texture) {
	//上    
	glPushMatrix();
	glTranslatef(0.0f, SkyboxSize / 2.0f, 0.0f);
	glRotatef(270, 1, 0, 0);

	glScalef(SkyboxSize, SkyboxSize, 1);
	drawRect(texture[4]);
	glPopMatrix();

	//下   
	glPushMatrix();
	glTranslatef(0.0f, -SkyboxSize / 2.0f, 0.0f);
	glRotatef(90, 1, 0, 0);

	glScalef(SkyboxSize, SkyboxSize, 1);
	drawRect(texture[5]);
	glPopMatrix();

	//左
	glPushMatrix();
	glTranslatef(-SkyboxSize / 2.0f, 0.0f, 0.0f);
	glRotatef(270, 0, 1, 0);
	glRotatef(180, 0, 0, 1);

	glScalef(SkyboxSize, SkyboxSize, 1);
	drawRect(texture[6]);
	glPopMatrix();

	//右
	glPushMatrix();
	glTranslatef(SkyboxSize / 2.0f, 0.0f, 0.0f);
	glRotatef(90, 0, 1, 0);
	glRotatef(180, 0, 0, 1);

	glScalef(SkyboxSize, SkyboxSize, 1);
	drawRect(texture[7]);
	glPopMatrix();

	//前
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -SkyboxSize / 2.0);
	glRotatef(180, 1, 0, 0);

	glScalef(SkyboxSize, SkyboxSize, 1);
	drawRect(texture[8]);
	glPopMatrix();

	//后
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, SkyboxSize / 2.0f);
	glRotatef(180, 0, 0, 1);

	glScalef(SkyboxSize, SkyboxSize, 1);
	drawRect(texture[9]);
	glPopMatrix();
}

void initBoxCollidersProperty() {
	//1-0
	boxPosition.push_back(glm::vec3(-60.f, -1.0f*roomSizeY / 2.0f + 2.5f, 60.f));
	boxScale.push_back(glm::vec3(5, 5, 40));
	//2-1
	boxPosition.push_back(glm::vec3(-70.f, -1.0f*roomSizeY / 2.0f + 7.5f, 40.f));
	boxScale.push_back(glm::vec3(20, 5, 5));
	//3-2
	boxPosition.push_back(glm::vec3(-80.f, -1.0f*roomSizeY / 2.0f + 12.5f, 0.f));
	boxScale.push_back(glm::vec3(5, 5, 80));
	//4-1
	boxPosition.push_back(glm::vec3(-50.f, -1.0f*roomSizeY / 2.0f + 7.5f, -40.f));
	boxScale.push_back(glm::vec3(60, 5, 5));
	//5-2
	boxPosition.push_back(glm::vec3(-20.f, -1.0f*roomSizeY / 2.0f + 12.5f, -20.f));
	boxScale.push_back(glm::vec3(5, 5, 40));
	//5.1-3
	boxPosition.push_back(glm::vec3(-30.f, -1.0f*roomSizeY / 2.0f + 17.5f, 0.f));
	boxScale.push_back(glm::vec3(20, 5, 5));
	//6-4
	boxPosition.push_back(glm::vec3(-40.f, -1.0f*roomSizeY / 2.0f + 22.5f, 10.f));
	boxScale.push_back(glm::vec3(5, 5, 20));
	//7-3
	boxPosition.push_back(glm::vec3(-10.f, -1.0f*roomSizeY / 2.0f + 17.5f, 20.f));
	boxScale.push_back(glm::vec3(60, 5, 5));
	//8-4
	boxPosition.push_back(glm::vec3(20.f, -1.0f*roomSizeY / 2.0f + 22.5f, -30.f));
	boxScale.push_back(glm::vec3(5, 5, 100));
	//9-5
	boxPosition.push_back(glm::vec3(-20.f, -1.0f*roomSizeY / 2.0f + 27.5f, -80.f));
	boxScale.push_back(glm::vec3(80, 5, 5));
	//10-3
	boxPosition.push_back(glm::vec3(50.f, -1.0f*roomSizeY / 2.0f + 17.5f, -80.f));
	boxScale.push_back(glm::vec3(60, 5, 5));
	//11-2
	boxPosition.push_back(glm::vec3(80.f, -1.0f*roomSizeY / 2.0f + 12.5f, -60.f));
	boxScale.push_back(glm::vec3(5, 5, 40));
	//12-1
	boxPosition.push_back(glm::vec3(60.f, -1.0f*roomSizeY / 2.0f + 7.5f, -40.f));
	boxScale.push_back(glm::vec3(40, 5, 5));
	//13-2
	boxPosition.push_back(glm::vec3(40.f, -1.0f*roomSizeY / 2.0f + 12.5f, 10.f));
	boxScale.push_back(glm::vec3(5, 5, 100));
	//14-3
	boxPosition.push_back(glm::vec3(20.f, -1.0f*roomSizeY / 2.0f + 17.5f, 60.f));
	boxScale.push_back(glm::vec3(40, 5, 5));
	//15-2
	boxPosition.push_back(glm::vec3(-20.f, -1.0f*roomSizeY / 2.0f + 12.5f, 60.f));
	boxScale.push_back(glm::vec3(40, 5, 5));
	//16-4
	boxPosition.push_back(glm::vec3(0.f, -1.0f*roomSizeY / 2.0f + 22.5f, 70.f));
	boxScale.push_back(glm::vec3(5, 5, 20));
	//17-5
	boxPosition.push_back(glm::vec3(40.f, -1.0f*roomSizeY / 2.0f + 27.5f, 80.f));
	boxScale.push_back(glm::vec3(80, 5, 5));
	//18-6
	boxPosition.push_back(glm::vec3(80.f, -1.0f*roomSizeY / 2.0f + 32.5f, 40.f));
	boxScale.push_back(glm::vec3(5, 5, 80));


}

void drawBoxColliders(GLuint* texture) {
	for (int i = 0; i < boxPosition.size(); i++) {
		glPushMatrix();
		glTranslatef(boxPosition[i].x, boxPosition[i].y, boxPosition[i].z);
		glScalef(boxScale[i].x, boxScale[i].y, boxScale[i].z);
		drawCube(texture[2]);
		glPopMatrix();
	}
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