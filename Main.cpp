#define	STB_IMAGE_IMPLEMENTATION
#include "system.h"
GLint width = 1024, height = 512;
GLuint s_program;
GLuint s_program_ui;
extern int Scene;
extern enum Scene {
	eIntro = 0,
	eGame,
	eEnd
};
GLvoid Window_Initializer(int argc, char **argv)
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(width, height); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)
	Scene = eIntro;
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
}
void main(int argc, char** argv)
{
	Window_Initializer(argc, argv);
	// mtl 조성택꺼는 뒤어 _jo붙음
	//MakeFile("Obj/among_us/among_us.obj", "Obj/among_us/among_us.mtl");
	MakeFile("Obj/among_us/among_us.obj", "Obj/among_us/among_us_jo.mtl");
	s_program = make_shaderProgram("vertex.glsl", "fragment.glsl");
	s_program_ui=make_shaderProgram("vertex_OT.glsl", "fragment_OT.glsl");
	InitBuffer();
	InitTexture();
	InitTexture_ui();
	glEnable(GL_DEPTH_TEST);
	glutTimerFunc(30, TimerFunction, 1);
	// 출력 콜백 함수
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(pMotion);
	glutKeyboardFunc(Keyboard);	// 키보드 입력
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(special);
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMainLoop(); // 이벤트 처리 시작
}

