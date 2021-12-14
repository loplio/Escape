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
BOOL Collide();
int Picking(int xCursor, int yCursor);
void MakeFile(const char* objfile, const char* mtlfile);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void PlayBg(std::string path);
void PlayEf(std::string path);
void removeBg(std::string path);
void pMotion(int x, int y);
void Jump();
void PlayBg_R(std::string path);
void Sabo();
void Timer();

