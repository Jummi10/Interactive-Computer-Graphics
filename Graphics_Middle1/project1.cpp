//1011
#pragma comment (lib, "lib/freeglut.lib")
#pragma comment (lib, "lib/glew32.lib")
#include "GL/glew.h"	//���������� GL �Űܼ� ../GL/~
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "GL/cg-2.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include "time.h"

using namespace std;
using namespace glm;
void version();
GLuint compile_shaders(const char* vsource, const char* fsource);

void startup();
void render();
void shutdown();

struct scene {
	int width = 800, height = 600;
	GLuint rendering_program;
	GLuint vertex_array_object;

	vec4 vertices[100];
	vec4 colors[100];
	unsigned int indices[100];
	int num, numIndex;

	mat4 model;
	GLint mv_location;

	GLfloat rotate_angle = 0.001;	//�ǽ� 1, rotate house

	float theta = 0.0;	//�ǽ� 2, translate and rotate house by (0,O)

	/*
	//�ǽ� 3
	//mat4 model;
	//GLint mv_location;
	GLint color;

	float dispw, dispb;
	*/
};

scene sc;

void make_model()
{
	sc.num = 0;
	/*  //House (drawArray) 
	sc.vertices[sc.num] = vec4(0.5, 0.2, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(0.0, 0.5, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(-0.5, 0.2, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	
	sc.vertices[sc.num] = vec4(0.5, 0.2, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(-0.5, 0.2, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(-0.5, -0.5, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 0.0, 0.0, 1.0);  sc.num++;

	sc.vertices[sc.num] = vec4(0.5, 0.2, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(-0.5, -0.5, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 0.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(0.5, -0.5, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 0.0, 0.0, 1.0);  sc.num++;
	*/



	///*
	//House (drawElements)
	sc.vertices[sc.num] = vec4(0.5, 0.2, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(0.0, 0.5, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(-0.5, 0.2, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(-0.5, -0.5, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 0.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(0.5, -0.5, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 0.0, 0.0, 1.0);  sc.num++;
	sc.indices[0] = 0; sc.indices[1] = 1; sc.indices[2] = 2;
	sc.indices[3] = 0; sc.indices[4] = 2; sc.indices[5] = 3;
	sc.indices[6] = 0; sc.indices[7] = 3; sc.indices[8] = 4;
	sc.numIndex = 9;
	//*/


	/*
	//�ǽ� 3
	sc.vertices[sc.num] = vec4(0.5, 0.5, 0.0, 1.0);	 sc.num++;	//0
	sc.vertices[sc.num] = vec4(-0.5, 0.5, 0.0, 1.0);	 sc.num++;	//1
	sc.vertices[sc.num] = vec4(-0.5, -0.5, 0.0, 1.0);	 sc.num++;	//2
	sc.vertices[sc.num] = vec4(0.5, -0.5, 0.0, 1.0);	 sc.num++;	//3

	//(0,1,3), (3,1,2)
	sc.indices[0] = 0;	sc.indices[1] = 1;	sc.indices[2] = 3;
	sc.indices[3] = 3;	sc.indices[4] = 1;	sc.indices[5] = 2;
	sc.numIndex = 6;
	*/
}

// �� ó�� �ѹ� ����
void startup()
{
	//------------------------------------------------------------------------
	sc.rendering_program = compile_shaders("vproject1.glsl", "fproject1.glsl");

	glGenVertexArrays(1, &sc.vertex_array_object);
	glBindVertexArray(sc.vertex_array_object);

	GLuint buffer[2];

	make_model();

	/*
	//�ǽ� 3
	const GLfloat color[] = { 0.8f,0.8f,0.8f,0.8f };
	glClearBufferfv(GL_COLOR, 0, color);

	mat4 white, black;
	white = translate(mat4(1.0), vec3(sc.dispw, 0.1, 0.0));
	white = scale(white, vec3(0.2, 0.2, 0.0));
	glUniformMatrix4fv(sc.mv_location, 1, GL_FALSE, value_ptr(white));
	glUniform4f(sc.color, 1.0, 1.0, 1.0, 1.0);
	glDrawElements(GL_TRIANGLES, sc.numIndex, GL_UNSIGNED_INT, sc.indices);

	black = translate(mat4(1.0), vec3(sc.dispb, -0.1, 0.0));
	black = scale(black, vec3(0.2, 0.2, 0.0));
	glUniformMatrix4fv(sc.mv_location, 1, GL_FALSE, value_ptr(black));
	glUniform4f(sc.color, 1.0, 1.0, 1.0, 1.0);
	glDrawElements(GL_TRIANGLES, sc.numIndex, GL_UNSIGNED_INT, sc.indices);
	*/



	glGenBuffers(2, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sc.vertices), sc.vertices, GL_STATIC_DRAW);
	GLuint vPosition = glGetAttribLocation(sc.rendering_program, "vPosition");
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(vPosition);


	///*
	//�ǽ� 3������ �� ���� ����
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sc.colors), sc.colors, GL_STATIC_DRAW);
	GLuint vColor = glGetAttribLocation(sc.rendering_program, "vColor");
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(vColor);
	//*/



	sc.model = mat4(1.0);	//identity matrix
	/*sc.dispb = -1.0;	sc.dispw = -1.0;		//�ǽ� 3*/
	//glGetUniformLoction()���� uniform���� ������ mv_matrix�� �����ͼ� glUniformMatrix4fv()�� ��������ش�.
	//"vproject1.glsl": uniform mat4 mv_matrix;
	sc.mv_location = glGetUniformLocation(sc.rendering_program, "mv_matrix");
	//sc.model�� sc.mv_location�� ����->??
	//�޾ƿ� location, ���� ��� ����, GL_TRUE(��� ��ġ) or GL_FALSE(����� �� �켱 ������ �����), ������ ���
	glUniformMatrix4fv(sc.mv_location, 1, GL_FALSE, value_ptr( sc.model)); 
}

//animation, continuous update
void idle()
{
	/*
	//�ǽ� 1-rotate house
	//angle �������� 0.001������ �����ΰ� idle���� ��� ������Ʈ�� ��
	vec3 myRotationAxis(0, 0, 1);
	//���, ȸ������, ȸ���� axis
	sc.model = rotate(sc.model, sc.rotate_angle, myRotationAxis);
	*/



	///*
	//�ǽ� 2-house animation
	//T*S*p -> scale�ϰ� translate
	sc.theta += 0.01f;
	//x�����δ� cos��ŭ, y�����δ� sin��ŭ, 1. x^2+y^2=r^2	   
	//2.x=r*cos(theta), y=r*sin(theta), -180<theta<180->parameter�� �ϴ°� ����. radian���� ��ȯ�������
	sc.model = translate(mat4(1.0), vec3(0.5 * cos(radians(sc.theta)), 0.5 * sin(radians(sc.theta)), 0.0));
	//sc.model�� �̵��� ����ε� �� ��Ŀ� scale�ض�, x�� ������ 0.1�踸ŭ, y�� 0.3�踸ŭ
	sc.model = scale(sc.model, vec3(0.1, 0.3, 0.0));
	//*/


	/*
	//�ǽ� 3-tranlate squares
	sc.dispw += 0.0001;	if (sc.dispw > 1.0)	sc.dispw = -1.0;
	sc.dispb += 0.0002;	if (sc.dispb > 1.0)	sc.dispb = -1.0;
	*/


	//  ����ؼ� �����-�� ��ɾ ������ �� �׷�����.
	glutPostRedisplay();
}

//do all of your drawing
void render()
{
	glUseProgram(sc.rendering_program);

	const GLfloat color[] = { 0.8f, 0.8f, 0.8f, 0.8f };
	glClearBufferfv(GL_COLOR, 0, color);

	//98�� ����->�̰� ��� �� ������, render���� �ص�, idle()���� �Ұ� �� �ϰ� �ص� �ȴ�.
	glUniformMatrix4fv(sc.mv_location, 1, GL_FALSE, value_ptr(sc.model));

	//glDrawArrays(GL_TRIANGLES, 0, sc.num);
	glDrawElements(GL_TRIANGLES, sc.numIndex, GL_UNSIGNED_INT, sc.indices);  

	glutSwapBuffers();
}


void shutdown()
{
	glDeleteVertexArrays(1, &sc.vertex_array_object);
	glDeleteProgram(sc.rendering_program);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); 
	// MAC
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);

	glutInitWindowSize(800, 600);
	glutCreateWindow("2D Drawing");
	glewInit();
	version();

	startup();
	glutDisplayFunc(render);
	glutIdleFunc(idle);

	glutMainLoop();
	shutdown();
	return 0;
}
