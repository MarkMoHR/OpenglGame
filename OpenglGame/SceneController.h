#include <gl/freeglut.h>

#define roomSizeX 100  
#define roomSizeY 15  
#define roomSizeZ 100

void loadTex(int i, char *filename, GLuint* texture);

void drawRect(GLuint texture);
void drawCube(GLuint texture);