#include "InitGM.h"
#include "ReadObj.h"

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
	extern Mtl* InfoMTL;
	extern GLuint texture[IMAGE_N];
	extern int image_Num, mtl_Num;
	int widthImage, heightImage, numberOfChannel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = NULL;
	glGenTextures(IMAGE_N, texture);
	if (image_Num + 1 > IMAGE_N) {
		printf("texture index leak.");
		exit(0);
	}
	for (int i = 0; i <= image_Num; i++) {
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		if(image_Num == i){
			data = stbi_load("Obj/among_us/source/white.jpg", &widthImage, &heightImage, &numberOfChannel, 0);
		}
		for (int j = 0; j < mtl_Num; j++) {
			if (InfoMTL[j].Index == i) {
				data = stbi_load(InfoMTL[j].map_Kd, &widthImage, &heightImage, &numberOfChannel, 0);
				//printf("%d�� InfoMTL:%s\n", i, InfoMTL[j].map_Kd);
			}
		}
		glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}
void InitBuffer()		// ���� �����ϰ� ������ �޾ƿ���
{
	glGenVertexArrays(16, vao);
	glGenBuffers(31, vbo);	// 2�� VBO ���� �� �Ҵ�
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
	glBindBuffer(GL_ARRAY_BUFFER, vbo[26]);	// ���ؽ� �Ӽ�(��ǥ��) ����
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(GLfloat), line, GL_STATIC_DRAW);	//���ؽ� ������ ���� ���ۿ� ����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// ��ǥ���� attribute �ε��� 0���� ����
	glEnableVertexAttribArray(0);	// attribute �ε��� 0���� ���밡���ϰ� ��

	glGenVertexArrays(VAO_N, objVao);
	glGenBuffers(VAO_N, objVbo);

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
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
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
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
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
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
		return false;
	}

	glUseProgram(ShaderProgramID);

	return ShaderProgramID;
}
