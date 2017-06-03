#define _CRT_SECURE_NO_WARNINGS

#include "SceneController.h"
#include "ParticleSystem.h"
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

vector<glm::vec3> boxPosition;
vector<glm::vec3> boxScale;
static int boxSum = 0;

vector<Model*> breadSet;
vector<ParticleSystem> breadEatenEffectSet;
vector<bool> isBreadEatenSet;
static int eatenBreadNum = 0;

static float angle = 0.0f;

GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, SkyboxSize / 2.f, 0.f, 1.0f };
GLfloat LightPosition2[] = { -SkyboxSize / 2.f, -SkyboxSize / 2.f, 0.f, 1.0f };

void drawRect(GLuint texture) {
	glEnable(GL_TEXTURE_2D);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);    //天空盒加环境光
	glEnable(GL_COLOR_MATERIAL);

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

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
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
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
		for (j = 0; j < 4; j++) {
			glTexCoord2iv(dir[i][j]);
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

void drawBoxColliders(GLuint* texture) {
	for (int i = 0; i < boxPosition.size(); i++) {
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
			glTranslatef(boxPosition[i].x, boxPosition[i].y + 10.f, boxPosition[i].z);
			glRotatef(angle, 0.f, 1.f, 0.f);
			glScalef(5.f, 5.f, 5.f);
			breadSet[i]->renderTheModel(0.5f);
			glPopMatrix();
		}
	}
	angle += 0.75f;
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
			if (cam->detectPlayerEatingBread(breadPos, EatBreadDistance)) {
				isBreadEatenSet[i] = true;
				eatenBreadNum++;
				break;
			}
		}
	}
}


float textAmbient[4] = { 0, 0, 0, 0 };
float textDiffuse[4] = { 0, 0, 0, 0 };
float textSpecular[4] = { 0, 0, 0, 0 };
string UIText = "Bread: ";

void drawUIText(FPSCamera* cam, int x, int y) {
	glm::mat4 screenScale = glm::scale(glm::mat4(1.0), glm::vec3(1.0 / 600, -1.0 / 600, 1));

	glm::vec4 camCo = screenScale * glm::vec4(x, y, -1.3, 1);
	//cout << glm::to_string(camCo) << endl;
	camCo = camCo + glm::vec4(-0.5, 0.5, 0, 0);
	//cout << glm::to_string(camCo) << endl;

	glm::vec4 textPos = glm::inverse(cam->viewMatrix) *  camCo;
	textPos = textPos / textPos[3];

	//cout << "Camera Pos: " << glm::to_string(cam->cameraPos) << endl;
	//cout << "UI Canvas: " << glm::to_string(UISurfaceCenter) << endl;
	glColor3f(0, 0, 0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, textAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, textDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, textSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	//glRasterPos3f(uiCanvasCen.x, uiCanvasCen.y, uiCanvasCen.z);
	glRasterPos3f(textPos.x, textPos.y, textPos.z);

	char strBuffer[80];
	const char * UIText1c = UIText.c_str();
	string UIText2 = " / ";
	const char * UIText2c = UIText2.c_str();
	sprintf(strBuffer, "%s%d%s%d", UIText1c, eatenBreadNum, UIText2c, boxSum);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)strBuffer);

	glColor4f(1, 1, 1, 1);

	glPopMatrix();
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

}