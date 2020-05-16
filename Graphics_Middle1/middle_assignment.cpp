#pragma comment (lib, "lib/freeglut.lib")
#pragma comment (lib, "lib/glew32.lib")
#include "GL/glew.h"	//���������� GL �Űܼ� ../GL/~
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "GL/cg-2.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <stdlib.h>
#include <iostream>
#include "time.h"
#include <vector>

#define PI 3.1415926535897932384626433832795

using namespace std;
using namespace glm;
void version();
GLuint compile_shaders(const char* vsource, const char* fsource);

void startup();
void render();
void shutdown();

vector<vec4> vertices[4];
GLuint rendering_program;
GLuint vertex_array_object;


struct scene {
	mat4 sphere1 = mat4(1.0f);	//vertices[0], upper chocolate icecream, falling icecream
	mat4 sphere2 = mat4(1.0f);	//vertices[1], lower vanilla icecream
	mat4 cone = mat4(1.0f);	//vertices[2], cone
	mat4 sprinkles[9];	//vertices[3], 9 sprinkles, backgrnd
	GLint mv_location;

	GLfloat rotate_angle = 0.05;	//icecream ��ü delta ȸ������
	int rotate_num = 0;	//icecream�� ȸ���ϴ� ���� idle()�� ȣ��Ǵ� Ƚ��
};

scene icecream;

void make_icecream()
{
	float theta, pi;
	float delta_theta, delta_pi;
	float start_theta, start_pi;


	//upper sphere, chocolate icecream
	float finish_theta = 21; float finish_pi = finish_theta;
	start_theta = 0.0;
	delta_theta = 2 * PI / finish_theta;
	start_pi = -PI / 2.0;
	delta_pi = PI / (finish_pi - 1);

	for (int j = 0; j < finish_pi; j++)
	{
		pi = start_pi + j * delta_pi;	//-180���� 180����(-y ~ y)
		for (int i = 0; i < finish_theta; i++)
		{
			theta = start_theta + i * delta_theta;	//0 ~ 360

			//�߽� (0.0, 0.4, 0.0), ������ = 0.2
			vertices[0].emplace_back(0.2 * cos(pi) * cos(theta), 0.4 + 0.2 * cos(pi) * sin(theta), 0.2 * sin(pi), 1.0);
			vertices[0].emplace_back(0.2 * cos(pi) * cos(theta + delta_theta), 0.4 + 0.2 * cos(pi) * sin(theta + delta_theta), 0.2 * sin(pi), 1.0);
			vertices[0].emplace_back(0.2 * cos(pi + delta_pi) * cos(theta + delta_theta), 0.4 + 0.2 * cos(pi + delta_pi) * sin(theta + delta_theta), 0.2 * sin(pi + delta_pi), 1.0);

			vertices[0].emplace_back(0.2 * cos(pi) * cos(theta), 0.4 + 0.2 * cos(pi) * sin(theta), 0.2 * sin(pi), 1.0);
			vertices[0].emplace_back(0.2 * cos(pi + delta_pi) * cos(theta + delta_theta), 0.4 + 0.2 * cos(pi + delta_pi) * sin(theta + delta_theta), 0.2 * sin(pi + delta_pi), 1.0);
			vertices[0].emplace_back(0.2 * cos(pi + delta_pi) * cos(theta), 0.4 + 0.2 * cos(pi + delta_pi) * sin(theta), 0.2 * sin(pi + delta_pi), 1.0);
		}
	}




	//lower sphere, vanilla icecream
	start_theta = 0.0;
	delta_theta = 2 * PI / finish_theta;
	start_pi = -PI / 2.0;
	delta_pi = PI / (finish_pi - 1);

	for (int j = 0; j < finish_pi; j++)
	{
		pi = start_pi + j * delta_pi;
		for (int i = 0; i < finish_theta; i++)
		{
			theta = start_theta + i * delta_theta;

			//�߽� (0.0, 0.05, 0.0), ������ = 0.25
			vertices[1].emplace_back(0.25 * cos(pi) * cos(theta), 0.05 + 0.25 * cos(pi) * sin(theta), 0.25 * sin(pi), 1.0);
			vertices[1].emplace_back(0.25 * cos(pi) * cos(theta + delta_theta), 0.05 + 0.25 * cos(pi) * sin(theta + delta_theta), 0.25 * sin(pi), 1.0);
			vertices[1].emplace_back(0.25 * cos(pi + delta_pi) * cos(theta + delta_theta), 0.05 + 0.25 * cos(pi + delta_pi) * sin(theta + delta_theta), 0.25 * sin(pi + delta_pi), 1.0);

			vertices[1].emplace_back(0.25 * cos(pi) * cos(theta), 0.05 + 0.25 * cos(pi) * sin(theta), 0.25 * sin(pi), 1.0);
			vertices[1].emplace_back(0.25 * cos(pi + delta_pi) * cos(theta + delta_theta), 0.05 + 0.25 * cos(pi + delta_pi) * sin(theta + delta_theta), 0.25 * sin(pi + delta_pi), 1.0);
			vertices[1].emplace_back(0.25 * cos(pi + delta_pi) * cos(theta), 0.05 + 0.25 * cos(pi + delta_pi) * sin(theta), 0.25 * sin(pi + delta_pi), 1.0);
		}
	}


	//cone
	vertices[2].emplace_back(0.0, -0.6, 0.0, 1.0);		//������ 
	for (int i = 0; i < 361; i++) {	//���� 360�� �����鼭 gl_triangle_fan���� �׸���
		float angle = radians(float(i));
		vertices[2].emplace_back(0.24 * cos(angle), -0.03, 0.24 * sin(angle), 1.0);
	}


	//sprinkles
	float a = 2;
	vertices[3].emplace_back(-0.01 * a, -0.03 * a, 0.0, 1.0);	//sprinkles[0]
	vertices[3].emplace_back(-0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, -0.03 * a, 0.0, 1.0);

	vertices[3].emplace_back(-0.01 * a, -0.03 * a, 0.0, 1.0);	//sprinkles[1]
	vertices[3].emplace_back(-0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, -0.03 * a, 0.0, 1.0);

	vertices[3].emplace_back(-0.01 * a, -0.03 * a, 0.0, 1.0);	//sprinkles[2]
	vertices[3].emplace_back(-0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, -0.03 * a, 0.0, 1.0);


	vertices[3].emplace_back(-0.01 * a, -0.03 * a, 0.0, 1.0);	//sprinkles[3]
	vertices[3].emplace_back(-0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, -0.03 * a, 0.0, 1.0);

	vertices[3].emplace_back(-0.01 * a, -0.03 * a, 0.0, 1.0);	//sprinkles[4]
	vertices[3].emplace_back(-0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, -0.03 * a, 0.0, 1.0);

	vertices[3].emplace_back(-0.01 * a, -0.03 * a, 0.0, 1.0);	//sprinkles[5]
	vertices[3].emplace_back(-0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, -0.03 * a, 0.0, 1.0);


	vertices[3].emplace_back(-0.01 * a, -0.03 * a, 0.0, 1.0);	//sprinkles[6]
	vertices[3].emplace_back(-0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, -0.03 * a, 0.0, 1.0);

	vertices[3].emplace_back(-0.01 * a, -0.03 * a, 0.0, 1.0);	//sprinkles[7]
	vertices[3].emplace_back(-0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, -0.03 * a, 0.0, 1.0);

	vertices[3].emplace_back(-0.01 * a, -0.03 * a, 0.0, 1.0);	//sprinkles[8]
	vertices[3].emplace_back(-0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, 0.03 * a, 0.0, 1.0);
	vertices[3].emplace_back(0.01 * a, -0.03 * a, 0.0, 1.0);
}

// �� ó�� �ѹ� ����
void startup()
{
	//------------------------------------------------------------------------
	rendering_program = compile_shaders("vproject1.glsl", "fmiddle_assign.glsl");

	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);


	make_icecream();


	GLuint buffer[4];

	glGenBuffers(4, buffer);

	for (int i = 0; i < 4; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);
		glBufferData(GL_ARRAY_BUFFER, vertices[i].size() * sizeof(vec4), vertices[i].data(), GL_STATIC_DRAW);
		GLuint vPosition = glGetAttribLocation(rendering_program, "vPosition");
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(vPosition);
	}

	for (int i = 0; i < 9; i++) {
		icecream.sprinkles[i] = mat4(1.0);
	}
	icecream.sprinkles[0] = translate(icecream.sprinkles[0], vec3(-0.7, 0.7, 0.0));

	icecream.sprinkles[1] = translate(icecream.sprinkles[1], vec3(0.0, 0.7, 0.0));
	icecream.sprinkles[1] = scale(icecream.sprinkles[1], vec3(0.8, 0.8, 0.0));

	icecream.sprinkles[2] = translate(icecream.sprinkles[2], vec3(0.7, 0.7, 0.0));
	icecream.sprinkles[2] = scale(icecream.sprinkles[2], vec3(1.1, 1.1, 0.0));

	icecream.sprinkles[3] = translate(icecream.sprinkles[3], vec3(-0.7, 0.0, 0.0));
	icecream.sprinkles[3] = scale(icecream.sprinkles[3], vec3(0.9, 0.9, 0.0));

	icecream.sprinkles[4] = translate(icecream.sprinkles[4], vec3(0.0, 0.0, 0.0));

	icecream.sprinkles[5] = translate(icecream.sprinkles[5], vec3(0.7, 0.0, 0.0));
	icecream.sprinkles[5] = scale(icecream.sprinkles[5], vec3(0.9, 0.9, 0.0));

	icecream.sprinkles[6] = translate(icecream.sprinkles[6], vec3(-0.7, -0.7, 0.0));
	icecream.sprinkles[6] = scale(icecream.sprinkles[6], vec3(1.1, 1.1, 0.0));

	icecream.sprinkles[7] = translate(icecream.sprinkles[7], vec3(0.0, -0.7, 0.0));
	icecream.sprinkles[7] = scale(icecream.sprinkles[7], vec3(0.95, 0.95, 0.0));

	icecream.sprinkles[8] = translate(icecream.sprinkles[8], vec3(0.7, -0.7, 0.0));

	//glGetUniformLoction()���� uniform���� ������ mv_matrix�� �����ͼ� glUniformMatrix4fv()�� ��������ش�.
	//"vproject1.glsl": uniform mat4 mv_matrix;
	icecream.mv_location = glGetUniformLocation(rendering_program, "mv_matrix");
	//�޾ƿ� location, ���� ��� ����, GL_TRUE(��� ��ġ) or GL_FALSE(����� �� �켱 ������ �����), ������ ���
	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.sphere1));
	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.sphere2));
	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.cone));
	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(*icecream.sprinkles));
}

//animation, continuous update
void idle()
{
	icecream.rotate_num++;
	vec3 rotate_axis = vec3(-1.0, 0.0, 1.0);
	if (icecream.rotate_num < 900) {
		icecream.sphere1 = rotate(icecream.sphere1, radians(icecream.rotate_angle), rotate_axis);
		icecream.sphere2 = rotate(icecream.sphere2, radians(icecream.rotate_angle), rotate_axis);
		icecream.cone = rotate(icecream.cone, radians(icecream.rotate_angle), rotate_axis);
	}
	else {	//0.05*900=45�� ȸ���ϸ�, ȸ�� ���߰� upper icecream�� ����߷���
		icecream.sphere1 = translate(icecream.sphere1, vec3(-0.001, -0.001, 0.0));
	}

	for (int i = 0; i < 9; i++) {
		if (i % 2 == 0) {	//¦����° ������Ŭ�̸� ������ ���ƶ�
			icecream.sprinkles[i] = rotate(icecream.sprinkles[i], radians(0.1f), vec3(0.0, 0.0, 1.0));
		}
		else {	//Ȧ����° ������Ŭ�̸� ������ ���ƶ�
			icecream.sprinkles[i] = rotate(icecream.sprinkles[i], radians(0.15f), vec3(0.0, 0.0, 1.0));
		}
	}
	

	//  ����ؼ� �����-�� ��ɾ ������ �� �׷�����.
	glutPostRedisplay();
}

//do all of your drawing
void render()
{
	glUseProgram(rendering_program);

	const GLfloat color[] = { 0.8f, 0.8f, 0.8f, 0.8f };
	glClearBufferfv(GL_COLOR, 0, color);


	//0=���ڻ�, 1=�ٴҶ��, 2=�ܻ�, 3=�ϴû�, 4=��ȫ��, 5=���λ�->fragment shader, "fproject1.glsl"���� �� ����
	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.sphere1));	//���ڻ�
	//glUniform1i(glGetUniformLocation(rendering_program, "i"), i);//�༺���� ���� ������ ������ ���� if�� index shader�� ����.
	glUniform1i(glGetUniformLocation(rendering_program, "i"), 0);
	glBufferData(GL_ARRAY_BUFFER, vertices[0].size() * sizeof(vec4), vertices[0].data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, vertices[0].size());	//vertices[0]�� 0��° index���� vertices[0].size()��ŭ GL_TRIANGLES�� �׸��ڴ�


	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.sphere2));
	glUniform1i(glGetUniformLocation(rendering_program, "i"), 1);	//�ٴҶ��
	glBufferData(GL_ARRAY_BUFFER, vertices[1].size() * sizeof(vec4), vertices[1].data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, vertices[1].size());


	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.cone));
	glUniform1i(glGetUniformLocation(rendering_program, "i"), 2);	//Ȳ���
	glBufferData(GL_ARRAY_BUFFER, vertices[2].size() * sizeof(vec4), vertices[2].data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices[2].size());

	//
	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.sprinkles[0]));
	glUniform1i(glGetUniformLocation(rendering_program, "i"), 3);	//�ϴû�
	glBufferData(GL_ARRAY_BUFFER, vertices[3].size() * sizeof(vec4), vertices[3].data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.sprinkles[1]));
	glUniform1i(glGetUniformLocation(rendering_program, "i"), 4);	//��ȫ��
	glBufferData(GL_ARRAY_BUFFER, vertices[3].size() * sizeof(vec4), vertices[3].data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.sprinkles[2]));
	glUniform1i(glGetUniformLocation(rendering_program, "i"), 5);	//���λ�
	glBufferData(GL_ARRAY_BUFFER, vertices[3].size() * sizeof(vec4), vertices[3].data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	//
	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.sprinkles[3]));
	glUniform1i(glGetUniformLocation(rendering_program, "i"), 4);	//��ȫ��
	glBufferData(GL_ARRAY_BUFFER, vertices[3].size() * sizeof(vec4), vertices[3].data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	//���̽�ũ���� ��ħ
	//glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.sprinkles[4]));
	//glUniform1i(glGetUniformLocation(rendering_program, "i"), 5);	//���λ�
	//glBufferData(GL_ARRAY_BUFFER, vertices[3].size() * sizeof(vec4), vertices[3].data(), GL_STATIC_DRAW);
	//glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.sprinkles[5]));
	glUniform1i(glGetUniformLocation(rendering_program, "i"), 3);	//�ϴû�
	glBufferData(GL_ARRAY_BUFFER, vertices[3].size() * sizeof(vec4), vertices[3].data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
	//
	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.sprinkles[6]));
	glUniform1i(glGetUniformLocation(rendering_program, "i"), 5);	//���λ�
	glBufferData(GL_ARRAY_BUFFER, vertices[3].size() * sizeof(vec4), vertices[3].data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 4);

	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.sprinkles[7]));
	glUniform1i(glGetUniformLocation(rendering_program, "i"), 3);	//�ϴû�
	glBufferData(GL_ARRAY_BUFFER, vertices[3].size() * sizeof(vec4), vertices[3].data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 28, 4);

	glUniformMatrix4fv(icecream.mv_location, 1, GL_FALSE, value_ptr(icecream.sprinkles[8]));
	glUniform1i(glGetUniformLocation(rendering_program, "i"), 4);	//��ȫ��
	glBufferData(GL_ARRAY_BUFFER, vertices[3].size() * sizeof(vec4), vertices[3].data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 32, 4);


	glutSwapBuffers();
}


void shutdown()
{
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteProgram(rendering_program);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(800, 800);
	glutCreateWindow("IceCream");
	glewInit();
	version();

	startup();
	glutDisplayFunc(render);
	glutIdleFunc(idle);

	glutMainLoop();
	shutdown();
	return 0;
}