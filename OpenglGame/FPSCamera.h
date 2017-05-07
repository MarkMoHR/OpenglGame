#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <vector>
using namespace std;

class FPSCamera {
	public:

		FPSCamera();
		void resetWinSize(int w, int h);    //窗口大小发生变化时回调

		void setSceneOuterBoundary(float x1, float z1, float x2, float z2);    //设置空间外部边缘
		void setSceneInnerBoundary(float x1, float z1, float x2, float z2);    //设置空间内部边缘

		float speed;

		void rotate(GLfloat const pitchRad, GLfloat const yawRad);
		void pressed(const unsigned char key);

	private:

		//Taking changes into effect.
		void update();
		int winH = 0;
		int winW = 0;

		glm::vec4 outerBoundary;
		vector<glm::vec4> innerBoundary;

		//Current view matrix
		glm::mat4 viewMatrix;
		//Lens parameters for the camera
		GLfloat pfov, pratio, pnear, pfar;
		//Camera Position
		glm::vec3 cameraPos;
		//Target Position
		glm::vec3 targetPos;
		//Camera roll, pitch, yaw info.
		GLfloat roll, pitch, yaw;

		void inCollisionTest(float x1, float z1, float x2, float z2);     //空间内部边缘碰撞检测
		void outCollisionTest(float x1, float z1, float x2, float z2);    //空间外部边缘碰撞检测
};