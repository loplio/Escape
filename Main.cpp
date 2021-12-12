#define	STB_IMAGE_IMPLEMENTATION
#include "system.h"
GLint width = 1000, height = 500;
GLuint s_program;
GLvoid Window_Initializer(int argc, char **argv)
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
}
void main(int argc, char** argv)
{
	Window_Initializer(argc, argv);
	// 세이더 읽어와서 세이더 프로그램 만들기
	MakeFile("Obj/among_us/among_us.obj", "Obj/among_us/among_us.mtl");
	s_program = make_shaderProgram("vertex.glsl", "fragment.glsl");

	InitBuffer();
	InitTexture();
	glEnable(GL_DEPTH_TEST);
	glutTimerFunc(30, TimerFunction, 1);
	// 출력 콜백 함수
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);	// 키보드 입력
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(special);
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMainLoop(); // 이벤트 처리 시작
}

