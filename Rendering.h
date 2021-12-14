#pragma once
#include "metrix.h"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
void special(int key, int x, int y);
void TimerFunction(int value);
void Update();
BOOL isInside(glm::vec2 B);
void MakeFile(const char* objfile, const char* mtlfile);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Motion2(int x, int y);