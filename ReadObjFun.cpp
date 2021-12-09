#include "ReadObj.h"

void New_Info(Mtl *info)
{
	info->Index = -1;
	info->name = NULL;
	info->map_Kd = NULL;
}
void Del_Info(Mtl *info)
{
	free(info->name);
	free(info->map_Kd);
	info->name = NULL;
	info->map_Kd = NULL;
}
void Del_Obj(object* obj, int objNum)
{
	for (int i = 0; i < objNum; i++) {
		Del_Info(&obj[i].info);
		free(obj[i].group_name);
		obj[i].group_name = NULL;
	}
}