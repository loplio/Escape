#include "ReadObj.h"

extern GLfloat amongus[34][24];
void amongusLoad()
{
	FILE* objFile;
	objFile = fopen("Obj/among_us/red.obj", "rb");
	if (objFile == NULL) {
		printf("file open fail.\n");
		exit(1);
	}
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
	int g_polygon_MAX = 0;
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
	}
	for (int i = 0, j = 0; i < faceIndex; i++, j++) {
		amongus[i][0] = vertex[v[j].x].x, amongus[i][1] = vertex[v[j].x].y, amongus[i][2] = vertex[v[j].x].z;
		amongus[i][3] = normal[n[j].x].x, amongus[i][4] = normal[n[j].x].y, amongus[i][5] = normal[n[j].x].z;
		amongus[i][6] = texture[t[j].x].x, amongus[i][7] = texture[t[j].x].y;

		amongus[i][8] = vertex[v[j].y].x, amongus[i][9] = vertex[v[j].y].y, amongus[i][10] = vertex[v[j].y].z;
		amongus[i][11] = normal[n[j].y].x, amongus[i][12] = normal[n[j].y].y, amongus[i][13] = normal[n[j].y].z;
		amongus[i][14] = texture[t[j].y].x, amongus[i][15] = texture[t[j].y].y;

		amongus[i][16] = vertex[v[j].z].x, amongus[i][17] = vertex[v[j].z].y, amongus[i][18] = vertex[v[j].z].z;
		amongus[i][19] = normal[n[j].z].x, amongus[i][20] = normal[n[j].z].y, amongus[i][21] = normal[n[j].z].z;
		amongus[i][22] = texture[t[j].z].x, amongus[i][23] = texture[t[j].z].y;
	}
	free(v), free(t), free(n), free(vertex), free(normal);
	fclose(objFile);
}