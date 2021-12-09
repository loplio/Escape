#include "ReadObj.h"

Mtl* ReadMtl(FILE* mtlFile, object* obj, int objNum, int* image_Num, int* mtl_Num)
{
	int mtlNum = 0;
	char count[100], bind[100];
	
	// 임시 저장 포인터 사이즈 측정
	while (!feof(mtlFile)) {
		fscanf(mtlFile, "%s", count);
		if (!strcmp(count, "newmtl"))
			mtlNum++;
	}
	Mtl* m = (Mtl*)malloc(sizeof(Mtl) * mtlNum);
	*mtl_Num = mtlNum;
	for (int i = 0; i < mtlNum; i++)
		New_Info(&m[i]);
	int mtlIndex = -1;
	int imageIndex = 0;
	rewind(mtlFile);

	// 임시 저장 포인터에 값 대입
	while (!feof(mtlFile)) {
		fscanf(mtlFile, "%s", bind);
		if (!strcmp(bind, "newmtl")) {
			//memset(bind, '\0', sizeof(bind));
			fscanf(mtlFile, "%s", bind);
			mtlIndex++;
			m[mtlIndex].name = (char*)malloc(sizeof(char) * strlen(bind));
			strcpy(m[mtlIndex].name, bind);
		}
		else if (!strcmp(bind, "map_Kd")) {
			//memset(bind, '\0', sizeof(bind));
			fscanf(mtlFile, "%s", bind);
			m[mtlIndex].map_Kd = (char*)malloc(sizeof(char) * strlen(bind));
			strcpy(m[mtlIndex].map_Kd, bind);
			m[mtlIndex].Index = imageIndex;
			imageIndex++;
		}
	}

	// 각 Object에 mtl정보 매칭
	for (int i = 0; i < objNum; i++) {
		for (int j = 0; j < mtlNum; j++) {
			if (!strcmp(obj[i].info.name, m[j].name)) {
				if (m[j].map_Kd != NULL) {
					obj[i].info.Index = m[j].Index;
					obj[i].info.map_Kd = (char*)malloc(sizeof(char) * strlen(m[j].map_Kd));
					strcpy(obj[i].info.map_Kd, m[j].map_Kd);
				}
				break;
			}
		}
	}
	*image_Num = imageIndex;
	return m;
}