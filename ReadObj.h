#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "metrix.h"
typedef struct ObjFile {
	int face_num;
	char group_name[15];
} object;
object* ReadObj(FILE* objFile, object* obj, int* TriNum);