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

	// 모델링 변수
	VBOCube *cube;
	VBOPlane *plane;
	VBOTeapot *teapot;
	VBOSphere* sphere;

	// 뷰잉 변수
	mat4 model, view, proj;
	GLint m_loc, v_loc, proj_loc, color_loc;	//shader->location 하나씩 줄거라서

	//카메라 돌기 실습
	float theta;
};

scene sc;


// 맨 처음 한번 실행
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

	sc.cube = new VBOCube();	//큐브 생성
	sc.plane = new VBOPlane(20.0f, 10.0f, 10, 10);	//10 by 10 크기로 조각조각 내라
	//sc.plane = new VBOPlane(10.0f, 5.0f, 10, 10);	//10 by 5 크기로 조각조각 내라
	sc.teapot = new VBOTeapot(14, mat4(1.0));
	sc.sphere = new VBOSphere();
	
	//우리가 만든 함수=lookAt->glm library=LookAt
	sc.view = lookAt(vec3(0.0f, 3.0f, 8.0f),	//어디서 바라보는가(eye)
		vec3(0.0f, 0.0f, 0.0f),	//어디를 바라보는가
		vec3(0.0f, 1.0f, 0.0f));	//upside-down이 안 된 상황이니까(up)

	float cameraZ = sc.height * 0.5 / tan(radians(0.5*80.0));	//50.0 대신에 field of view / 0.5는 fovy를 반으로 갈라서 만든 삼각형->그의 z값 구하기
	//sc.proj = Perspective(80.0f, 
	//	(float)sc.width/(float)sc.height, 
	//	(float)0.001*cameraZ, (float)10.0*cameraZ);	//near, far
	sc.proj = Perspective(50.0f, 1.33f, 0.5f, 100.0f);	//degree(각도, radian으로 줘야해), aspect(h/w->가로가 더 짧다)
	//sc.proj= Ortho(-5.0, 5.0,-2.0, 8.0, 0.3, 100.0);

	//ortho
	//각각 가져와서
	sc.m_loc = glGetUniformLocation(sc.rendering_program, "m_matrix");
	sc.v_loc = glGetUniformLocation(sc.rendering_program, "v_matrix");
	sc.proj_loc = glGetUniformLocation(sc.rendering_program, "proj_matrix");
	sc.color_loc = glGetUniformLocation(sc.rendering_program, "color");

	//값 주기
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(mat4(1.0))); 
	glUniformMatrix4fv(sc.v_loc, 1, GL_FALSE, value_ptr(sc.view));
	glUniformMatrix4fv(sc.proj_loc, 1, GL_TRUE, value_ptr(sc.proj));	

	sc.theta = 0.0f;
}

//  계속해서 실행됨
void idle()
{
	sc.theta += 0.001f;	//theta가 계속 바껴야함->전역으로 만들어주기
	
	glutPostRedisplay();
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	glUseProgram(sc.rendering_program);

	
	////여기서 LookAt()을 바꿔주면 카메라가 이동한다. x를 바꿔주기
	//float x, z;
	//x = 4.0 * sin(sc.theta);	//->시계 방향 회전
	//z = 4.0 * cos(sc.theta);	//x=cos(theta), y=sin()이면 반시계 방향 회전
	///*카메라 평면 위에서 돌면서 물체 찍기
	//sc.view = lookAt(vec3(x, 1.0f, z),
	//	vec3(0.0f, 0.0f, 0.0f),
	//	vec3(0.0f, 1.0f, 0.0f));
	//	*/

	////*카메라가 5만큼 멀리있다가 점점 큐브 쪽으로 다가가다가 top을 한 번 찍고 다시 쭉 멀어진다.
	//sc.view = lookAt(vec3(0.0f, 1.0f, 5.0f - sc.theta),
	//	vec3(0.0f, 0.0f, 0.0f),
	//	vec3(0.0f, 1.0f, 0.0f));
	//	//*/

	///*카메라가 멀리있다가 점점 큐브 쪽으로 다가가다가 지나친다.
	//sc.view = lookAt(vec3(0.0f, 1.0f, 5.0f - sc.theta),
	//	vec3(0.0f, 0.0f, -sc.theta),
	//	vec3(0.0f, 1.0f, 0.0f));
	//*/
	
	//glUniformMatrix4fv(sc.v_loc, 1, GL_FALSE, value_ptr(sc.view));


	// floor
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(mat4(1.0)));	//(0,0,0)줄거니까 identity 줌
	glUniform4f(sc.color_loc, 0.3, 0.3, 0.3, 1.0);
	sc.plane->render();	//plane의 버퍼에 있는 것만 가지고 그림을 그린다.
     
	// Cube
	mat4 model_base;
	model_base = translate(mat4(1.0), vec3(2.0, 0.25, 0.0));	//(0,0,0)에 그리면 평면에 잘리니까 높이만큼(=scale 반만큼) 올려줘서 평면위에 올리기
	model_base = scale(model_base, vec3(1.0, 0.5, 1.0));	//0.5만큼 scale
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(model_base));
	glUniform4f(sc.color_loc, 0.3, 0.2, 0.9, 1.0);
	sc.cube->render();	//cube의 vertexarray가 바인드돼서(vbocube.cpp~VBO::render()) 그 안의 VA가지고만 draw한다.

	//Teapot-x축으로 rotate하기
	mat4 teapotmodel = scale(mat4(1.0), vec3(0.5, 0.5, 0.5));
	teapotmodel = rotate(teapotmodel, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(teapotmodel));
	glUniform4f(sc.color_loc, 0.7, 0.8, 0.5, 1.0);
	sc.teapot->render();

	teapotmodel = translate(teapotmodel, vec3(-5.0, 0.0, 0.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(teapotmodel));
	glUniform4f(sc.color_loc, 0.7, 0.8, 0.7, 1.0);
	sc.teapot->render();

	//Sphere-원래 크기가 너무 커서 scale 엄청 작게 해주기, 코딩할 땐 TRS 순서
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
