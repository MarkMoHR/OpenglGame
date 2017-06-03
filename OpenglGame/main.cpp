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

	//��պ�
	drawSkybox(texture);

	//�ذ�  
	glPushMatrix();
	glTranslatef(0.0f, -roomSizeY / 2.0f, 0.0f);
	glRotatef(90, 1, 0, 0);
	glScalef(roomSizeX, roomSizeZ, 1);
	drawRect(texture[0]);
	glPopMatrix();

	//����  
	drawBoxColliders(texture);
	drawBreadModels();
	playBreadEatenEffect(cam);

	glColor3f(1, 1, 1);

	//����
	drawUIText(cam, 0, 0);


	cam->updateCameraMovement();
	detectBreadBeingEaten(cam);

}

void reshape(int width, int height) {
	if (height == 0) {     //����߶�Ϊ0 
		height = 1;        //�ø߶�Ϊ1��������ַ�ĸΪ0������      
	}

	cam->resetWinSize(width, height);    
}

void idle() {
	glutPostRedisplay();
}

void initLightingAndTexture() {
	glEnable(GL_DEPTH_TEST);//������Ȳ���        
	glEnable(GL_LIGHTING);  //�������ģʽ   
	glGenTextures(8, texture);
	loadTex(0, "Textures/11.bmp", texture);    //�ذ�
	loadTex(1, "Textures/14.bmp", texture);    //����

	//ѡ����������һ����պ���ͼ

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

//���յ��ʱ
void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouseLastPosX = x;
		mouseLastPosY = y;
	}
}

//��������ƶ�ʱ
void mouseMove(int x, int y) {
	//����������Ծ���
	float pitch = (float)(y - mouseLastPosY) * Sensitivity;
	float yaw = (float)(x - mouseLastPosX) * Sensitivity;
	mouseLastPosY = y;
	mouseLastPosX = x;
	cam->rotate(pitch, yaw);
}

void redraw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ����Ȼ���   
	
	drawScene();//���Ƴ���   
	glutSwapBuffers();//����������  
}

void initializeGL() {
	cam = new FPSCamera();
	//�����ײ��Ե
	cam->setSceneOuterBoundary(-roomSizeX / 2.0, -roomSizeZ / 2.0, roomSizeX / 2.0, roomSizeZ / 2.0);

	cam->setSceneInnerBoundary(-roomSizeX / 2.0,  -roomSizeY / 2.0f - 1.f, -roomSizeZ / 2.0,
		roomSizeX / 2.0, -roomSizeY / 2.0f, roomSizeZ / 2.0);    //�ذ�collider

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

	glutDisplayFunc(redraw);               //ע����ƻص�����
	glutReshapeFunc(reshape);              //ע���ػ�ص�����
	glutKeyboardFunc(normalKeyPress);      //ע����ͨ�����ص�����
	glutKeyboardUpFunc(normalKeyUp);
	glutMouseFunc(mouseClick);             //ע��������ص�����
	glutMotionFunc(mouseMove);             //ע�������&�ƶ��ص�����
	glutIdleFunc(idle);                    //ע��ȫ�ֻص�����������ʱ����       

	initLightingAndTexture();
	glutMainLoop();                        // glut�¼�����ѭ��    

	delete cam;
	deleteBreadModels();

	return 0;
}