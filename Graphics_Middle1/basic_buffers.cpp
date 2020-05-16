//1004
#pragma comment (lib, "lib/freeglut.lib")
#pragma comment (lib, "lib/glew32.lib")
#include "GL/glew.h"	//���������� GL �Űܼ� ../GL/~
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "GL/cg-1.h"

#include <iostream>
#include "time.h"

using namespace std;

void startup();
void render();
void shutdown();

struct scene {
	int width = 800, height = 600;
	GLuint rendering_program;
	GLuint vertex_array_object;
};

scene sc;


//----------------------------------------------------------------------------
// MODELING

const int Num = 60;
float points[Num][4];
float colors[Num][4];

void triangle(float x1, float y1, float x2, float y2, float x3, float y3, int i, float* c) {
	   // ���� (x1,y1) (x2,y2) (x3,y3) ���� c
	// (x,y,z,w) w=1.0�̸� ~, 0.0�̸� ���ͷ� �ν�
	points[i][0] = x1;	points[i][1] = y1;	points[i][2] = 0.0;	points[i][3] = 1.0;
	points[i + 1][0] = x2;	points[i + 1][1] = y2;	points[i + 1][2] = 0.0;	points[i + 1][3] = 1.0;
	points[i + 2][0] = x3;	points[i + 2][1] = y3;	points[i + 2][2] = 0.0;	points[i + 2][3] = 1.0;

	cout << c[0] << c[1] << c[2] << c[3] << endl;

	for (int j = 0; j < 3; j++) {
		colors[i + j][0] = c[0];
		colors[i + j][1] = c[1];
		colors[i + j][2] = c[2];
		colors[i + j][3] = c[3];
	}
}

void rect(float x1, float y1, float x2, float y2, int i, float* c) {
	   // ���� �� (x1, y1) ������ �Ʒ� (x2,y2)  ���� c
	/*
	(x1,y1)	(x2,y1)
	(x1,y2)	(x2,y2)
	*/
	points[i][0] = x1;	points[i][1] = y1;	points[i][2] = 0.0;	points[i][3] = 1.0;		//i
	points[i + 1][0] = x1;	points[i + 1][1] = y2;	points[i + 1][2] = 0.0;	points[i + 1][3] = 1.0;		//i + 1
	points[i + 2][0] = x2;	points[i + 2][1] = y2;	points[i + 2][2] = 0.0;	points[i + 2][3] = 1.0;		//i + 2

	points[i + 3][0] = x1;	points[i + 3][1] = y1;	points[i + 3][2] = 0.0;	points[i + 3][3] = 1.0;		//i + 3
	points[i + 4][0] = x2;	points[i + 4][1] = y1;	points[i + 4][2] = 0.0;	points[i + 4][3] = 1.0;		//i + 4
	points[i + 5][0] = x2;	points[i + 5][1] = y2;	points[i + 5][2] = 0.0;	points[i + 5][3] = 1.0;		//i + 5

	cout << c[0] << c[1] << c[2] << c[3] << endl;

	for (int j = 0; j < 6; j++) {	// 'i+' �� �ϸ� �־��� index���Ͱ� �ƴ϶� 0���� ĥ������.
		colors[i + j][0] = c[0];
		colors[i + j][1] = c[1];
		colors[i + j][2] = c[2];
		colors[i + j][3] = c[3];	}
}
void make_house()
{
		  /*          ___
					 (   )
				   (_______)
				   |   __  |
				   |   [ ] |
				   |___[_]_|     */
		 
	int index = 0;
	float green[4] = { 0.0, 1.0, 0.0, 1.0 };  float purple[4] = { 1.0, 0.0, 1.0, 1.0 };  float yellow[4] = { 1.0, 1.0, 0.0, 1.0 };
	triangle(0.15, 0.2, 0.0, 0.0, 0.3, 0.0, index, green); index += 3;
	rect(0.0, 0.0, 0.3, -0.3, index, purple); index += 6;
	rect(0.1, -0.15, 0.2, -0.3, index, yellow);	index += 6;
	for (int i = 0; i < 15; i++)
		cout << "points: " << points[i][0] << ", " << points[i][1] << ", " << points[i][2] << ", " << points[i][3]
		<< "			colors: " << colors[i][0] << ", " << colors[i][1] << ", " << colors[i][2] << ", " << colors[i][3] << endl;

	//for (int i = 0; i < 15; i++)
	//{	// ������, �������� �ֱ�
	//	colors[i][0] = 1.0; colors[i][1] = 0.0; colors[i][2] = 0.0; colors[i][3] = 1.0;
	//}
}
				   
void startup()
{
	/*
	//--------- EXAMPLE1----------------------------------------------------
	//��ȫ�� �ﰢ��
	// feeding vertex shaders from buffers 
	
	sc.rendering_program = compile_shaders("v_buffers1.glsl", "f_buffers1.glsl");

	// Vertex Array Object 
#ifdef WINDOWS
	glGenVertexArrays(1, &sc.vertex_array_object);
	glBindVertexArray(sc.vertex_array_object);
#elif TARGET_OS_MAC
	glGenVertexArraysApple(1, &sc.vertex_array_object);
	glBindVertexArrayApple(sc.vertex_array_object);
#endif // WINDOWS

	float vertices[12] = { 0.25, -0.25, 0.5, 1.0,
						-0.25, -0.25, 0.5, 1.0,
						0.25, 0.25, 0.5, 1.0 };

	GLuint buffer;
	glGenBuffers(1, &buffer);

	// First, bind our buffer object to the GL_ARRAY_BUFFER binding
	// The subsequent call to glVertexAttribPointer will reference this buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Attribute 0, four components, floating-point data, non-normalized,
	// tightly packed, offset zero
	GLuint vPosition = glGetAttribLocation(sc.rendering_program, "vPosition");
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, ((GLvoid*)(0)));
	glEnableVertexAttribArray(vPosition);
*/	

	//------------------------------------------------------------------------
	// EXAMPLE2
	// Separate attributes 
	// EXAMPLE3�� ���� ���, ������ �ﰢ��, �����̵� ����


	// /*
	//shader compile
	sc.rendering_program = compile_shaders("v_buffers2.glsl", "f_buffers2.glsl");

#ifdef WINDOWS
	glGenVertexArrays(1, &sc.vertex_array_object);
	glBindVertexArray(sc.vertex_array_object);
#elif TARGET_OS_MAC
	glGenVertexArraysApple(1, &sc.vertex_array_object);
	glBindVertexArrayApple(sc.vertex_array_object);
#endif // WINDOWS
	//���� �� ��, ���� �� ��, ���⼭ ���� �ٲ㰡�鼭 �ϸ� �Ͽ콺 ����� ���ñ� �Ѵ�. �迭 �ٲ㰡�鼭
	GLuint buffer[2];

	//�������̶� �÷� ��ſ� �Ͽ콺�� ������� positions ��ſ� points, ���� 2�� colors ��ſ� �������� colors
	/* ������ �ﰢ���� ������, �Ͽ콺 �� �� �ڸ�Ʈ �ƿ�
	const GLfloat positions[] = {0.25, -0.25, 0.5, 1.0,
									-0.25, -0.25, 0.5, 1.0,
									0.25, 0.25, 0.5, 1.0};
	const GLfloat colors[] = {1.0, 0.0, 0.0, 1.0,
								  0.0, 1.0, 0.0, 1.0,
								  0.0, 0.0, 1.0, 1.0};
								  */
	
	// �� ����, make_house()�Լ� ����~rect(), triangle() �ٲٱ�, points�� colors�� �ƹ��͵� �� �־������ϱ�
	// render()�Լ��� vertex ������ �ٲٱ�
	make_house();
	
	glGenBuffers(2, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	//ù ��° ���ۿ� ������ �� �ְ�
	// positions->�ﰢ���� ��ġ ������
	//glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	// points->�Ͽ콺�� ��ġ ������
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	GLuint vPosition = glGetAttribLocation(sc.rendering_program, "vPosition");
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(vPosition);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	//�� ��° ���ۿ� �÷� �� ���-�̰� �ٲٱ�
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	GLuint vColor = glGetAttribLocation(sc.rendering_program, "vColor");
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE,0, NULL);
	glEnableVertexAttribArray(vColor);
	// */

	
//------------------------------------------------------------------------
// EXAMPLE3
// Multiple Buffer Object (interleaved attributes) 
	/*
	//���� �ϳ�, attribute �� ��, vertices�� �ش�

	sc.rendering_program = compile_shaders("v_buffers2.glsl", "f_buffers2.glsl");
#ifdef WINDOWS
	glGenVertexArrays(1, &sc.vertex_array_object);
	glBindVertexArray(sc.vertex_array_object);
#elif TARGET_OS_MAC
	glGenVertexArraysApple(1, &sc.vertex_array_object);
	glBindVertexArrayApple(sc.vertex_array_object);
#endif // WINDOWS

	struct vertex { float x, y, z, w, r, g, b, a; };
	GLuint buffer;
	static const vertex vertices[] = { 0.25, -0.25, 0.5,1.0, 1.0, 0.0, 0.0, 1.0, 
									-0.25, -0.25, 0.5, 1.0,  0.0, 1.0, 0.0,1.0, 
									0.25, 0.25, 0.5,1.0,  0.0, 0.0, 1.0, 1.0 };
	// allocate and initialize a buffer object
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// set up two vertex attributes - first positions
	GLuint vPosition = glGetAttribLocation(sc.rendering_program, "vPosition");
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, x));
	glEnableVertexAttribArray(vPosition);
	// now colors
	GLuint vColor = glGetAttribLocation(sc.rendering_program, "vColor");
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, r));
	glEnableVertexAttribArray(vColor);
	*/


	/*
//------------------------------------------------
// Uniforms
	//  Setting Uniform Arrays 
	GLint locTime, locIndex, locColor, locFlag;
	locTime = glGetUniformLocation(sc.rendering_program, "fTime");
	locIndex = glGetUniformLocation(sc.rendering_program, "iIndex");
	locColor = glGetUniformLocation(sc.rendering_program, "vColorValue");
	locFlag = glGetUniformLocation(sc.rendering_program, "bSomeFlag");

	glUseProgram(sc.rendering_program);
	glUniform1f(locTime, 45.2f);
	glUniform1i(locIndex, 42);
	glUniform4f(locColor, 1.0f, 0.0f, 0.0f, 1.0f);
	glUniform1i(locFlag, GL_FALSE);*/
}

void render()
{
	const GLfloat color[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, color);

	glUseProgram(sc.rendering_program);
	// triangle�� vertex 3��
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	// house�� vertex 15��->�ﰢ���� 5��
	glDrawArrays(GL_TRIANGLES, 0, 15);
	glFlush();
}

void shutdown()
{
	glDeleteVertexArrays(1, &sc.vertex_array_object);
	glDeleteProgram(sc.rendering_program);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Basic Buffer Examples");
	glewInit();
	version();

	startup();
	glutDisplayFunc(render);

	glutMainLoop();
	shutdown();
	return 0;
}