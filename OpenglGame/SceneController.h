#include <gl/freeglut.h>

#define roomSizeX 100.f
#define roomSizeY 15.f
#define roomSizeZ 100.f

void loadTex(int i, char *filename, GLuint* texture);

void drawRect(GLuint texture);
void drawCube(GLuint texture);