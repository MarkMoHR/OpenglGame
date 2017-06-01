#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
using namespace std;

#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) < (y) ? (y) : (x))

#define HeroHeight 7.5f           //玩家视点到脚的高度

#define GravityAcceler -9.8f

#define MoveSpeed 0.15f           //玩家移动速度
#define BoundaryGap 1.0f          //碰撞间距
#define JumpInitialSpeed 12.0f    //起跳初速度
#define JumpFactor 0.04f          //跳起速度系数
#define GravityFactor 0.04f       //下落速度系数

struct dot {
	float x;
	float y;
	dot(float _x, float _y) :x(_x), y(_y) { }
};

class PhysicsEngine {
public:
	PhysicsEngine();
	~PhysicsEngine();

	//设置空间外部边缘
	void setSceneOuterBoundary(float x1, float z1, float x2, float z2);
	//外部碰撞检测
	void outCollisionTest(glm::vec3 & cameraPos, glm::vec3 & targetPos);
	//设置空间内部边缘
	void setSceneInnerBoundary(float x1, float y1, float z1, float x2, float y2, float z2);
	//内部碰撞检测
	void inCollisionTest(glm::vec3 & cameraPos, glm::vec3 & targetPos);

	bool isJumping;
	void jumpAndUpdateVelocity();    //按下space跳跃时调用
	//每帧绘制的时候更新摄像机垂直方向移动
	void updateCameraVertMovement(glm::vec3 & cameraPos, glm::vec3 & targetPos);

private:
	//空间内部边缘碰撞检测（考虑高度）
	void inCollisionTestWithHeight(float x1, float y1, float z1, float x2, float y2, float z2, glm::vec3 & cameraPos, glm::vec3 & targetPos);
	//空间内部边缘碰撞检测（不考虑高度，即XZ平面）
	void inCollisionTestXZ(float x1, float z1, float x2, float z2, glm::vec3 & cameraPos, glm::vec3 & targetPos);
	//空间外部边缘碰撞检测
	void outCollisionTestXZ(float x1, float z1, float x2, float z2, glm::vec3 & cameraPos, glm::vec3 & targetPos);

	glm::vec3 velocity;        //垂直方向速度
	glm::vec3 gravity;         //重力加速度
	glm::vec3 accelerUp;       //方向向上的加速度

	glm::vec4 outerBoundary;
	vector<glm::vec3> innerBoundaryMin;    //碰撞器小的x/y/z坐标
	vector<glm::vec3> innerBoundaryMax;    //碰撞器大的x/y/z坐标
};

#endif // !PHYSICSENGINE_H



