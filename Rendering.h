#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define F_ARRAY 20
#define S_ARRAY 240000
#include "metrix.h"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
void special(int key, int x, int y);
void TimerFunction(int value);
void Update();
void MakeFile(const char* objfile);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);