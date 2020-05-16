//11.15, view.cpp 대신
#include "GL/cg.h"
#include "GL/camera.h"
#include "GL/vboplane.h"
#include "GL/vboteapot.h"

void startup();
void render();
void shutdown();
extern void version();

struct scene {
	int width = 800, height = 600;
	GLuint rendering_program;
	GLuint vertex_array_object;

	// 모델링 변수
	VBOPlane *plane;
	VBOTeapot *teapot;

	// 뷰잉 변수
	mat4 model, view, proj;
	GLint m_loc, v_loc, proj_loc, color_loc;
	float light_angle;
};
scene sc;

void startup()
{
	/******* OpenGL Initialization */
	glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL); //Passes if the incoming depth value is less than or
	/**** Shaders as variables */
	sc.rendering_program = compile_shaders("perfrag.glsl", "perfragf.glsl");
	glUseProgram(sc.rendering_program);

	// modeling
	sc.plane = new VBOPlane(50.0f, 50.0f, 1, 1);
	sc.teapot = new VBOTeapot(14, mat4(1.0f));

	// viewing
	sc.light_angle = 0.957283f;
	sc.model = mat4(1.0);
	
	sc.view = LookAt(vec3(0.0f, 1.5f, 15.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	float cameraZ = sc.height * 0.5 / tan(radians(0.5*50.0));
	sc.proj = Perspective(50.0f, (float)sc.width / (float)sc.height, (float)0.001*cameraZ, (float)10.0*cameraZ);	

		//ortho
	sc.m_loc = glGetUniformLocation(sc.rendering_program, "m_matrix");
	sc.v_loc = glGetUniformLocation(sc.rendering_program, "v_matrix");
	sc.proj_loc = glGetUniformLocation(sc.rendering_program, "proj_matrix");
	sc.color_loc = glGetUniformLocation(sc.rendering_program, "color");

	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(mat4(1.0)));
	glUniformMatrix4fv(sc.v_loc, 1, GL_FALSE, value_ptr(sc.view));
	glUniformMatrix4fv(sc.proj_loc, 1, GL_TRUE, value_ptr(sc.proj));

	// Lighting
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Light1.Intensity"), 0.8, 0.8, 0.8);
	//glUniform3f(glGetUniformLocation(sc.rendering_program, "Light2.Intensity"), 0.7, 0.3, 0.3);
}

void idle() {
	//*
	sc.light_angle += 0.001f;
	if (sc.light_angle > 2.0*3.141592) sc.light_angle -= 2.0*3.141592;//*/
	glutPostRedisplay();
}
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	glUseProgram(sc.rendering_program);

	// Lighting 
	glUniform4fv(glGetUniformLocation(sc.rendering_program, "Light1.Position"),
		1, value_ptr(vec4(10.0, 0.0f, 0.0f, 1.0f)));
	/*glUniform4fv(glGetUniformLocation(sc.rendering_program, "Light2.Position"),
		1, value_ptr(vec4(-10.0f, 0.0f, 0.0f, 1.0f)));*/
	/*glUniform4fv(glGetUniformLocation(sc.rendering_program, "Light1.Position"),
		1, value_ptr(vec4(10.0f*cos(sc.light_angle), 3.0f, 10.0f*sin(sc.light_angle), 1.0f)));*/
	/*
	glUniform4fv(glGetUniformLocation(sc.rendering_program, "Light2.Position"),
		1, value_ptr(vec4(10.0f * cos(sc.light_angle), 3.0f, 10.0f * sin(sc.light_angle), 1.0f)));//*/
	
	/******** Mesh  */
	//light를 정면으로 받을때와 측면으로 받을때 같다, 거의 빨간색
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.4f, 0.9f, 0.4f);
	//light를 정면으로 받을때와 측면으로 받을때 차이
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.15f, 0.15f, 0.15f);
	//specular: 반짝반짝하는거, 화이트톤에 가까우면 밝은 곳이 많고
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.1f, 0.1f, 0.1f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 100.0f);
	// Models

	//teapot
	mat4 m2;
	m2 = translate(mat4(1.0), vec3(0.0, 0.0, 0.0));
	m2 = rotate(m2, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m2));
	//glUniform4f(sc.color_loc, 0.2, 0.7, 0.7, 1.0);
	sc.teapot->render();


	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.66f, 0.4f, 0.27f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.7f, 0.7f, 0.7f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.1f, 0.1f, 0.1f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 180.0f);
	//
	// floor
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(mat4(1.0)));
	//glUniform4f(sc.color_loc, 0.3, 0.3, 0.3, 1.0);
	sc.plane->render();

		//추가------------------------
	/*glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.5f, 0.2f, 0.7f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.9f, 0.96f, 0.95f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.2f, 0.2f, 0.2f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 180.0f);

	mat4 t1;
	t1 = translate(mat4(1.0), vec3(0.0, 4.0, -4.0));
	t1 = rotate(t1, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(t1));
	sc.teapot->render();*/

	
	glutSwapBuffers();
}

void shutdown()
{
	glDeleteProgram(sc.rendering_program);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(sc.width, sc.height);
	glutCreateWindow("Phong Shading");
	glewInit();

	version();
	startup();
	glutDisplayFunc(render);
	glutIdleFunc(idle);

	glutMainLoop();
	shutdown();
	return 0;
}
