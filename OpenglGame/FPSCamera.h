#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <vector>
#include "PhysicsEngine.h"
using namespace std;

class FPSCamera {
	public:

		FPSCamera();
		~FPSCamera();
		void resetWinSize(int w, int h);    //���ڴ�С�����仯ʱ�ص�

		void rotate(GLfloat const pitchRad, GLfloat const yawRad);
		void keyPressed(const unsigned char key);       //��������
		void keyUp(const unsigned char key);            //����̧��

		void setSceneOuterBoundary(float x1, float z1, float x2, float z2);    //���ÿռ��ⲿ��Ե
		void setSceneInnerBoundary(float x1, float y1, float z1, float x2, float y2, float z2);    //���ÿռ��ڲ���Ե

		void updateCameraMovement();    //ÿ֡���Ƶ�ʱ�����������ƶ�

		glm::vec3 getForward();

		//���������λ���ж��Ƿ��ܳԵ����
		bool detectPlayerEatingBread(glm::vec3 breadPos, float dist);

		glm::vec3 cameraPos;
		glm::vec3 targetPos;

		//Current view matrix
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

	private:
		void updateCameraHoriMovement();        //ÿ֡���Ƶ�ʱ����������ˮƽ�����ƶ�
		void updateCameraVertMovement();        //ÿ֡���Ƶ�ʱ������������ֱ�����ƶ�
		void updateView();                      //�����ӽ�

		int winH = 0;
		int winW = 0;

		bool isWPressing, isSPressing, isAPressing, isDPressing;


		//Lens parameters for the camera
		GLfloat pfov, pratio, pnear, pfar;
		
		//Camera roll, pitch, yaw info.
		GLfloat roll, pitch, yaw;

		PhysicsEngine* physicsEngine;

};