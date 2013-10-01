#ifndef ____gl_functions__
#define ____gl_functions__

#include <gl\gl.h>			// заголовочный файл для OpenGL32 библиотеки
#include <gl\glu.h>			// заголовочный файл для GLu32 библиотеки
#include <gl\glaux.h>		// заголовочный файл для GLaux библиотеки
#include <GL\glut.h>
#include "def.h"

void glPrintText(unsigned int base, char * string);
GLvoid DrawArrow(float x, float y, float size, int col);
GLvoid DrawGLScene(GLvoid);
GLvoid CFont(char * fontName, int Size);
Streight GetLine(Point P, Point Q);
Point Interspection(Streight one, Streight two);
Point InterCircle(float Radius, Streight line);
GLvoid WinCoordToObj(int x, int y, GLdouble* obx, GLdouble* oby, GLdouble* obz);

#endif /* defined(____gl_functions__) */
