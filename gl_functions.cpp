#include "gl_functions.h"

void glPrintText(unsigned int base, char * string)
{
	if((base == 0 || string == NULL))
		return;
	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(string), GL_UNSIGNED_BYTE, string);
	glPopAttrib();
}

GLvoid DrawArrow(float x, float y, float size, int col)
{
	GLfloat color[2][3] = {{YELLOW}, {WHITE}};
	float mul = size * arrow_h;
	glBegin(GL_TRIANGLES);
    glColor3fv(color[col]);
    glVertex3f(x - mul / 4, y - mul / 4, - 1.5f);
    glColor3fv(color[col]);
    glVertex3f(x, y, -1.5f);
    glColor3fv(color[col]);
    glVertex3f(x + mul / 4, y - mul / 4, - 1.5f);
	glEnd();
	glBegin(GL_QUADS);
    glColor3fv(color[col]);
    glVertex3f(x - mul / 8, y - mul / 4, -1.5f);
    glColor3fv(color[col]);
    glVertex3f(x + mul / 8, y - mul / 4, -1.5f);
    glColor3fv(color[col]);
    glVertex3f(x + mul / 8, y - mul , -1.5f);
    glColor3fv(color[col]);
    glVertex3f(x - mul / 8, y - mul , -1.5f);
	glEnd();
}

GLvoid DrawGLScene(GLvoid)
{
	GLUquadricObj *quadObj = gluNewQuadric();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// очистка экрана и буфера глубины
	glLoadIdentity();
	glColor3f(0.5, 0.0, 0.0);
	
	glLoadIdentity();
	glColor3f(RED);
	glTranslatef(- F, 0.0f, 0.0f);
	glTranslatef(- 0.02, - 0.06, -1.5f);
	glRasterPos2i(0, 0);
	glPrintText(base, "O");
	glTranslatef(F, 0, 0);
	glRasterPos2i(0, 0);
	glPrintText(base, "F");
	glTranslatef(F + 0.03 , 0, 0);
	glRasterPos2i(0, 0);
	glPrintText(base, "P");
    
	glLoadIdentity();
	glTranslatef(- F, 0.0f, 0.0f);
	glTranslatef(0.0, 0, -1.5);
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluPartialDisk(quadObj, RFAT, R, 200, 200, ANGLE, ANGLE2);	// зеркало
	gluDisk(quadObj, 0.0, 0.008, 20, 20);	// центр зеркала
	glTranslatef(F, 0.0f, 0.0f);
	gluDisk(quadObj, 0.0, 0.008, 20, 20);   // фокус
	glTranslatef(F, 0.0f, 0.0f);
	gluDisk(quadObj, 0.0, 0.008, 20, 20);   // полюс
	glLoadIdentity();
	glTranslatef(- F, 0.0f, 0.0f);
	glLineWidth(1);
	glBegin(GL_LINES);
    glVertex3f(-2, 0, -1.5);	// ГОО
    glVertex3f(2, 0, -1.5);		//
	glEnd();
	glLoadIdentity();
    glTranslatef(- F, 0.0f, 0.0f);
	glTranslatef(cx, cy, -1.5);
	glColor3f(GREEN);
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	//gluDisk(quadObj, 0.0, .005, 100, 100); //  точка
	glLoadIdentity();
    glTranslatef(- F, 0.0f, 0.0f);
    
	if(touch) {
		lineA_P.x = cx; lineA_P.y = - cy; // ч/з полюс отраженный
		lineA_Q.x = R;  lineA_Q.y = 0;
        
		if(cy < ang_y && cy > - ang_y) {
			lineB_P.x = R / 2 * (2 - R / sqrt(R * R - cy * cy));	lineB_P.y = 0; // || ГОО
			lineB_Q.x = sqrt(R * R - cy * cy); lineB_Q.y = cy;
		}
		else {
			lineB_P.x = cx; lineB_P.y = cy; // ч/з фокус до отражения
			lineB_Q.x = F;  lineB_Q.y = 0;
			interspect = InterCircle(R, GetLine(lineB_P, lineB_Q)); // пересечение с зеркалом
			lineB_P.x = 0; lineB_P.y = interspect.y;				// ч/з фокус отраженный
			lineB_Q.x = interspect.x;  lineB_Q.y = interspect.y;
		}
        
		interpoint = Interspection(GetLine(lineA_P, lineA_Q), GetLine(lineB_P, lineB_Q));
		sx = interpoint.x, sy = interpoint.y;
		tx = cx, ty = 0;
		touch = false;
	}
	DrawArrow(cx, cy, 1, 0);
	glRotatef(180, 0.0, 0.0, - 1.5);
	DrawArrow(- sx, - sy, - sy / arrow_h, 1);
    
	glLoadIdentity();
    glTranslatef(- F, 0.0f, 0.0f);
    
	glLineWidth(1.7);
	// ч/з полюс
    glColor3f(GREEN);
    glBegin(GL_LINES);
    glVertex3f(cx, cy, -1.5f);
    glVertex3f(R, 0.0f, -1.5f);
    glEnd();
	// || ГОО
    glColor3f(BLUE);
    glBegin(GL_LINES);
    glVertex3f(cx, cy, -1.5f);
    glVertex3f(lineB_Q.x, lineB_Q.y, -1.5f);
    glEnd();
	if(cx > F) { glEnable(GL_LINE_STIPPLE); glLineStipple(1, 0x00FF); }
    glColor3f(GREEN);
    // отраженный полюс
    glBegin(GL_LINES);
    glVertex3f(R, 0.0f, -1.5f);
    glVertex3f(sx, sy, -1.5f);
    glEnd();
    glColor3f(BLUE);
    // отраженный через фокус
    glBegin(GL_LINES);
    glVertex3f(lineB_Q.x, lineB_Q.y, -1.5f);
    glVertex3f(sx, sy, -1.5f);
    glEnd();
	if(cx > F) glDisable(GL_LINE_STIPPLE);
    glColor3f(GREEN);
	if(cx > F)	{
        glBegin(GL_LINES);
        glVertex3f(R, 0.0f, -1.5f);
        glVertex3f(cx, - cy, -1.5f);
		glEnd();
	}
    glColor3f(BLUE);
	if(cx > F)  { glEnable(GL_LINE_STIPPLE); glLineStipple(1, 0x00FF);
		glBegin(GL_LINES);
        glVertex3f(lineB_Q.x, lineB_Q.y, -1.5f);
        glVertex3f(0, - GetLine(lineB_P, lineB_Q).C / GetLine(lineB_P, lineB_Q).B, -1.5f);
		glEnd();
	}
	glDisable(GL_LINE_STIPPLE);
    
	glLoadIdentity();
	gluDeleteQuadric(quadObj);
}

GLvoid CFont(char * fontName, int Size)
{
	base = glGenLists(224);
	hFont = CreateFont(Size, 0, 0, 0, FW_BOLD,
                       FALSE, FALSE, FALSE,
                       ANSI_CHARSET,
                       OUT_TT_PRECIS,
                       CLIP_DEFAULT_PRECIS,
                       ANTIALIASED_QUALITY,
                       FF_DONTCARE | DEFAULT_PITCH,
                       (LPCWSTR)fontName);
	SelectObject(hDC, hFont);
	wglUseFontBitmaps(hDC, 32, 224, base);
	DeleteObject(hFont);
}

Streight GetLine(Point P, Point Q)
{
	Streight ans;
	ans.A = P.y - Q.y;
	ans.B = Q.x - P.x;
	ans.C = - ans.A * P.x - ans.B * P.y;
	float z = sqrt(ans.A * ans.A + ans.B * ans.B);
	ans.A /= z;
	ans.B /= z;
	ans.C /= z;
	return ans;
}

Point Interspection(Streight one, Streight two)
{
	Point ans;
	float tmp = one.A * two.B - two.A * one.B;
	ans.x = - (one.C * two.B - two.C * one.B) / tmp;
	ans.y = - (one.A * two.C - two.A * one.C) / tmp;
	return ans;
}

Point InterCircle(float Radius, Streight line)
{
	float x0, y0, tmp = line.A * line.A + line.B * line.B;
	x0 = - line.A * line.C / tmp;
	y0 = - line.B * line.C / tmp;
	float d = sqrt(Radius * Radius - line.C * line.C / tmp);
	float mult = sqrt(d * d / tmp);
	Point ans;
	if(cx < F) {
		ans.x = x0 + line.B * mult;
		ans.y = y0 - line.A * mult;
	}
	else {
		ans.x = x0 - line.B * mult;
		ans.y = y0 + line.A * mult;
	}
	return ans;
}

GLvoid WinCoordToObj(int x, int y, GLdouble* obx, GLdouble* oby, GLdouble* obz)
{
	GLint view[4]; //для окна
	GLdouble mv_matrix[16];
	GLdouble proj_matrix[16];
    
	glGetIntegerv(GL_VIEWPORT, view);
	glGetDoublev(GL_MODELVIEW_MATRIX, mv_matrix);
	glGetDoublev(GL_PROJECTION_MATRIX, proj_matrix);
	gluUnProject(x, y, 0, mv_matrix, proj_matrix, view, obx, oby, obz);
}
