#include "Rendering.h"
#include "ReadObj.h"
#define SPEED 10
#define SPACEBAR 32
glm::vec3 cameraPos(1600.0f, 100.0f, -1700.0f);
glm::vec3 cameraDirection(1600.0f, 100.0f, -1701.0f);
//glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
//glm::vec3 cameraDirection(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
glm::mat4 view(1.0f);
extern GLuint s_program;
extern int width, height, Tri_Num;
GLuint texture[IMAGE_N];
GLuint vao[16], vbo[31], objVao[VAO_N], objVbo[VAO_N];
GLfloat Box_pos[72][3] = {
	1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0,
	-1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0,
	1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0,
	-1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0,
	1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0,
	1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0,
	0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0,
	0.0,0.0,-1.0, 0.0,0.0,-1.0, 0.0,0.0,-1.0, 0.0,0.0,-1.0, 0.0,0.0,-1.0, 0.0,0.0,-1.0,
	1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0,
	-1.0,0.0,0.0, -1.0,0.0,0.0, -1.0,0.0,0.0, -1.0,0.0,0.0, -1.0,0.0,0.0, -1.0,0.0,0.0,
	0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0,
	0.0,-1.0,0.0, 0.0,-1.0,0.0, 0.0,-1.0,0.0, 0.0,-1.0,0.0, 0.0,-1.0,0.0, 0.0,-1.0,0.0
};
GLfloat _2dwindow[48] = {
 -1.0f, -1.0f, 0.0f, 0.0, 0.0, 1.0, 0.0, 0.0,
1.0f, -1.0f, 0.0f, 0.0, 0.0, 1.0, 1.0, 0.0,
1.0f, 1.0f, 0.0f, 0.0, 0.0, 1.0, 1.0, 1.0,
1.0f, 1.0f, 0.0f, 0.0, 0.0, 1.0, 1.0, 1.0,
-1.0f, 1.0f, 0.0f, 0.0, 0.0, 1.0, 0.0, 1.0,
-1.0f, -1.0f, 0.0f, 0.0, 0.0, 1.0, 0.0, 0.0
};
GLfloat TriObj[F_ARRAY][S_ARRAY][24];
GLfloat floor_pos[12][3] = { 150.0, 0.0, 150.0, 0.0,1.0,0.0, -150.0, 0.0, 150.0, 0.0,1.0,0.0, -150.0, 0.0, -150.0, 0.0,1.0,0.0, 150.0, 0.0, -150.0, 0.0,1.0,0.0 };
GLfloat line[6][3] = { 0.0,-400.0,0.0, 0.0,400.0,0.0, -400.0,0.0,0.0, 400.0,0.0,0.0, 0.0,0.0,-400.0, 0.0,0.0,400.0 };
GLfloat shape_focus[16][3] = { 0.0,0.0,0.0, 12.0,5.0,0.0, 0.0,0.1,0.0, 0.0,0.35,0.0, -0.15,0.75,0.0, 0.15,0.75,0.0, };
object* sphere;
Mtl* InfoMTL;
BOOL state_timer = TRUE, rotate_object, rotate_light, state_walk; //RT - Rect, Tri  SL - Solid, Line
char dir_z, dir_y, dir_x, light_dir_y = 1;	//x,y,z 방향
float axis_x, axis_y, light_axis_y, cameraRt_axis_y, cameraRt_axis_x;
float Mouse_x, Mouse_y, buffer_rad1, buffer_rad2, rad_p1, rad_p2, DirCameraX, DirCameraZ;
int special_key, KeyDownRL, KeyDownFB, keyRad;
int image_Num, mtl_Num;

int Scene;
enum keyDown {
	F = 0, RF = 45,
	R = 90, RB = 135,
	B = 180, LB = 225,
	L = 270, LF = 315
};
enum scene {
	eIntro = 0,
	eGame,
	eEnd
};
GLvoid TransformFun()
{
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(s_program, "viewTransform");
	unsigned int projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	int viewPosLocation = glGetUniformLocation(s_program, "viewPos");
	glm::mat4 projection(1.0f);
	glm::mat4 mvPos(1.0f);
	glm::vec3 vPos(0.0f, 0.0f, 20.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 500000.0f);
	projection = glm::rotate(projection, glm::radians(cameraRt_axis_y), glm::vec3(1.0f, 0.0f, 0.0f));
	projection = glm::rotate(projection, glm::radians(cameraRt_axis_x), glm::vec3(0.0f, 1.0f, 0.0f));
	projection = glm::translate(projection, glm::vec3(0.0f, -30.0f, -20.0f));
	//projection = glm::rotate(projection, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//projection = glm::rotate(projection, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//mvPos = glm::rotate(mvPos, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//mvPos = glm::rotate(mvPos, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvPos = glm::translate(mvPos, vPos);
	mvPos = glm::rotate(mvPos, glm::radians(-cameraRt_axis_x), glm::vec3(1.0f, 0.0f, 0.0f));
	mvPos = glm::rotate(mvPos, glm::radians(-cameraRt_axis_y), glm::vec3(0.0f, 1.0f, 0.0f));
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
	glUniform3f(objColorLocation, 0.7, 0.7, 0.7);
	glm::mat4 Rx(1.0f); glm::mat4 Ry(1.0f); glm::mat4 Ry_l(1.0f);
	glm::mat4 T(1.0f); glm::mat4 S(1.0f); glm::mat4 R(1.0f); glm::mat4 L(1.0f); glm::mat4 Init(1.0f);
	Rx = glm::rotate(Rx, glm::radians(axis_x), glm::vec3(1.0f, 0.0f, 0.0f)); Ry = glm::rotate(Ry, glm::radians(axis_y), glm::vec3(0.0f, 1.0f, 0.0f)); Ry_l = glm::rotate(Ry_l, glm::radians(light_axis_y), glm::vec3(0.0f, 1.0f, 0.0f));
	T = glm::translate(T, glm::vec3(shape_focus[1][0], shape_focus[1][1], shape_focus[1][2])); S = glm::scale(S, glm::vec3(1.0f, 1.0f, 1.0f)); Init = glm::translate(Init, glm::vec3(0.0f, 0.0f, 0.0f));
	TransformFun();
	if (Scene == eIntro) {
		L = glm::scale(L, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(L));
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}
	else if (Scene == eGame) {
		//배경
		L = glm::scale(L, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(L));
		glBindVertexArray(vao[13]);
		glDrawArrays(GL_LINES, 0, 2);
		glDrawArrays(GL_LINES, 2, 2);
		glDrawArrays(GL_LINES, 4, 2);
		glBindVertexArray(vao[6]);

		////객체
		////R = glm::scale(R, glm::vec3(0.05f, 0.05f, 0.05f));
		//R = glm::scale(S, glm::vec3(0.1f, 0.1f, 0.1f));
		R = glm::scale(R, glm::vec3(40.0f, 40.0f, 40.0f));
		R = glm::translate(R, glm::vec3(40.0f, 0.0, -40.0));
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(R));
		glActiveTexture(GL_TEXTURE0);
		for (int n = 0; n < Tri_Num; n++) {
			glBindVertexArray(objVao[n]);
			glBindTexture(GL_TEXTURE_2D, texture[sphere[n].info.Index]);
			for (int i = 0; i < sphere[n].face_num; i++) {				//기본 박스
				glDrawArrays(GL_TRIANGLES, i * 3, 3);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
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
	}

	else if (Scene == eEnd) {

	}
	

	glutSwapBuffers(); // 화면에 출력하기
}
void Update()
{
	if (rotate_object) cameraRt_axis_y += 5;
	if (rotate_light) light_axis_y += 3 * light_dir_y;
	if (state_walk) cameraPos.x += DirCameraX, cameraPos.z += DirCameraZ, cameraDirection.x += DirCameraX, cameraDirection.z += DirCameraZ;
	printf("cameraPos: %f  %f  %f\n", cameraPos.x, cameraPos.y, cameraPos.z);
}
void TimerFunction(int value)
{
	glutPostRedisplay();
	Update();
	glutTimerFunc(30, TimerFunction, 1);
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
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
	case 'w':
		if (KeyDownFB == -1) { KeyDownFB = 0; break; }
		else if (KeyDownFB == 0) {
			KeyDownFB = 1;
			if (KeyDownRL != 0) {
				if (KeyDownRL == 1) keyRad = RF;
				else keyRad = LF;
			}
			else keyRad = F;
		}
		state_walk = TRUE;
		DirCameraX = SPEED * sin(glm::radians(cameraRt_axis_x + keyRad)), DirCameraZ = -SPEED * cos(glm::radians(cameraRt_axis_x + keyRad));
		break;
	case 's':
		if (KeyDownFB == 1) { KeyDownFB = 0; break; }
		else if (KeyDownFB == 0) {
			KeyDownFB = -1;
			if (KeyDownRL != 0) {
				if (KeyDownRL == 1) keyRad = RB;
				else keyRad = LB;
			}
			else keyRad = B;
		}
		state_walk = TRUE;
		DirCameraX = SPEED * sin(glm::radians(cameraRt_axis_x + keyRad)), DirCameraZ = -SPEED * cos(glm::radians(cameraRt_axis_x + keyRad));
		break;
	case 'a':
		if (KeyDownRL == 1) { KeyDownRL = 0; break; }
		else if (KeyDownRL == 0) {
			KeyDownRL = -1;
			if (KeyDownFB != 0) {
				if (KeyDownFB == 1) keyRad = LF;
				else keyRad = LB;
			}
			else keyRad = L;
		}
		state_walk = TRUE;
		DirCameraX = SPEED * sin(glm::radians(cameraRt_axis_x + keyRad)), DirCameraZ = -SPEED * cos(glm::radians(cameraRt_axis_x + keyRad));
		break;
	case 'd':
		if (KeyDownRL == -1) { KeyDownRL = 0; break; }
		else if (KeyDownRL == 0) {
			KeyDownRL = 1;
			if (KeyDownFB != 0) {
				if (KeyDownFB == 1) keyRad = RF;
				else keyRad = RB;
			}
			else
				keyRad = R;
		}
		state_walk = TRUE;
		DirCameraX = SPEED * sin(glm::radians(cameraRt_axis_x + keyRad)), DirCameraZ = -SPEED * cos(glm::radians(cameraRt_axis_x + keyRad));
		break;
	case 'q':
		glutLeaveMainLoop();
	}
	glutPostRedisplay();
}
GLvoid KeyboardUp(unsigned char key, int x, int y)
{
	switch (key) {
	case SPACEBAR:
		cameraPos.y += 10, cameraDirection.y += 10;
		break;
	case 'w':
		if (KeyDownFB == 0) {
			KeyDownFB = -1;
			if (KeyDownRL == 0)keyRad = B;
			else if (KeyDownRL == 1)keyRad = RB;
			else keyRad = LB;
		}
		else {
			KeyDownFB = 0;
			if (KeyDownRL == 0) state_walk = FALSE;
			else if (KeyDownRL == 1) keyRad = R;
			else keyRad = L;
		}
		DirCameraX = SPEED * sin(glm::radians(cameraRt_axis_x + keyRad)), DirCameraZ = -SPEED * cos(glm::radians(cameraRt_axis_x + keyRad));
		break;
	case 'a':
		if (KeyDownRL == 0) {
			KeyDownRL = 1;
			if (KeyDownFB == 0) keyRad = R;
			else if (KeyDownFB == 1) keyRad = RF;
			else keyRad = RB;
		}
		else {
			KeyDownRL = 0;
			if (KeyDownFB == 0) state_walk = FALSE;
			else if (KeyDownFB == 1) keyRad = F;
			else keyRad = B;
		}
		DirCameraX = SPEED * sin(glm::radians(cameraRt_axis_x + keyRad)), DirCameraZ = -SPEED * cos(glm::radians(cameraRt_axis_x + keyRad));
		break;
	case 's':
		if (KeyDownFB == 0) {
			KeyDownFB = 1;
			if (KeyDownRL == 0) keyRad = F;
			else if (KeyDownRL == 1) keyRad = RF;
			else keyRad = LF;
		}
		else {
			KeyDownFB = 0;
			if (KeyDownRL == 0) state_walk = FALSE;
			else if (KeyDownRL == 1) keyRad = R;
			else keyRad = L;
		}
		DirCameraX = SPEED * sin(glm::radians(cameraRt_axis_x + keyRad)), DirCameraZ = -SPEED * cos(glm::radians(cameraRt_axis_x + keyRad));
		break;
	case 'd':
		if (KeyDownRL == 0) {
			KeyDownRL = -1;
			if (KeyDownFB == 0) keyRad = L;
			else if (KeyDownFB == 1) keyRad = LF;
			else keyRad = LB;
		}
		else {
			KeyDownRL = 0;
			if (KeyDownFB == 0) state_walk = FALSE;
			else if (KeyDownFB == 1) keyRad = F;
			else keyRad = B;
		}
		DirCameraX = SPEED * sin(glm::radians(cameraRt_axis_x + keyRad)), DirCameraZ = -SPEED * cos(glm::radians(cameraRt_axis_x + keyRad));
		break;
	}
	glutPostRedisplay();
}
void special(int key, int x, int y)
{
	special_key = key;
	switch (key) {
	case GLUT_KEY_CTRL_L:
		cameraPos.y -= 10, cameraDirection.y -= 10;
		break;
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
void MakeFile(const char* objfile, const char* mtlfile)
{
	FILE* Shape, *MTL;
	Shape = fopen(objfile, "rb");
	MTL = fopen(mtlfile, "rb");
	if (Shape == NULL || MTL == NULL) {
		printf("File open failed...");
		exit(1);
	}
	sphere = ReadObj(Shape, sphere, &Tri_Num);
	InfoMTL = ReadMtl(MTL, sphere, Tri_Num, &image_Num, &mtl_Num);
	fclose(Shape);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Mouse_x = (2.0 * x / width - 1.0) * 10, Mouse_y = -(2.0 * y / height - 1.0) * 10;
	}
	else if (state == GLUT_UP) {
		buffer_rad1 += rad_p1;
		buffer_rad2 += rad_p2;
	}
}
void Motion(int x, int y)
{
	float now_x = (2.0 * x / width - 1.0) * 10, now_y = -(2.0 * y / height - 1.0) * 10;
	rad_p1 = (now_x - Mouse_x) * 9;
	rad_p2 = -(now_y - Mouse_y) * 9;
	cameraRt_axis_x = rad_p1 + buffer_rad1;
	cameraRt_axis_y = rad_p2 + buffer_rad2;
	//printf("cameraRt_axis_x:%f\n", cameraRt_axis_x);
}