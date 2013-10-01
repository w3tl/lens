#ifndef ____def__
#define ____def__

#define RED		1.0f, 0.0f,  0.0f
#define GREEN	0.0f, 1.0f,  0.0f
#define BLUE	0.0f, 0.0f,  1.0f
#define WHITE	1.0f, 1.0f,  1.0f
#define CYAN	0.0f, 1.0f,	 1.0f
#define AMBER	1.0f, 0.75f, 0.0f
#define YELLOW	1.0f, 1.0f,  0.0f

#define Z		1.5f

#define R		0.4f
#define R_2		0.2f
#define F		R_2
#define FAT		0.007
#define RFAT	R - FAT
#define ANGLE	45
#define	ANGLE2	90
#define ang_y	R * 0.707106781186548

struct Point {
	float x, y;
};

struct Streight {
	float A, B, C;
};

#endif /* defined(____def__) */
