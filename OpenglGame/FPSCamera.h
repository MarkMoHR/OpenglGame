#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
using namespace std;

class FPSCamera {
	public:

		FPSCamera();
		void resetWinSize(int w, int h);    //窗口大小发生变化时回调

		void setSceneOuterBoundary(float x1, float z1, float x2, float z2);    //设置空间外部边缘
		void setSceneInnerBoundary(float x1, float z1, float x2, float z2);    //设置空间内部边缘

		void setSceneInnerBoundaryMap(float x1, float y1, float z1, float x2, float y2, float z2);    //设置空间内部边缘

		void rotate(GLfloat const pitchRad, GLfloat const yawRad);
		void keyPressed(const unsigned char key);       //按键按下
		void keyUp(const unsigned char key);            //按键抬起

		void detectCameraMove();        //每帧绘制的时候检测摄像机是否移动

	private:

		//Taking changes into effect.
		void updateView();
		void detectJump();              //每帧绘制的时候检测是否跳跃

		int winH = 0;
		int winW = 0;

		bool isWPressing, isSPressing, isAPressing, isDPressing;
		bool isJumping;

		glm::vec4 outerBoundary;
		map<glm::vec3, glm::vec3> innerBoundaryMap;    //左键为小的x/y/z坐标，右键为大的x/y/z坐标
		vector<glm::vec4> innerBoundary;

		vector<glm::vec3> innerBoundaryMin;
		vector<glm::vec3> innerBoundaryMax;

		//Current view matrix
		glm::mat4 viewMatrix;
		//Lens parameters for the camera
		GLfloat pfov, pratio, pnear, pfar;

		glm::vec3 cameraPos;
		glm::vec3 targetPos;
		//Camera roll, pitch, yaw info.
		GLfloat roll, pitch, yaw;

		glm::vec3 velocity, gravity;


		void inCollisionTest(float x1, float z1, float x2, float z2);     //空间内部边缘碰撞检测
		void inCollisionTestWithHeight(float x1, float y1, float z1, float x2, float y2, float z2);
		void outCollisionTest(float x1, float z1, float x2, float z2);    //空间外部边缘碰撞检测
};