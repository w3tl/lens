#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

#include <math.h>

#include "gl_functions.h"
#include "win_functions.h"

static HGLRC hRC;		// постоянный контекст рендеринга
static HDC hDC;			// приватный контекст устройства GDI

float arrow_h = 0.07, tmpx, tmpy;

Point		interspect;
Point		interpoint;
Point		lineA_P, lineA_Q, lineB_P, lineB_Q, lineC_P, lineC_Q, lineD_P, lineD_Q;
Streight    line;
float		ratio = 2 * Z * tan(45 * 3.14159265f / 360.0f);
int			w = 640, h = 480;
GLdouble	obj_coord[] = {0.5, 0.5, 0.0};
GLdouble	cx = 0.0, cy = arrow_h, cz = - 1.0, sx, sy, tx, ty, dx, dy;

bool	down = false;
bool	fullscreen = 0, touch = true;
bool	keys[256];		// массив для процедуры обработки клавиатуры

HFONT hFont;
GLuint base;



GLvoid InitGL(GLsizei Width, GLsizei Height)	// вызвать после создания окна GL
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // очистка экрана в черный цвет
	glClearDepth(1.0);		// разрешить очистку буфера глубины
	glDepthFunc(GL_LESS);	// тип теста глубины
	glEnable(GL_DEPTH_TEST);// разрешить тест глубины
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);// разрешить плавное цветовое сглаживание
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glMatrixMode(GL_PROJECTION);// выбор матрицы проекции
	glLoadIdentity();		// сброс матрицы проекции
	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,1.0f,2.0f);
							// вычислить соотношение геометрических размеров для окна
	glMatrixMode(GL_MODELVIEW);// выбор матрицы просмотра модели
	CFont("Times New Roman", 26);
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
	if (Height==0)		// предотвращение деления на ноль, если окно слишком мало
		Height=1;
	w = Width; h = Height;
	glViewport(0, 0, Width, Height);
		// сброс текущей области вывода и перспективных преобразований
	glMatrixMode(GL_PROJECTION);// выбор матрицы проекций
	glLoadIdentity();			// сброс матрицы проекции
	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,1.0f,2.0f);
		// вычисление соотношения геометрических размеров для окна
	glMatrixMode(GL_MODELVIEW);	// выбор матрицы просмотра модели
	glLoadIdentity();
}

