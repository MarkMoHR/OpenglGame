#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>  
#include "SceneController.h"

#define Sensitivity 0.003

FPSCamera* cam;

static int mouseLastPosX = 0;
static int mouseLastPosY = 0;

GLuint texture[10];

void drawScene() {
	//天空盒
	drawSkybox(texture);

	//地板  
	glPushMatrix();
	glTranslatef(0.0f, -roomSizeY / 2.0f, 0.0f);
	glRotatef(90, 1, 0, 0);
	glScalef(roomSizeX, roomSizeZ, 1);
	drawRect(texture[3]);
	glPopMatrix();

	//箱子  
	drawBoxColliders(texture);

	cam->updateCameraMovement();
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
	glGenTextures(10, texture);
	loadTex(0, "textures/1.bmp", texture);
	loadTex(1, "textures/16.bmp", texture);
	loadTex(2, "textures/14.bmp", texture);
	loadTex(3, "textures/11.bmp", texture);

	loadTex(4, "textures/Skybox/Sunny_up.bmp", texture);
	loadTex(5, "textures/Skybox/Sunny_down.bmp", texture);
	loadTex(6, "textures/Skybox/Sunny_left.bmp", texture);
	loadTex(7, "textures/Skybox/Sunny_right.bmp", texture);
	loadTex(8, "textures/Skybox/Sunny_front.bmp", texture);
	loadTex(9, "textures/Skybox/Sunny_back.bmp", texture);
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
	
	glPolygonMode(GL_FRONT, GL_FILL);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	// 启用光照计算  
	glEnable(GL_LIGHTING);
	// 指定环境光强度（RGBA）  
	GLfloat ambientLight[] = { 2.0f, 2.0f, 2.0f, 1.0f };

	// 设置光照模型，将ambientLight所指定的RGBA强度值应用到环境光  
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	// 启用颜色追踪  
	glEnable(GL_COLOR_MATERIAL);
	// 设置多边形正面的环境光和散射光材料属性，追踪glColor  
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

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
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);    
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(250, 100);
	glutInitWindowSize(600, 600);
	int windowHandle = glutCreateWindow("Final project!");

	initializeGL();

	glutDisplayFunc(redraw);               //注册绘制回调函数
	glutReshapeFunc(reshape);              //注册重绘回调函数
	glutKeyboardFunc(normalKeyPress);    //注册普通按键回调函数
	glutKeyboardUpFunc(normalKeyUp);
	glutMouseFunc(mouseClick);             //注册鼠标点击回调函数
	glutMotionFunc(mouseMove);             //注册鼠标点击&移动回调函数
	glutIdleFunc(idle);                    //注册全局回调函数：空闲时调用       

	initLightingAndTexture();
	glutMainLoop();                        // glut事件处理循环    

	delete cam;

	return 0;
}