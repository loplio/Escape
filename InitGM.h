#pragma once
#include "metrix.h"
#include "stb_image.h"

void InitTexture();
void InitBuffer();
void make_vertexShaders(const char* v_glsl);
void make_fragmentShaders(const char* f_glsl);
GLuint make_shaderProgram(const char* v_glsl, const char* f_glsl);
char* filetobuf(const char* file);
