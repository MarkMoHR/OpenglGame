#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include "Model.h"
#include "FPSCamera.h"

#define roomSizeX 200.f
#define roomSizeY 60.f
#define roomSizeZ 200.f

#define SkyboxSize 600.f        //��պд�С

#define EatBreadDistance 3.f    //�Ե�����ľ���

void loadTex(int i, char *filename, GLuint* texture);

void drawRect(GLuint texture);
void drawCube(GLuint texture);

void drawSkybox(GLuint* texture);                  //������պ�

void initBoxCollidersProperty();                   //���ú��ӵ�λ�úʹ�С
void initSingleBoxCollider(glm::vec3 pos, glm::vec3 scalar);    //���õ������ӵ�λ�úʹ�С
void setBoxColliderBoundary(FPSCamera* cam);       //���ú�����ײ��Ե
void drawBoxColliders(GLuint* texture);            //���ƺ���

void initBreadModels();        //��ʼ�������
void drawBreadModels();        //���������
void playBreadEatenEffect(FPSCamera* cam);
void deleteBreadModels();      //���������

void detectBreadBeingEaten(FPSCamera* cam);      //�������Ƿ񱻳Ե�

void drawUIText(FPSCamera* cam, int x, int y);             //����UI����

void setupLights();            //���ù���