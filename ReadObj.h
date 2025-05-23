#pragma once
#include "metrix.h"
typedef struct MtlFile {
	int Index;
	char* name;
	char* map_Kd;
	glm::vec3 Kd;
} Mtl;
typedef struct ObjFile {
	int face_num;
	char* group_name;
	Mtl info;
} object;
object* ReadObj(FILE* objFile, object* obj, int* TriNum);
void amongusLoad();
Mtl* ReadMtl(FILE* mtlfile, object* obj, int TriNum, int* image_Num, int* mtl_Num);
void New_Info(Mtl* info);
void Del_Info(Mtl* info);
void Del_Obj(object* obj, int TriNum);