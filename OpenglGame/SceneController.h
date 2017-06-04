#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include "Model.h"
#include "FPSCamera.h"

#define roomSizeX 200.f
#define roomSizeY 60.f
#define roomSizeZ 200.f

#define SkyboxSize 600.f        //天空盒大小

#define EatBreadDistance 3.f    //吃掉面包的距离

void loadTex(int i, char *filename, GLuint* texture);

void drawRect(GLuint texture);
void drawCube(GLuint texture);

void drawSkybox(GLuint* texture);                  //绘制天空盒

void initBoxCollidersProperty();                   //设置盒子的位置和大小
void initSingleBoxCollider(glm::vec3 pos, glm::vec3 scalar);    //设置单个盒子的位置和大小
void setBoxColliderBoundary(FPSCamera* cam);       //设置盒子碰撞边缘
void drawBoxColliders(GLuint* texture);            //绘制盒子

void initBreadModels();        //初始化面包集
void drawBreadModels();        //绘制面包集
void playBreadEatenEffect(FPSCamera* cam);
void deleteBreadModels();      //销毁面包集

void detectBreadBeingEaten(FPSCamera* cam);      //检测面包是否被吃掉

void drawMenuSceneUIText(FPSCamera* cam);
void drawGameSceneUIText(FPSCamera* cam, int x, int y);             //绘制UI文字

void setupLights();            //设置光照