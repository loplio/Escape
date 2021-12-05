#include "Main.h"

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();
GLuint make_shaderProgram();
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void TimerFunction(int value);
void Update();
typedef struct ObjFile {
	int Polygon_Num;
	int plane_per_vertex;
} object;
GLint width = 1000, height = 500;
GLuint shaderID, vertexShader, fragmentShader, s_program, vao[16], vbo[31], objVao[2];
glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
glm::mat4 view(1.0f);
GLfloat Box_pos[72][3] = {
//front
	1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0,
//back
	-1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0,
//right
	1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0,
//left
	-1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0,
//top
	1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0,
//bottom
	1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0,
//vector
	0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0,
	0.0,0.0,-1.0, 0.0,0.0,-1.0, 0.0,0.0,-1.0, 0.0,0.0,-1.0, 0.0,0.0,-1.0, 0.0,0.0,-1.0,
	1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0,
	-1.0,0.0,0.0, -1.0,0.0,0.0, -1.0,0.0,0.0, -1.0,0.0,0.0, -1.0,0.0,0.0, -1.0,0.0,0.0,
	0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0,
	0.0,-1.0,0.0, 0.0,-1.0,0.0, 0.0,-1.0,0.0, 0.0,-1.0,0.0, 0.0,-1.0,0.0, 0.0,-1.0,0.0
};
GLfloat QurdObj[30000][36] = { 0, };
GLfloat TriObj[10000][18] = { 0, };
GLfloat floor_pos[12][3] = { 16.0, 0.0, 13.0, 0.0,1.0,0.0, -16.0, 0.0, 13.0, 0.0,1.0,0.0, -16.0, 0.0, -13.0, 0.0,1.0,0.0, -16.0, 0.0, -13.0, 0.0,1.0,0.0, 16.0, 0.0, -13.0, 0.0,1.0,0.0, 16.0, 0.0, 13.0, 0.0,1.0,0.0 };
GLfloat line[6][3] = { 0.0,-15.0,0.0, 0.0,15.0,0.0, -15.0,0.0,0.0, 15.0,0.0,0.0, 0.0,0.0,-15.0, 0.0,0.0,15.0 };
GLfloat shape_focus[16][3] = { 0.0,0.0,0.0, 12.0,5.0,0.0, 0.0,0.1,0.0, 0.0,0.35,0.0, -0.15,0.75,0.0, 0.15,0.75,0.0, };
BOOL wireSL = FALSE, state_timer = TRUE, rotate_object = FALSE, rotate_light = FALSE; //RT - Rect, Tri  SL - Solid, Line
object sphere = {};
char dir_z = 0, dir_y = 0, dir_x = 0, cameraRv_dir_y = 0, light_dir_y = 1;	//x,y,z 방향
float axis_x = 0.0, axis_y = 0.0, light_axis_y = 0.0, cameraRv_axis_y = 0.0, cameraRt_axis_y = 0.0;
int special_key = 0, Polygon_Number = 0;
object ReadObj(FILE* objFile, object obj)
{
	char count[100], bind[100];
	int vertexNum = 0;
	int faceNum = 0;
	int plane_per_vertex_n = 0;
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		//printf("%s\n", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			faceNum += 1;
		memset(count, '\0', sizeof(count));
	}
	vertexNum += 1;
	if (vertexNum < faceNum)
		plane_per_vertex_n = 3;
	else
		plane_per_vertex_n = 4;
	printf("v:%d f:%d\n", vertexNum, faceNum);
	glm::vec3* vertex;
	glm::vec3* normal;
	glm::uvec4* v, * t, * n;
	int vertIndex = 1;
	int normIndex = 1;
	int faceIndex = 0;
	v = (glm::uvec4*)malloc(sizeof(glm::vec4) * faceNum);
	t = (glm::uvec4*)malloc(sizeof(glm::vec4) * faceNum);
	n = (glm::uvec4*)malloc(sizeof(glm::vec4) * faceNum);
	vertex = (glm::vec3*)malloc(sizeof(glm::vec3) * vertexNum);
	normal = (glm::vec3*)malloc(sizeof(glm::vec3) * vertexNum);
	rewind(objFile);
	while (!feof(objFile)) {
		fscanf(objFile, "%s", bind);
		if (bind[0] == 'v' && bind[1] == '\0') {
			fscanf(objFile, "%f %f %f",
				&vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
			//printf("v: %f %f %f\n", vertex[vertIndex].x, vertex[vertIndex].y, vertex[vertIndex].z);
			vertIndex++;
		}
		else if (bind[0] == 'v' && bind[1] == 'n') {
			fscanf(objFile, "%f %f %f",
				&normal[normIndex].x, &normal[normIndex].y, &normal[normIndex].z);
			//printf("vn: %f %f %f\n", normal[normIndex].x, normal[normIndex].y, normal[normIndex].z);
			normIndex++;
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				&v[faceIndex].x, &t[faceIndex].x, &n[faceIndex].x, &v[faceIndex].y, &t[faceIndex].y, &n[faceIndex].y, &v[faceIndex].z, &t[faceIndex].z, &n[faceIndex].z, &v[faceIndex].w, &t[faceIndex].w, &n[faceIndex].w);
			faceIndex++;
		}
	}
	if (plane_per_vertex_n == 3) {
		printf("Make a TriObj\n");
		for (int i = 0; i < faceIndex; i++) {
			TriObj[i][0] = vertex[v[i].x].x;
			TriObj[i][1] = vertex[v[i].x].y;
			TriObj[i][2] = vertex[v[i].x].z;
			TriObj[i][3] = normal[n[i].x].x;
			TriObj[i][4] = normal[n[i].x].y;
			TriObj[i][5] = normal[n[i].x].z;

			TriObj[i][6] = vertex[v[i].y].x;
			TriObj[i][7] = vertex[v[i].y].y;
			TriObj[i][8] = vertex[v[i].y].z;
			TriObj[i][9] = normal[n[i].y].x;
			TriObj[i][10] = normal[n[i].y].y;
			TriObj[i][11] = normal[n[i].y].z;

			TriObj[i][12] = vertex[v[i].z].x;
			TriObj[i][13] = vertex[v[i].z].y;
			TriObj[i][14] = vertex[v[i].z].z;
			TriObj[i][15] = normal[n[i].z].x;
			TriObj[i][16] = normal[n[i].z].y;
			TriObj[i][17] = normal[n[i].z].z;
		}
	}
	else {
		printf("Make a QurdObj\n");
		for (int i = 0; i < faceIndex; i++) {
			QurdObj[i][0] = vertex[v[i].x].x;
			QurdObj[i][1] = vertex[v[i].x].y;
			QurdObj[i][2] = vertex[v[i].x].z;
			QurdObj[i][3] = normal[n[i].x].x;
			QurdObj[i][4] = normal[n[i].x].y;
			QurdObj[i][5] = normal[n[i].x].z;

			QurdObj[i][6] = vertex[v[i].y].x;
			QurdObj[i][7] = vertex[v[i].y].y;
			QurdObj[i][8] = vertex[v[i].y].z;
			QurdObj[i][9] = normal[n[i].y].x;
			QurdObj[i][10] = normal[n[i].y].y;
			QurdObj[i][11] = normal[n[i].y].z;

			QurdObj[i][12] = vertex[v[i].z].x;
			QurdObj[i][13] = vertex[v[i].z].y;
			QurdObj[i][14] = vertex[v[i].z].z;
			QurdObj[i][15] = normal[n[i].z].x;
			QurdObj[i][16] = normal[n[i].z].y;
			QurdObj[i][17] = normal[n[i].z].z;

			QurdObj[i][18] = vertex[v[i].z].x;
			QurdObj[i][19] = vertex[v[i].z].y;
			QurdObj[i][20] = vertex[v[i].z].z;
			QurdObj[i][21] = normal[n[i].z].x;
			QurdObj[i][22] = normal[n[i].z].y;
			QurdObj[i][23] = normal[n[i].z].z;

			QurdObj[i][24] = vertex[v[i].w].x;
			QurdObj[i][25] = vertex[v[i].w].y;
			QurdObj[i][26] = vertex[v[i].w].z;
			QurdObj[i][27] = normal[n[i].w].x;
			QurdObj[i][28] = normal[n[i].w].y;
			QurdObj[i][29] = normal[n[i].w].z;

			QurdObj[i][30] = vertex[v[i].x].x;
			QurdObj[i][31] = vertex[v[i].x].y;
			QurdObj[i][32] = vertex[v[i].x].z;
			QurdObj[i][33] = normal[n[i].x].x;
			QurdObj[i][34] = normal[n[i].x].y;
			QurdObj[i][35] = normal[n[i].x].z;
		}
	}
	obj.Polygon_Num = faceIndex;
	obj.plane_per_vertex = plane_per_vertex_n;
	return obj;
}
char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}
void main(int argc, char** argv)//--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(width, height); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	srand((unsigned)time(NULL));
	// 세이더 읽어와서 세이더 프로그램 만들기
	FILE* Shape;
	Shape = fopen("Bed.obj", "rb");
	if (Shape == NULL) {
		printf("File open failed...");
		exit(1);
	}
	sphere = ReadObj(Shape, sphere);
	fclose(Shape);

	InitShader();
	InitBuffer();
	glEnable(GL_DEPTH_TEST);
	glutTimerFunc(30, TimerFunction, 1);
	// 출력 콜백 함수
	glutKeyboardFunc(Keyboard);	// 키보드 입력
	glutSpecialFunc(special);
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMainLoop(); // 이벤트 처리 시작
}
GLvoid TransformFun()
{
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(s_program, "viewTransform");
	unsigned int projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	int viewPosLocation = glGetUniformLocation(s_program, "viewPos");
	glm::mat4 projection(1.0f);
	glm::mat4 mvPos(1.0f);
	glm::vec3 vPos(0.0f, 0.0f, 20.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 50.0f);
	//projection = glm::rotate(projection, glm::radians(cameraRt_axis_y), glm::vec3(0.0f, 1.0f, 0.0f));
	projection = glm::translate(projection, glm::vec3(0.0f, 0.0f, -20.0f));
	projection = glm::rotate(projection, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	projection = glm::rotate(projection, glm::radians(cameraRv_axis_y), glm::vec3(0.0f, 1.0f, 0.0f));
	//projection = glm::rotate(projection, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//mvPos = glm::rotate(mvPos, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mvPos = glm::rotate(mvPos, glm::radians(-cameraRv_axis_y), glm::vec3(0.0f, 1.0f, 0.0f));
	mvPos = glm::rotate(mvPos, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvPos = glm::translate(mvPos, vPos);
	//mvPos = glm::rotate(mvPos, glm::radians(-cameraRt_axis_y), glm::vec3(0.0f, 1.0f, 0.0f));
	vPos = glm::vec3(mvPos * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	glUniform3f(viewPosLocation, vPos.x, vPos.y, vPos.z);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}
GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	GLfloat rColor = 0.6, gColor = 0.8, bColor = 0.8;
	glClearColor(rColor, gColor, bColor, 1.0f); // 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);
	unsigned int transformLocation = glGetUniformLocation(s_program, "transform");
	unsigned int LightTransformLocation = glGetUniformLocation(s_program, "LightTransform");
	int lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	int lightColorLocation = glGetUniformLocation(s_program, "lightColor");
	int objColorLocation = glGetUniformLocation(s_program, "objectColor");
	glUniform3f(lightPosLocation, 12.0, 5.0, 0.0);
	glUniform3f(objColorLocation, 1.0, 0.5, 0.3);
	glm::mat4 Rx(1.0f); glm::mat4 Ry(1.0f); glm::mat4 Ry_l(1.0f);
	glm::mat4 T(1.0f); glm::mat4 S(1.0f); glm::mat4 R(1.0f); glm::mat4 L(1.0f); glm::mat4 Init(1.0f);
	Rx = glm::rotate(Rx, glm::radians(axis_x), glm::vec3(1.0f, 0.0f, 0.0f)); Ry = glm::rotate(Ry, glm::radians(axis_y), glm::vec3(0.0f, 1.0f, 0.0f)); Ry_l = glm::rotate(Ry_l, glm::radians(light_axis_y), glm::vec3(0.0f, 1.0f, 0.0f));
	T = glm::translate(T, glm::vec3(shape_focus[1][0], shape_focus[1][1], shape_focus[1][2])); S = glm::scale(S, glm::vec3(1.0f, 1.0f, 1.0f)); Init = glm::translate(Init, glm::vec3(0.0f, 0.0f, 0.0f));
	TransformFun();
	//배경
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(L));
	glBindVertexArray(vao[13]);
	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 2, 2);
	glDrawArrays(GL_LINES, 4, 2);
	//glBindVertexArray(vao[6]);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawArrays(GL_TRIANGLES, 3, 3);

	//객체
	R = glm::scale(S, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(R));
	glBindVertexArray(vao[sphere.plane_per_vertex + 1]);
	for (int i = 0; i < sphere.Polygon_Num * (sphere.plane_per_vertex - 2); i++) {				//기본 박스
		glDrawArrays(GL_TRIANGLES, i * 3, 3);
	}

	//조명
	R = Ry_l;
	glUniformMatrix4fv(LightTransformLocation, 1, GL_FALSE, glm::value_ptr(R));
	R = Ry_l * T;
	glUniform3f(lightPosLocation, -5.0, 5.0, 0.0);
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	glUniform3f(objColorLocation, 0.0, 0.5, 1.0);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(R));
	glBindVertexArray(vao[0]);
	for (int i = 0; i < 12; i++) {
		glDrawArrays(GL_TRIANGLES, i * 3, 3);
	}

	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 't':
		if (!rotate_light) rotate_light = TRUE;
		else if (rotate_light && light_dir_y == 1) rotate_light = FALSE;
		light_dir_y = 1;
		break;
	case 'T':
		if (!rotate_light) rotate_light = TRUE;
		else if (rotate_light && light_dir_y == -1)rotate_light = FALSE;
		light_dir_y = -1;
		break;
	case 'z':					// 카메라 z축 이동
		cameraPos.z += 0.2;
		cameraDirection.z += 0.2;
		break;
	case 'Z':
		cameraPos.z -= 0.2;
		cameraDirection.z -= 0.2;
		break;
	case 'x':					// 카메라 x축 이동
		cameraPos.x += 0.2;
		cameraDirection.x += 0.2;
		break;
	case 'X':
		cameraPos.x -= 0.2;
		cameraDirection.x -= 0.2;
		break;
	case 'r':					// 카메라 y축 기준 회전
		if (cameraRv_dir_y != 1) cameraRv_dir_y = 1;
		else cameraRv_dir_y = 0;
		break;
	case 'R':
		if (cameraRv_dir_y != -1) cameraRv_dir_y = -1;
		else cameraRv_dir_y = 0;
		break;
	case 'w':
		wireSL = TRUE;
		break;
	case 'W':
		wireSL = FALSE;
		break;
	case 'h':
		if (glIsEnabled(GL_DEPTH_TEST))
			glDisable(GL_DEPTH_TEST);
		else
			glEnable(GL_DEPTH_TEST);
		break;
	case 'q':
		glutLeaveMainLoop();
	}
	glutPostRedisplay();
}
void special(int key, int x, int y)
{
	special_key = key;
	switch (key) {
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	}
	glutPostRedisplay();
}
void Update()
{
	if (rotate_object) cameraRt_axis_y += 5;
	if (rotate_light) light_axis_y += 3 * light_dir_y;
	if (cameraRv_dir_y != 0) {
		cameraRv_axis_y += 3 * cameraRv_dir_y;
	}
}
void TimerFunction(int value)
{
	glutPostRedisplay();
	Update();
	glutTimerFunc(30, TimerFunction, 1);
}
void make_vertexShaders()
{
	GLchar* vertexSource;
	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
void make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
GLuint make_shaderProgram()
{
	GLuint ShaderProgramID = glCreateProgram();

	glAttachShader(ShaderProgramID, vertexShader);
	glAttachShader(ShaderProgramID, fragmentShader);
	glLinkProgram(ShaderProgramID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return false;
	}

	glUseProgram(ShaderProgramID);

	return ShaderProgramID;
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}
void InitBuffer()		// 버퍼 생성하고 데이터 받아오기
{
	glGenVertexArrays(16, vao);
	glGenBuffers(31, vbo);	// 2개 VBO 지정 후 할당
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Box_pos), Box_pos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(Box_pos) / 2));
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriObj), TriObj, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[5]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QurdObj), QurdObj, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[6]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_pos), floor_pos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[13]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[26]);	// 버텍스 속성(좌표값) 저장
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(GLfloat), line, GL_STATIC_DRAW);	//버텍스 데이터 값을 버퍼에 저장
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// 좌표값을 attribute 인덱스 0번에 명시
	glEnableVertexAttribArray(0);	// attribute 인덱스 0번을 사용가능하게 함
}
void InitShader()		// 세이더 프로그램 생성하기
{
	make_vertexShaders();
	make_fragmentShaders();

	s_program = glCreateProgram();

	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(s_program);
}