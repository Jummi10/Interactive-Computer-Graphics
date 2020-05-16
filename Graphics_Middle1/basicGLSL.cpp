//0927
#pragma comment (lib, "lib/freeglut.lib")
#pragma comment (lib, "lib/glew32.lib")
#include "GL/glew.h"	//상위폴더에 GL 옮겨서 ../GL/~
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"

#include "GL/cg-1.h"

void startup();
void render();
void shutdown();
const int WIDTH = 800; const int HEIGHT = 600;

struct scene {
	int width = 800, height = 600;
	GLuint rendering_program;
	GLuint vertex_array_object;
};

scene sc;

void render()
{
	const GLfloat color[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, color);

	glUseProgram(sc.rendering_program);
	//  0. // Point
	glPointSize(40.0f);
	glDrawArrays(GL_POINTS, 0, 1);

	glutSwapBuffers();
}

void idle() { glutPostRedisplay(); }

void startup()
{
	/**** Shaders as variables */
	//0. Point
	sc.rendering_program = compile_shaders("v0.glsl", "f0.glsl");

	/* Vertex Array Object */
	glGenVertexArrays(1, &sc.vertex_array_object);
	glBindVertexArray(sc.vertex_array_object);
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
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Point");
	glewInit();

	version();
	startup();
	glutDisplayFunc(render);
	glutIdleFunc(idle);

	glutMainLoop();

	system("pause");

	shutdown();
	return 0;
}