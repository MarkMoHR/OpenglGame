#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include "FPSCamera.h"

#define roomSizeX 200.f
#define roomSizeY 60.f
#define roomSizeZ 200.f

#define SkyboxSize 600.f

void loadTex(int i, char *filename, GLuint* texture);

void drawRect(GLuint texture);
void drawCube(GLuint texture);

void drawSkybox(GLuint* texture);                  //绘制天空盒

void initBoxCollidersProperty();                   //设置盒子的位置和大小
void drawBoxColliders(GLuint* texture);            //绘制盒子
void setBoxColliderBoundary(FPSCamera* cam);       //设置盒子碰撞边缘