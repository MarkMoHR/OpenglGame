#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>  
#include <glm/gtx/string_cast.hpp>
#include "SceneController.h"

#define Sensitivity 0.003

FPSCamera* cam;

static int mouseLastPosX = 0;
static int mouseLastPosY = 0;

GLuint texture[8];

void drawScene() {

	//天空盒
	drawSkybox(texture);

	//地板  
	glPushMatrix();
	glTranslatef(0.0f, -roomSizeY / 2.0f, 0.0f);
	glRotatef(90, 1, 0, 0);
	glScalef(roomSizeX, roomSizeZ, 1);
	drawRect(texture[0]);
	glPopMatrix();

	//箱子  
	drawBoxColliders(texture);
	drawBreadModels();
	playBreadEatenEffect(cam);

	glColor3f(1, 1, 1);

	//文字
	drawUIText(cam, 0, 0);


	cam->updateCameraMovement();
	detectBreadBeingEaten(cam);

}

void reshape(int width, int height) {
	if (height == 0) {
		height = 1;        //让高度为1（避免出现分母为0的现象）   
	}

	cam->resetWinSize(width, height);    
}

void idle() {
	glutPostRedisplay();
}

void initLightingAndTexture() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glGenTextures(8, texture);
	loadTex(0, "Textures/11.bmp", texture);    //地板
	loadTex(1, "Textures/14.bmp", texture);    //箱子

	//选择以下任意一种天空盒贴图

	//loadTex(2, "Textures/Skybox/SkyBox2_up.bmp", texture);
	//loadTex(3, "Textures/Skybox/SkyBox2_down.bmp", texture);
	//loadTex(4, "Textures/Skybox/SkyBox2_left.bmp", texture);
	//loadTex(5, "Textures/Skybox/SkyBox2_right.bmp", texture);
	//loadTex(6, "Textures/Skybox/SkyBox2_front.bmp", texture);
	//loadTex(7, "Textures/Skybox/SkyBox2_back.bmp", texture);

	loadTex(2, "Textures/Skybox/Sunny_up.bmp", texture);
	loadTex(3, "Textures/Skybox/Sunny_down.bmp", texture);
	loadTex(4, "Textures/Skybox/Sunny_left.bmp", texture);
	loadTex(5, "Textures/Skybox/Sunny_right.bmp", texture);
	loadTex(6, "Textures/Skybox/Sunny_front.bmp", texture);
	loadTex(7, "Textures/Skybox/Sunny_back.bmp", texture);
}

void normalKeyPress(unsigned char key, int x, int y) {
	cam->keyPressed(key);
}

void normalKeyUp(unsigned char key, int x, int y) {
	cam->keyUp(key);
}

//鼠标刚点击时
void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouseLastPosX = x;
		mouseLastPosY = y;
	}
}

//鼠标点击且移动时
void mouseMove(int x, int y) {
	//与点击处的相对距离
	float pitch = (float)(y - mouseLastPosY) * Sensitivity;
	float yaw = (float)(x - mouseLastPosX) * Sensitivity;
	mouseLastPosY = y;
	mouseLastPosX = x;
	cam->rotate(pitch, yaw);
}

void redraw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	drawScene();
	glutSwapBuffers();
}

void initializeGL() {
	cam = new FPSCamera();
	//添加碰撞边缘
	cam->setSceneOuterBoundary(-roomSizeX / 2.0, -roomSizeZ / 2.0, roomSizeX / 2.0, roomSizeZ / 2.0);

	cam->setSceneInnerBoundary(-roomSizeX / 2.0,  -roomSizeY / 2.0f - 1.f, -roomSizeZ / 2.0,
		roomSizeX / 2.0, -roomSizeY / 2.0f, roomSizeZ / 2.0);    //地板collider

	initBoxCollidersProperty();
	setBoxColliderBoundary(cam);

	initBreadModels();

	setupLights();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);    
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(250, 100);
	glutInitWindowSize(600, 600);
	int windowHandle = glutCreateWindow("Final project - OpenGL game!");

	initializeGL();

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(normalKeyPress);
	glutKeyboardUpFunc(normalKeyUp);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutIdleFunc(idle);

	initLightingAndTexture();
	glutMainLoop();

	delete cam;
	deleteBreadModels();

	return 0;
}