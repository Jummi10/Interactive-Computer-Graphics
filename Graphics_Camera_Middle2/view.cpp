//11.01
#include "GL/cg.h"
#include "GL/camera.h"
#include "GL/vbocube.h"
#include "GL/vboplane.h"
#include "GL/vboteapot.h"
#include "GL/vbosphere.h"

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

	// �𵨸� ����
	VBOCube *cube;
	VBOPlane *plane;
	VBOTeapot *teapot;
	VBOSphere* sphere;

	// ���� ����
	mat4 model, view, proj;
	GLint m_loc, v_loc, proj_loc, color_loc;	//shader->location �ϳ��� �ٰŶ�

	//ī�޶� ���� �ǽ�
	float theta;
};

scene sc;


// �� ó�� �ѹ� ����
void startup()
{
	//------------------------------------------------------------------------
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);

	sc.rendering_program = compile_shaders("vview.glsl", "fview.glsl");
	glUseProgram(sc.rendering_program);
	
	glGenVertexArrays(1, &sc.vertex_array_object);
	glBindVertexArray(sc.vertex_array_object);

	sc.cube = new VBOCube();	//ť�� ����
	sc.plane = new VBOPlane(20.0f, 10.0f, 10, 10);	//10 by 10 ũ��� �������� ����
	//sc.plane = new VBOPlane(10.0f, 5.0f, 10, 10);	//10 by 5 ũ��� �������� ����
	sc.teapot = new VBOTeapot(14, mat4(1.0));
	sc.sphere = new VBOSphere();
	
	//�츮�� ���� �Լ�=lookAt->glm library=LookAt
	sc.view = lookAt(vec3(0.0f, 3.0f, 8.0f),	//��� �ٶ󺸴°�(eye)
		vec3(0.0f, 0.0f, 0.0f),	//��� �ٶ󺸴°�
		vec3(0.0f, 1.0f, 0.0f));	//upside-down�� �� �� ��Ȳ�̴ϱ�(up)

	float cameraZ = sc.height * 0.5 / tan(radians(0.5*80.0));	//50.0 ��ſ� field of view / 0.5�� fovy�� ������ ���� ���� �ﰢ��->���� z�� ���ϱ�
	//sc.proj = Perspective(80.0f, 
	//	(float)sc.width/(float)sc.height, 
	//	(float)0.001*cameraZ, (float)10.0*cameraZ);	//near, far
	sc.proj = Perspective(50.0f, 1.33f, 0.5f, 100.0f);	//degree(����, radian���� �����), aspect(h/w->���ΰ� �� ª��)
	//sc.proj= Ortho(-5.0, 5.0,-2.0, 8.0, 0.3, 100.0);

	//ortho
	//���� �����ͼ�
	sc.m_loc = glGetUniformLocation(sc.rendering_program, "m_matrix");
	sc.v_loc = glGetUniformLocation(sc.rendering_program, "v_matrix");
	sc.proj_loc = glGetUniformLocation(sc.rendering_program, "proj_matrix");
	sc.color_loc = glGetUniformLocation(sc.rendering_program, "color");

	//�� �ֱ�
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(mat4(1.0))); 
	glUniformMatrix4fv(sc.v_loc, 1, GL_FALSE, value_ptr(sc.view));
	glUniformMatrix4fv(sc.proj_loc, 1, GL_TRUE, value_ptr(sc.proj));	

	sc.theta = 0.0f;
}

//  ����ؼ� �����
void idle()
{
	sc.theta += 0.001f;	//theta�� ��� �ٲ�����->�������� ������ֱ�
	
	glutPostRedisplay();
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	glUseProgram(sc.rendering_program);

	
	////���⼭ LookAt()�� �ٲ��ָ� ī�޶� �̵��Ѵ�. x�� �ٲ��ֱ�
	//float x, z;
	//x = 4.0 * sin(sc.theta);	//->�ð� ���� ȸ��
	//z = 4.0 * cos(sc.theta);	//x=cos(theta), y=sin()�̸� �ݽð� ���� ȸ��
	///*ī�޶� ��� ������ ���鼭 ��ü ���
	//sc.view = lookAt(vec3(x, 1.0f, z),
	//	vec3(0.0f, 0.0f, 0.0f),
	//	vec3(0.0f, 1.0f, 0.0f));
	//	*/

	////*ī�޶� 5��ŭ �ָ��ִٰ� ���� ť�� ������ �ٰ����ٰ� top�� �� �� ��� �ٽ� �� �־�����.
	//sc.view = lookAt(vec3(0.0f, 1.0f, 5.0f - sc.theta),
	//	vec3(0.0f, 0.0f, 0.0f),
	//	vec3(0.0f, 1.0f, 0.0f));
	//	//*/

	///*ī�޶� �ָ��ִٰ� ���� ť�� ������ �ٰ����ٰ� ����ģ��.
	//sc.view = lookAt(vec3(0.0f, 1.0f, 5.0f - sc.theta),
	//	vec3(0.0f, 0.0f, -sc.theta),
	//	vec3(0.0f, 1.0f, 0.0f));
	//*/
	
	//glUniformMatrix4fv(sc.v_loc, 1, GL_FALSE, value_ptr(sc.view));


	// floor
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(mat4(1.0)));	//(0,0,0)�ٰŴϱ� identity ��
	glUniform4f(sc.color_loc, 0.3, 0.3, 0.3, 1.0);
	sc.plane->render();	//plane�� ���ۿ� �ִ� �͸� ������ �׸��� �׸���.
     
	// Cube
	mat4 model_base;
	model_base = translate(mat4(1.0), vec3(2.0, 0.25, 0.0));	//(0,0,0)�� �׸��� ��鿡 �߸��ϱ� ���̸�ŭ(=scale �ݸ�ŭ) �÷��༭ ������� �ø���
	model_base = scale(model_base, vec3(1.0, 0.5, 1.0));	//0.5��ŭ scale
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(model_base));
	glUniform4f(sc.color_loc, 0.3, 0.2, 0.9, 1.0);
	sc.cube->render();	//cube�� vertexarray�� ���ε�ż�(vbocube.cpp~VBO::render()) �� ���� VA������ draw�Ѵ�.

	//Teapot-x������ rotate�ϱ�
	mat4 teapotmodel = scale(mat4(1.0), vec3(0.5, 0.5, 0.5));
	teapotmodel = rotate(teapotmodel, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(teapotmodel));
	glUniform4f(sc.color_loc, 0.7, 0.8, 0.5, 1.0);
	sc.teapot->render();

	teapotmodel = translate(teapotmodel, vec3(-5.0, 0.0, 0.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(teapotmodel));
	glUniform4f(sc.color_loc, 0.7, 0.8, 0.7, 1.0);
	sc.teapot->render();

	//Sphere-���� ũ�Ⱑ �ʹ� Ŀ�� scale ��û �۰� ���ֱ�, �ڵ��� �� TRS ����
	mat4 sm;
	sm = translate(mat4(1.0), vec3(4.0, 1.0, 0.0));
	sm = scale(sm, vec3(0.01, 0.01, 0.01));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(sm));
	glUniform4f(sc.color_loc, 0.2, 0.7, 0.2, 1.0);
	sc.sphere->render();

	
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

	glutInitWindowSize(sc.width, sc.height);
	glutCreateWindow("3D Viewing");
	glewInit();
	version();

	startup();
	glutDisplayFunc(render);
	glutIdleFunc(idle);

	glutMainLoop();
	shutdown();
	return 0;
}
