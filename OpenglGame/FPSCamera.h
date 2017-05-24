#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <vector>
using namespace std;

class FPSCamera {
	public:

		FPSCamera();
		void resetWinSize(int w, int h);    //窗口大小发生变化时回调

		void setSceneOuterBoundary(float x1, float z1, float x2, float z2);    //设置空间外部边缘
		void setSceneInnerBoundary(float x1, float y1, float z1, float x2, float y2, float z2);    //设置空间内部边缘

		void rotate(GLfloat const pitchRad, GLfloat const yawRad);
		void keyPressed(const unsigned char key);       //按键按下
		void keyUp(const unsigned char key);            //按键抬起

		void updateCameraMovement();    //每帧绘制的时候更新摄像机移动

	private:
		void updateCameraHoriMovement();        //每帧绘制的时候更新摄像机水平方向移动
		void updateCameraVertMovement();        //每帧绘制的时候更新摄像机垂直方向移动
		void updateView();                      //更新视角

		void inCollisionTest(float x1, float z1, float x2, float z2);     //空间内部边缘碰撞检测
		void inCollisionTestWithHeight(float x1, float y1, float z1, float x2, float y2, float z2);
		void outCollisionTest(float x1, float z1, float x2, float z2);    //空间外部边缘碰撞检测

		int winH = 0;
		int winW = 0;

		bool isWPressing, isSPressing, isAPressing, isDPressing;
		bool isJumping;

		glm::vec4 outerBoundary;
		vector<glm::vec3> innerBoundaryMin;    //碰撞器小的x/y/z坐标
		vector<glm::vec3> innerBoundaryMax;    //碰撞器大的x/y/z坐标

		//Current view matrix
		glm::mat4 viewMatrix;
		//Lens parameters for the camera
		GLfloat pfov, pratio, pnear, pfar;

		glm::vec3 cameraPos;
		glm::vec3 targetPos;
		//Camera roll, pitch, yaw info.
		GLfloat roll, pitch, yaw;

		glm::vec3 velocity;        //垂直方向速度
		glm::vec3 gravity;         //重力加速度
		glm::vec3 accelerUp;       //方向向上的加速度

};