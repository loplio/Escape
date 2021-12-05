#include "InitGM.h"
#define F_ARRAY 20
#define S_ARRAY 240000

static GLuint shaderID, vertexShader, fragmentShader;
extern GLuint vao[16], vbo[31], objVao[200], objVbo[200];
extern GLfloat Box_pos[72][3], TriObj[F_ARRAY][S_ARRAY][24], floor_pos[12][3], line[6][3];
int Tri_Num;
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

void InitTexture()
{
	extern GLuint texture;
	int widthImage, heightImage, numberOfChannel;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("Obj/Isometric/texture/7.jpg", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
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

	//glBindVertexArray(vao[6]);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(floor_pos), floor_pos, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	glBindVertexArray(vao[13]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[26]);	// 버텍스 속성(좌표값) 저장
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(GLfloat), line, GL_STATIC_DRAW);	//버텍스 데이터 값을 버퍼에 저장
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// 좌표값을 attribute 인덱스 0번에 명시
	glEnableVertexAttribArray(0);	// attribute 인덱스 0번을 사용가능하게 함

	glGenVertexArrays(200, objVao);
	glGenBuffers(200, objVbo);

	for (int i = 0; i < Tri_Num; i++) {
		glBindVertexArray(objVao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, objVbo[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TriObj[i]), TriObj[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
}
void make_vertexShaders(const char* v_glsl)
{
	GLchar* vertexSource;
	vertexSource = filetobuf(v_glsl);
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
void make_fragmentShaders(const char* f_glsl)
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf(f_glsl);
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
GLuint make_shaderProgram(const char* v_glsl, const char* f_glsl)
{
	GLuint ShaderProgramID = glCreateProgram();

	make_vertexShaders(v_glsl);
	make_fragmentShaders(f_glsl);

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