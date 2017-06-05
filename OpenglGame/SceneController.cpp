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

string GameTitle = "Bread Eating Game";
string GameRule = "Game Rule: ";
string GameRuleCtrl = "Control:\n   1. Press 'w/a/s/d' to move.\n   2. Press 'space' to jump.\n   3. Use 'mouse' to look around.";
string GameRuleTarget = "Target:\n    Eat as more bread as possible!";
string GameStartTitle = "Start";
string GameMaker = "——由第26组制作";

void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

#define MAX_CHAR 128
void drawEnString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall == 1) { // 如果是第一次调用，执行初始化, 为每一个ASCII字符产生一个显示列表
		isFirstCall = 0;

		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);

		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

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
	for (i = 0; i<len; ++i) {
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

	selectFont(40, ANSI_CHARSET, "Comic Sans MS");

	//顶部标题
	glPushMatrix();
	applyBlackMaterial();
	glRasterPos3f(cam->cameraPos.x - 0.86f, cam->cameraPos.y + 1.48f, cam->cameraPos.z - 5.f);
	drawEnString(GameTitle.c_str());
	glPopMatrix();

	applyWhiteMaterial();

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

	//Start标题
	glPushMatrix();
	applyBlackMaterial();
	glRasterPos3f(cam->cameraPos.x - 0.25f, cam->cameraPos.y - 1.24f, cam->cameraPos.z - 5.f);
	drawEnString(GameStartTitle.c_str());
	glPopMatrix();

	
	glPushMatrix();
	selectFont(24, GB2312_CHARSET, "楷体");
	//selectFont(24, GB2312_CHARSET, "仿宋");
	applyWhiteMaterial();
	glRasterPos3f(cam->cameraPos.x + 0.5f, cam->cameraPos.y - 1.7f, cam->cameraPos.z - 5.f);
	drawCNString(GameMaker.c_str());
	glPopMatrix();

}

string GameSceneUIText = "Bread: ";

void drawGameSceneUIText(FPSCamera* cam, int x, int y) {
	glm::mat4 screenScale = glm::scale(glm::mat4(1.0), glm::vec3(1.0 / 600, -1.0 / 600, 1));

	glm::vec4 camCo = screenScale * glm::vec4(x, y, -1.3, 1);
	//cout << glm::to_string(camCo) << endl;
	glm::vec4 camCoD = camCo + glm::vec4(-0.48, 0.47, 0, 0);
	//cout << glm::to_string(camCoD) << endl;

	glm::vec4 textPosD = glm::inverse(cam->viewMatrix) *  camCoD;
	textPosD = textPosD / textPosD[3];

	glPushMatrix();
	//cout << "Camera Pos: " << glm::to_string(cam->cameraPos) << endl;
	//cout << "UI Canvas: " << glm::to_string(UISurfaceCenter) << endl;
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
	if (eatenBreadNum == boxSum) {
		glm::vec4 textPos = glm::inverse(cam->viewMatrix) *  camCo;
		textPos = textPos / textPos[3];

		glPushMatrix();
		applyRedMaterial();
		glRasterPos3f(textPos.x, textPos.y, textPos.z);
		drawEnString("Win!");
		glPopMatrix();
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

}