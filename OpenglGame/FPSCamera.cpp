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

FPSCamera::FPSCamera() {
	speed = 0.3;

	pfov = 45.0;
	pratio = 1.0;
	pnear = 1.0;
	pfar = 150.0;

	roll = 0;
	pitch = 0;
	yaw = 0;

	cameraPos = glm::vec3(-35.0, 0, 40.0);
	targetPos = glm::vec3(-35.0, 0, 35.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(pfov, pratio, pnear, pfar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	viewMatrix = glm::lookAt(
		cameraPos,
		targetPos,
		glm::vec3(0.0, 1.0, 0.0)
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
	update();
}

void FPSCamera::setSceneOuterBoundary(float x1, float x2, float z1, float z2) {
	outerBoundary = glm::vec4(x1, x2, z1, z2);
}

void FPSCamera::setSceneInnerBoundary(float x1, float x2, float z1, float z2) {
	innerBoundary.push_back(glm::vec4(x1, x2, z1, z2));
}

void FPSCamera::update() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glm::mat4 matroll = glm::rotate(glm::mat4(1.0), -roll, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 matpitch = glm::rotate(glm::mat4(1.0), -pitch, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 matyaw = glm::rotate(glm::mat4(1.0), -yaw, glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 mattranslate = glm::translate(glm::mat4(1.0f), -cameraPos);

	viewMatrix = matroll * matpitch * matyaw * mattranslate;

	glMultMatrixf((float*)glm::value_ptr(viewMatrix));
}

void FPSCamera::pressed(const unsigned char key) {
	float dx = 0;
	float dz = 0;

	switch (key) {
		case ' ':
			cout << "space press!" << endl;
			break;

		case 'W':
		case 'w':
			dz = 2;
			break;

		case 'S':
		case 's':
			dz = -2;
			break;

		case 'A':
		case 'a':
			dx = -2;
			break;

		case 'D':
		case 'd':
			dx = 2;
			break;
		default:
			break;
	}

	//glm::vec3 forward = glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);
	//glm::vec3 strafe = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);

	//行走不改变y轴坐标
	glm::vec3 forward = glm::vec3(viewMatrix[0][2], 0, viewMatrix[2][2]);
	glm::vec3 strafe = glm::vec3(viewMatrix[0][0], 0, viewMatrix[2][0]);

	cameraPos += (-dz * forward + dx * strafe) * speed;
	targetPos = cameraPos + (-dz * forward + dx * strafe) * 2.0;

	//每次做完坐标变换后，先进行碰撞检测来调整坐标
	outCollisionTest(outerBoundary[0], outerBoundary[1], outerBoundary[2], outerBoundary[3]);
	//后面可以在这里添加：预处理，排除当前肯定不会产生碰撞的物体
	for (int i = 0; i < innerBoundary.size(); i++) {
		inCollisionTest(innerBoundary[i][0], innerBoundary[i][1], innerBoundary[i][2], innerBoundary[i][3]);
	}

	update();
}

void FPSCamera::rotate(GLfloat const pitchRad, GLfloat const yawRad) {
	pitch += pitchRad;
	yaw += yawRad;

	update();
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