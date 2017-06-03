#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <vector>
#include "PhysicsEngine.h"
using namespace std;

class FPSCamera {
	public:

		FPSCamera();
		~FPSCamera();
		void resetWinSize(int w, int h);    //窗口大小发生变化时回调

		void rotate(GLfloat const pitchRad, GLfloat const yawRad);
		void keyPressed(const unsigned char key);       //按键按下
		void keyUp(const unsigned char key);            //按键抬起

		void setSceneOuterBoundary(float x1, float z1, float x2, float z2);    //设置空间外部边缘
		void setSceneInnerBoundary(float x1, float y1, float z1, float x2, float y2, float z2);    //设置空间内部边缘

		void updateCameraMovement();    //每帧绘制的时候更新摄像机移动

		glm::vec3 getForward();

		//根据面包的位置判断是否能吃掉面包
		bool detectPlayerEatingBread(glm::vec3 breadPos, float dist);

		glm::vec3 cameraPos;
		glm::vec3 targetPos;

		//Current view matrix
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

	private:
		void updateCameraHoriMovement();        //每帧绘制的时候更新摄像机水平方向移动
		void updateCameraVertMovement();        //每帧绘制的时候更新摄像机垂直方向移动
		void updateView();                      //更新视角

		int winH = 0;
		int winW = 0;

		bool isWPressing, isSPressing, isAPressing, isDPressing;


		//Lens parameters for the camera
		GLfloat pfov, pratio, pnear, pfar;
		
		//Camera roll, pitch, yaw info.
		GLfloat roll, pitch, yaw;

		PhysicsEngine* physicsEngine;

};