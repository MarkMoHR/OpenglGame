#include <iostream>
#include <cmath>
#include <gl/freeglut.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "FPSCamera.h"
using namespace std;

#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) < (y) ? (y) : (x))

#define HeroHeight 7.5f

#define GravityAcceler -9.8f

#define MoveSpeed 0.01f
#define BoundaryGap 1.0f
#define JumpInitialSpeed 12.0f
#define JumpFactor 0.004f
#define GravityFactor 0.004f

FPSCamera::FPSCamera() {
	isWPressing = isSPressing = isAPressing = isDPressing = isJumping = false;

	pfov = 45.0;
	pratio = 1.0;
	pnear = 1.0;
	pfar = 150.0;

	roll = 0;
	pitch = 0;
	yaw = 0;

	cameraPos = glm::vec3(-35.0f, 4.f, 40.0f);
	targetPos = glm::vec3(-35.0f, 4.f, 35.0f);
	velocity = glm::vec3(0.f, 0.f, 0.f);
	gravity = glm::vec3(0.f, GravityAcceler, 0.f);
	accelerUp = glm::vec3(0.f, 0.f, 0.f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(pfov, pratio, pnear, pfar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	viewMatrix = glm::lookAt(
		cameraPos,
		targetPos,
		glm::vec3(0.f, 1.0f, 0.f)
	);
	glMultMatrixf((float*)glm::value_ptr(viewMatrix));
}

void FPSCamera::resetWinSize(int w, int h) {
	winW = w;
	winH = h;
	pratio = (GLfloat)w / (GLfloat)h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(pfov, pratio, pnear, pfar);

	glMatrixMode(GL_MODELVIEW);
	updateView();
}

void FPSCamera::setSceneOuterBoundary(float x1, float z1, float x2, float z2) {
	outerBoundary = glm::vec4(x1, z1, x2, z2);
}

void FPSCamera::setSceneInnerBoundary(float x1, float y1, float z1, float x2, float y2, float z2) {
	glm::vec3 key(x1 - BoundaryGap, y1 - BoundaryGap, z1 - BoundaryGap);
	glm::vec3 value(x2 + BoundaryGap, y2 + BoundaryGap, z2 + BoundaryGap);

	innerBoundaryMin.push_back(key);
	innerBoundaryMax.push_back(value);
}

void FPSCamera::updateView() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glm::mat4 matroll = glm::rotate(glm::mat4(1.0), -roll, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 matpitch = glm::rotate(glm::mat4(1.0), -pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 matyaw = glm::rotate(glm::mat4(1.0), -yaw, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 mattranslate = glm::translate(glm::mat4(1.0f), -cameraPos);

	viewMatrix = matroll * matpitch * matyaw * mattranslate;

	glMultMatrixf((float*)glm::value_ptr(viewMatrix));
}

void FPSCamera::updateCameraMovement() {
	updateCameraHoriMovement();
	updateCameraVertMovement();
	updateView();
}

void FPSCamera::updateCameraHoriMovement() {
	float dx = 0;
	float dz = 0;

	if (isWPressing)
		dz += 2;
	if (isSPressing)
		dz -= 2;
	if (isAPressing)
		dx -= 2;
	if (isDPressing)
		dx += 2;

	if (dz != 0 || dx != 0) {

		//行走不改变y轴坐标
		glm::vec3 forward = glm::vec3(viewMatrix[0][2], 0.f, viewMatrix[2][2]);
		glm::vec3 strafe = glm::vec3(viewMatrix[0][0], 0.f, viewMatrix[2][0]);

		cameraPos += (-dz * forward + dx * strafe) * MoveSpeed;
		targetPos = cameraPos + (-dz * forward + dx * strafe) * 1.5f;

		//每次做完坐标变换后，先进行碰撞检测来调整坐标
		outCollisionTest(outerBoundary[0], outerBoundary[1], outerBoundary[2], outerBoundary[3]);
		//后面可以在这里添加：预处理，排除当前肯定不会产生碰撞的物体
		for (int i = 0; i < innerBoundaryMin.size(); i++) {
			inCollisionTestWithHeight(innerBoundaryMin[i][0], innerBoundaryMin[i][1], innerBoundaryMin[i][2],
				innerBoundaryMax[i][0], innerBoundaryMax[i][1], innerBoundaryMax[i][2]);
		}
	}
}

void FPSCamera::inCollisionTestWithHeight(float x1, float y1, float z1, float x2, float y2, float z2) {
	if (!(cameraPos[1] <= y1 || cameraPos[1] - HeroHeight >= y2)) {
		inCollisionTest(x1, z1, x2, z2);
	}
}

//判断在xz平面，相机位置是否位于碰撞体内部
bool insideTheCollider(glm::vec3 _cameraPos, glm::vec3 _innerMin, glm::vec3 _innerMax) {
	float camX = _cameraPos.x;
	float camZ = _cameraPos.z;
	float minX = _innerMin.x;
	float minZ = _innerMin.z;
	float maxX = _innerMax.x;
	float maxZ = _innerMax.z;

	if (minX <= camX && camX <= maxX && minZ <= camZ && camZ <= maxZ)
		return true;
	else
		return false;
}

void FPSCamera::updateCameraVertMovement() {
	glm::vec3 acceleration = gravity + accelerUp;
	velocity += acceleration * GravityFactor;
	cameraPos += velocity * JumpFactor;
	targetPos += velocity * JumpFactor;

	//cout << "velocity " << velocity.y << endl;
	//if (abs(velocity.y) < 0.1f)
	//	cout << "#### cameraPos.y " << cameraPos.y << endl;

	//检测所有碰撞体
	for (int i = 0; i < innerBoundaryMin.size(); i++) {
		if (insideTheCollider(cameraPos, innerBoundaryMin[i], innerBoundaryMax[i])) {
			//cout << "inside the colliderXZ" << endl;
			if (cameraPos.y - HeroHeight <= innerBoundaryMax[i][1]) {    //接触到碰撞体
				//cout << "touch the colliderY" << endl;
				isJumping = false;
				accelerUp.y = -GravityAcceler;
				velocity.y = 0.f;
				cameraPos.y = innerBoundaryMax[i][1] + HeroHeight;
				break;
			}
			else {
				accelerUp.y = 0.f;
			}
		}
		else {
			accelerUp.y = 0.f;
		}
	}
}

void FPSCamera::keyPressed(const unsigned char key) {
	switch (key) {
	case ' ':
		cout << "space press!" << endl;
		if (!isJumping) {
			velocity += glm::vec3(0.f, JumpInitialSpeed, 0.f);
			accelerUp.y = 0.f;
		}
		isJumping = true;
		break;

	case 'W':
	case 'w':
		isWPressing = true;
		break;

	case 'S':
	case 's':
		isSPressing = true;
		break;

	case 'A':
	case 'a':
		isAPressing = true;
		break;

	case 'D':
	case 'd':
		isDPressing = true;
		break;
	default:
		break;
	}
}

void FPSCamera::keyUp(const unsigned char key) {
	switch (key) {
	case ' ':

		break;

	case 'W':
	case 'w':
		isWPressing = false;
		break;

	case 'S':
	case 's':
		isSPressing = false;
		break;

	case 'A':
	case 'a':
		isAPressing = false;
		break;

	case 'D':
	case 'd':
		isDPressing = false;
		break;
	default:
		break;
	}
}

void FPSCamera::rotate(GLfloat const pitchRad, GLfloat const yawRad) {
	pitch += pitchRad;
	yaw += yawRad;

	updateView();
}

void FPSCamera::outCollisionTest(float x1, float z1, float x2, float z2) {
	//先设置包围盒：比空间外部边缘小一点
	if (x1 < 0)
		x1 += 2;
	else x1 -= 2;

	if (x2 < 0)
		x2 += 2;
	else x2 -= 2;

	if (z1 < 0)
		z1 += 2;
	else z1 -= 2;

	if (z2 < 0)
		z2 += 2;
	else z2 -= 2;

	//如果目标位置出了包围盒，先放回来
	if (targetPos[0] < x1) {
		targetPos[0] = x1;
	}

	if (targetPos[0] > x2) {
		targetPos[0] = x2;
	}
	if (targetPos[2] < z1) {
		targetPos[2] = z1;
	}
	if (targetPos[2] > z2) {
		targetPos[2] = z2;
	}

	float distance = sqrt((cameraPos[0] - targetPos[0])*(cameraPos[0] - targetPos[0]) +
		(cameraPos[2] - targetPos[2])*(cameraPos[2] - targetPos[2]));

	//若视点与目标距离太小，则固定目标位置，视点沿正对目标的逆方向移动
	if (distance <= 2.0f) {
		cameraPos[0] = 2.0f*(cameraPos[0] - targetPos[0]) / distance + targetPos[0];
		cameraPos[2] = 2.0f*(cameraPos[2] - targetPos[2]) / distance + targetPos[2];
	}
	bool flag = false;

	//再检测视点是否出了包围盒，若是则放回
	if (cameraPos[0] < x1) {
		flag = true;
		cameraPos[0] = x1;
	}
	if (cameraPos[0] > x2) {
		flag = true;
		cameraPos[0] = x2;
	}
	if (cameraPos[2] < z1) {
		flag = true;
		cameraPos[2] = z1;
	}
	if (cameraPos[2] > z2) {
		flag = true;
		cameraPos[2] = z2;
	}

	//重复上述远离两点距离的操作
	if (flag) {
		distance = sqrt((cameraPos[0] - targetPos[0])*(cameraPos[0] - targetPos[0]) +
			(cameraPos[2] - targetPos[2])*(cameraPos[2] - targetPos[2]));

		if (distance <= 2.0f) {
			targetPos[0] = 2.0f*(targetPos[0] - cameraPos[0]) / distance + cameraPos[0];
			targetPos[2] = 2.0f*(targetPos[2] - cameraPos[2]) / distance + cameraPos[2];
		}
	}
}

struct dot {
	float x;
	float y;
	dot(float _x, float _y) :x(_x), y(_y) { }
};

double Direction(dot pi, dot pj, dot pk) {
	return (pk.x - pi.x)*(pj.y - pi.y) - (pj.x - pi.x)*(pk.y - pi.y);
}

bool OnSegment(dot pi, dot pj, dot pk) {
	if ((min(pi.x, pj.x) <= pk.x) && (pk.x <= max(pi.x, pj.x))
		&& (min(pi.y, pj.y) <= pk.y) && (pk.y <= max(pi.y, pj.y)))
		return true;
	else return false;
}

//检测线段相交快速算法
bool SegmentIntersect(dot p1, dot p2, dot p3, dot p4) {
	int d1, d2, d3, d4;
	d1 = Direction(p3, p4, p1);
	d2 = Direction(p3, p4, p2);
	d3 = Direction(p1, p2, p3);
	d4 = Direction(p1, p2, p4);
	if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2>0)) && ((d3>0 && d4 < 0) || (d3 < 0 && d4>0)))
		return true;
	else if (d1 == 0 && OnSegment(p3, p4, p1))
		return true;
	else if (d2 == 0 && OnSegment(p3, p4, p2))
		return true;
	else if (d3 == 0 && OnSegment(p1, p2, p3))
		return true;
	else if (d4 == 0 && OnSegment(p1, p2, p4))
		return true;
	else 
		return false;
}

void FPSCamera::inCollisionTest(float x1, float z1, float x2, float z2) {
	const float d = 2.0f;
	float tarX = targetPos[0], camX = cameraPos[0], tarZ = targetPos[2], camZ = cameraPos[2];
	float len = sqrt((camX - tarX)*(camX - tarX) + (camZ - tarZ)*(camZ - tarZ));

	dot d1(cameraPos[0], cameraPos[2]), d2(targetPos[0], targetPos[2]);
	dot d3(x1, z1), d4(x1, z2), d5(x2, z1), d6(x2, z2);

	if (SegmentIntersect(d1, d2, d4, d6)) {
		if (targetPos[2] < cameraPos[2]) {
			printf("1\n");

			//利用相似三角形原理计算，
			//仅改变z坐标
			targetPos[2] = z2;
			cameraPos[2] += (targetPos[2] - tarZ);
		}
		else if (targetPos[2] > cameraPos[2]) {
			printf("2\n");
			cameraPos[2] = z2;
			targetPos[2] += (cameraPos[2] - camZ);
		}
	}
	else if (SegmentIntersect(d1, d2, d5, d6)) {
		if (targetPos[0]<cameraPos[0]) {
			printf("3\n");
			targetPos[0] = x2;
			cameraPos[0] += (targetPos[0] - tarX);
		}
		else if (targetPos[0]>cameraPos[0]) {
			printf("4\n");
			cameraPos[0] = x2;
			targetPos[0] += (cameraPos[0] - camX);
		}
	}
	else if (SegmentIntersect(d1, d2, d3, d5)) {
		if (targetPos[2] > cameraPos[2]) {
			printf("5\n");
			targetPos[2] = z1;
			cameraPos[2] += (targetPos[2] - tarZ);
		}
		else if (targetPos[2] < cameraPos[2]) {
			printf("6\n");
			cameraPos[2] = z1;
			targetPos[2] += (cameraPos[2] - camZ);
		}
	}
	else if (SegmentIntersect(d1, d2, d3, d4)) {
		if (targetPos[0] > cameraPos[0]) {
			printf("7\n");
			targetPos[0] = x1;
			cameraPos[0] += (targetPos[0] - tarX);
		}
		else if (targetPos[0] < cameraPos[0]) {
			printf("8\n");
			cameraPos[0] = x1;
			targetPos[0] += (cameraPos[0] - camX);
		}
	}
}

