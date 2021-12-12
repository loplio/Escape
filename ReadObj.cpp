#include "ReadObj.h"

extern GLfloat TriObj[F_ARRAY][S_ARRAY][24];
object* ReadObj(FILE* objFile, object* obj, int* TriNum)
{
	char count[100], bind[100];
	int vertexNum = 0;
	int textureNum = 0;
	int normalNum = 0;
	int faceNum = 0;
	int objectNum = 0;
	int mtlNum = 0;
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		//printf("%s\n", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexNum += 1;
		else if (count[0] == 'v' && count[1] == 'n')
			normalNum += 1;
		else if (count[0] == 'v' && count[1] == 't')
			textureNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			faceNum += 1;
		else if (count[0] == 'g' && count[1] == '\0') {
			objectNum += 1;
			memset(count, '\0', sizeof(count));
			fscanf(objFile, "%s", count);
		}
		else if (count[0] == 'u' && count[1] == 's')
			mtlNum += 1;
		else if (count[0] == 's' && count[1] == '\0')
			fscanf(objFile, "%s", count);
		memset(count, '\0', sizeof(count));
	}
	printf("v:%d vn:%d f:%d object개수:%d, usemtl개수:%d\n", vertexNum, normalNum, faceNum, objectNum, mtlNum);
	glm::vec3* vertex;
	glm::vec3* texture;
	glm::vec3* normal;
	glm::uvec4* v, * t, * n;
	int vertIndex = 1;
	int texIndex = 1;
	int normIndex = 1;
	int faceIndex = 0;
	int objIndex = 0;
	int poly_face_n = 0, pfaceIndex = 0;
	int t_n = 0, g_polygon_MAX = 0;
	obj = (object*)malloc(sizeof(object) * objectNum);
	v = (glm::uvec4*)malloc(sizeof(glm::vec4) * faceNum);
	t = (glm::uvec4*)malloc(sizeof(glm::vec4) * faceNum);
	n = (glm::uvec4*)malloc(sizeof(glm::vec4) * faceNum);
	vertex = (glm::vec3*)malloc(sizeof(glm::vec3) * (vertexNum + 1));
	texture = (glm::vec3*)malloc(sizeof(glm::vec3) * (textureNum + 1));
	normal = (glm::vec3*)malloc(sizeof(glm::vec3) * (normalNum + 1));
	rewind(objFile);
	while (!feof(objFile)) {
		fscanf(objFile, "%s", bind);

		if (bind[0] == 'v' && bind[1] == '\0') {
			fscanf(objFile, "%f %f %f",
				&vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
			vertIndex++;
		}
		else if (bind[0] == 'v' && bind[1] == 'n') {
			fscanf(objFile, "%f %f %f",
				&normal[normIndex].x, &normal[normIndex].y, &normal[normIndex].z);
			normIndex++;
		}
		else if (bind[0] == 'v' && bind[1] == 't') {
			fscanf(objFile, "%f %f %f",
				&texture[texIndex].x, &texture[texIndex].y, &texture[texIndex].z);
			texIndex++;
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d",
				&v[faceIndex].x, &t[faceIndex].x, &n[faceIndex].x, &v[faceIndex].y, &t[faceIndex].y, &n[faceIndex].y, &v[faceIndex].z, &t[faceIndex].z, &n[faceIndex].z);
			faceIndex++;
		}
		else if (bind[0] == 'g' && bind[1] == '\0');
		else if (!strcmp(bind, "object") || bind[0] == '\0') {
			if (faceIndex != 0) {
				t_n = objIndex - 1;
				obj[t_n].face_num = faceIndex - pfaceIndex;
				poly_face_n = faceIndex - pfaceIndex;
				pfaceIndex = faceIndex;
				if (poly_face_n > g_polygon_MAX)
					g_polygon_MAX = poly_face_n;
				if (poly_face_n > S_ARRAY) {
					printf("/		TriObj Second Array overflow Error.(%d)		/\n", poly_face_n);
					//exit(0);
				}
				if (t_n > F_ARRAY) {
					printf("/		TriObj First Array overflow Error.(%d)		/\n", t_n);
					//exit(0);
				}
				for (int i = 0, j = faceIndex - poly_face_n; i < poly_face_n; i++, j++) {
					TriObj[t_n][i][0] = vertex[v[j].x].x, TriObj[t_n][i][1] = vertex[v[j].x].y, TriObj[t_n][i][2] = vertex[v[j].x].z;
					TriObj[t_n][i][3] = normal[n[j].x].x, TriObj[t_n][i][4] = normal[n[j].x].y, TriObj[t_n][i][5] = normal[n[j].x].z;
					TriObj[t_n][i][6] = texture[t[j].x].x, TriObj[t_n][i][7] = texture[t[j].x].y;

					TriObj[t_n][i][8] = vertex[v[j].y].x, TriObj[t_n][i][9] = vertex[v[j].y].y, TriObj[t_n][i][10] = vertex[v[j].y].z;
					TriObj[t_n][i][11] = normal[n[j].y].x, TriObj[t_n][i][12] = normal[n[j].y].y, TriObj[t_n][i][13] = normal[n[j].y].z;
					TriObj[t_n][i][14] = texture[t[j].y].x, TriObj[t_n][i][15] = texture[t[j].y].y;

					TriObj[t_n][i][16] = vertex[v[j].z].x, TriObj[t_n][i][17] = vertex[v[j].z].y, TriObj[t_n][i][18] = vertex[v[j].z].z;
					TriObj[t_n][i][19] = normal[n[j].z].x, TriObj[t_n][i][20] = normal[n[j].z].y, TriObj[t_n][i][21] = normal[n[j].z].z;
					TriObj[t_n][i][22] = texture[t[j].z].x, TriObj[t_n][i][23] = texture[t[j].z].y;
				}
			}
			if (bind[0] != '\0') {
				memset(bind, '\0', sizeof(bind));
				fscanf(objFile, "%s", bind);
				obj[objIndex].group_name = (char*)malloc(sizeof(char) * strlen(bind));
				strcpy(obj[objIndex].group_name, bind);
				New_Info(&obj[objIndex].info);
				//printf("%s\n", obj[objIndex].group_name);
				objIndex++;
			}
		}
		else if (bind[0] == 'u' && bind[1] == 's') {
			memset(bind, '\0', sizeof(bind));
			fscanf(objFile, "%s", bind);
			obj[objIndex-1].info.name = (char*)malloc(sizeof(char) * strlen(bind));
			strcpy(obj[objIndex-1].info.name, bind);
			//printf("usemtl:%s\n", obj[objIndex - 1].info.name);
		}
		if (faceIndex == faceNum)
			memset(bind, '\0', sizeof(bind));
	}
	printf("Up to Second ARRAYS %d are in use\n", g_polygon_MAX);
	*TriNum = objIndex;
	free(v), free(t), free(n), free(vertex), free(normal);
	return obj;
}