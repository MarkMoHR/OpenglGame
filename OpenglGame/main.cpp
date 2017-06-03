#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>  
#include <glm/gtx/string_cast.hpp>
#include "SceneController.h"

#define Sensitivity 0.003

FPSCamera* cam;

static int mouseLastPosX = 0;
static int mouseLastPosY = 0;

GLuint texture[8];

float textAmbient[4] = { 0, 0, 0, 0 };
float textDiffuse[4] = { 0, 0, 0, 0 };
float textSpecular[4] = { 0, 0, 0, 0 };
string testText = "testText!";

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

	glPushMatrix();
	glm::vec3 uiCanvasCen(cam->cameraPos + cam->getForward() * 1.5f);
	
	//cout << "Camera Pos: " << glm::to_string(cam->cameraPos) << endl;
	//cout << "UI Canvas: " << glm::to_string(UISurfaceCenter) << endl;
	glColor3f(0, 0, 0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, textAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, textDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, textSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	glRasterPos3f(uiCanvasCen.x, uiCanvasCen.y, uiCanvasCen.z);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)testText.c_str());
	glPopMatrix();

	glColor3f(1, 1, 1);

	cam->updateCameraMovement();
	detectBreadBeingEaten(cam);
}

void reshape(int width, int height) {
	if (height == 0) {     //如果高度为0 
		height = 1;        //让高度为1（避免出现分母为0的现象）      
	}

	cam->resetWinSize(width, height);    
}

void idle() {
	glutPostRedisplay();
}

void initLightingAndTexture() {
	glEnable(GL_DEPTH_TEST);//开启深度测试        
	glEnable(GL_LIGHTING);  //开启光照模式   
	glGenTextures(8, texture);
	loadTex(0, "Textures/11.bmp", texture);    //地板
	loadTex(1, "Textures/14.bmp", texture);    //盒子

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色和深度缓存   
	
	drawScene();//绘制场景   
	glutSwapBuffers();//交换缓冲区  
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
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);    
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(250, 100);
	glutInitWindowSize(600, 600);
	int windowHandle = glutCreateWindow("Final project - OpenGL game!");

	initializeGL();

	glutDisplayFunc(redraw);               //注册绘制回调函数
	glutReshapeFunc(reshape);              //注册重绘回调函数
	glutKeyboardFunc(normalKeyPress);      //注册普通按键回调函数
	glutKeyboardUpFunc(normalKeyUp);
	glutMouseFunc(mouseClick);             //注册鼠标点击回调函数
	glutMotionFunc(mouseMove);             //注册鼠标点击&移动回调函数
	glutIdleFunc(idle);                    //注册全局回调函数：空闲时调用       

	initLightingAndTexture();
	glutMainLoop();                        // glut事件处理循环    

	delete cam;
	deleteBreadModels();

	return 0;
}