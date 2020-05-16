//11.15, view.cpp 대신
#include "GL/cg.h"
#include "GL/camera.h"
#include "GL/vboplane.h"
#include "GL/vbomesh.h"
#include "GL/bmpreader.h"
#include <vector>
#include <algorithm>
#define M_PI 3.141592

void startup();
void render();
void shutdown();
extern void version();
extern GLuint compile_shaders(const char* vsource, const char* fsource);

struct scene {
	int width = 800, height = 600;
	GLuint rendering_program;
	GLuint vertex_array_object;

	// 모델링 변수
	VBOPlane* ground;
	VBOPlane* sky1;
	VBOPlane* sky2;
	VBOPlane* sky3;
	VBOPlane* sky4;
	VBOMesh* wrand;
	VBOMesh* snitch_body;
	VBOMesh* stadium;
	VBOMesh* wingR;
	VBOMesh* wingL;
	VBOMesh* broom;

	// 뷰잉 변수
	mat4 model_body, view, proj;
	GLint m_loc, v_loc, proj_loc, color_loc;
	float light_angle;
	float camera_angle;
	float camera_speed;
	float broom_angle;

	//hierarchy
	vector <mat4> mvs;

	//texture
	GLuint tex_object[3];
};
scene sc;

struct model {
	// body tx, body ty, body tz, wingR roty, wingL roty
	float opt[5] = { 0.0, 0.0, 0.0, -35.1f, 35.1f };
};
model snitch;


float deltaAngle = 0.0f;
int xOrigin = -1;
static float myAngle = 0.0;
// actual vector representing the camera’s direction
//static float lx = -50.0f, lz = 1500.0f, ly = 20.0;
static float lx = 0.0f, lz = -1.0f, ly = 0.0;
// XZ position of the camera
static float myX = -50.0f, myZ = 1500.0f, myY = 50.0;
//static float myX = 0.0f, myZ = 5.0f, myY = 1.0;

void mouseMove(int x, int y) {
	if (xOrigin >= 0) {


		deltaAngle = (x - xOrigin) * 0.001f;


		lx = sin(myAngle + deltaAngle);
		lz = -cos(myAngle + deltaAngle);
	}
	sc.view = LookAt(vec3(myX, myY, myZ), vec3(myX + lx, myY, myZ + lz), vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(sc.v_loc, 1, GL_FALSE, value_ptr(sc.view));
}
void mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			//isDragging = 1;
			//xDragStart = x;
			//yDragStart = y;
			xOrigin = x;
			myAngle -= deltaAngle;
		}

		else if (state == GLUT_UP) {
			myAngle += deltaAngle;
			xOrigin = -1;
		}
	}
	//if (button == GLUT_LEFT_BUTTON) {


	//	if (state == GLUT_UP) {
	//		myAngle += deltaAngle;
	//		xOrigin = -1;
	//	}
	//	else {
	//		xOrigin = x;
	//	}
	//}
}

void processNormalKeys(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 27:
		exit(0);
		break;
		//case 'w':
		//	rotate_x -= 5;
		//	break;
		//case 's':
		//	rotate_x += 5;
		//	break;
		//case 'a':
		//	rotate_y -= 5;
		//	break;
		//case 'd':
		//	rotate_y += 5;
		//	break;
		//case 'z':
		//	rotate_z += 5;
		//	break;
		//case 'x':
		//	rotate_z -= 5;
		//	break;
	}
}

void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 10.0f;

	switch (key) {
	case GLUT_KEY_LEFT:
		myAngle += 0.01f;
		lx = sin(myAngle);
		lz = -cos(myAngle);
		break;
	case GLUT_KEY_RIGHT:
		myAngle -= 0.01f;
		lx = sin(myAngle);
		lz = -cos(myAngle);
		break;
	case GLUT_KEY_UP:
		myX += lx * fraction;
		myZ += lz * fraction;
		break;
	case GLUT_KEY_DOWN:
		myX -= lx * fraction;
		myZ -= lz * fraction;
		break;
	}

	sc.view = LookAt(vec3(myX, myY, myZ), vec3(myX + lx, myY, myZ + lz), vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(sc.v_loc, 1, GL_FALSE, value_ptr(sc.view));
}


void startup()
{
	/******* OpenGL Initialization */
	glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL); //Passes if the incoming depth value is less than or
	glEnable(GL_SMOOTH);
	/**** Shaders as variables */
	sc.rendering_program = compile_shaders("tex_lighting_v.glsl", "tex_lighting_f.glsl");
	glUseProgram(sc.rendering_program);

	// modeling
	sc.ground = new VBOPlane(500.0f, 500.0f, 1, 1);
	sc.sky1 = new VBOPlane(8000.0f, 8000.0f, 1, 1);
	sc.sky2 = new VBOPlane(8000.0f, 8000.0f, 1, 1);
	sc.sky3 = new VBOPlane(8000.0f, 8000.0f, 1, 1);
	sc.sky4 = new VBOPlane(8000.0f, 8000.0f, 1, 1);
	sc.wrand = new VBOMesh("wrand.obj", true);
	sc.snitch_body = new VBOMesh("sphere.obj", true);
	sc.stadium = new VBOMesh("sports stadium.obj", true);
	sc.wingR = new VBOMesh("wingR.obj", true);
	sc.wingL = new VBOMesh("wingL.obj", true);
	sc.broom = new VBOMesh("broom.obj", true);


	// viewing
	sc.light_angle = 0.0f;
	sc.camera_angle = 0.0f;
	sc.broom_angle = 0.0f;
	sc.model_body = mat4(1.0);

	//sc.view = LookAt(vec3(0.0f, 1.5f, 20.0f), vec3(0.0f, 2.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	sc.view = LookAt(vec3(-50.0f, 50.0f, 1500.0f), vec3(0.0f, -50.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));	//첫 위치

	float cameraZ = sc.height * 0.5 / tan(radians(0.5 * 50.0));
	sc.proj = Perspective(50.0f, (float)sc.width / (float)sc.height, (float)0.001 * cameraZ, (float)10.0 * cameraZ);

	//ortho
	sc.m_loc = glGetUniformLocation(sc.rendering_program, "m_matrix");
	sc.v_loc = glGetUniformLocation(sc.rendering_program, "v_matrix");
	sc.proj_loc = glGetUniformLocation(sc.rendering_program, "proj_matrix");
	sc.color_loc = glGetUniformLocation(sc.rendering_program, "color");

	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(mat4(1.0)));
	glUniformMatrix4fv(sc.v_loc, 1, GL_FALSE, value_ptr(sc.view));
	glUniformMatrix4fv(sc.proj_loc, 1, GL_TRUE, value_ptr(sc.proj));

	// Lighting
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Light.Intensity"), 0.8, 0.8, 0.8);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Spot.Intensity"), 0.09, 0.09, 0.09);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Spot.exponent"), 10.0f);	//값 키우면 어두워진다.
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Spot.cutoff"), 10.0f);	//30도, 30도해서 60도

	// Load texture file
	glActiveTexture(GL_TEXTURE0);
	sc.tex_object[0] = BMPReader::loadTex("soil2_tex.bmp");
	glActiveTexture(GL_TEXTURE1);
	sc.tex_object[1] = BMPReader::loadTex("white_tex.bmp");
	glActiveTexture(GL_TEXTURE2);
	sc.tex_object[1] = BMPReader::loadTex("sky_tex.bmp");
}

bool flag = true;
void idle() {
	sc.camera_angle += 0.001f;
	sc.camera_speed += 0.003f;
	sc.light_angle += 0.001f;
	if (sc.light_angle > 2.0 * 3.141592) sc.light_angle -= 2.0 * 3.141592;
	sc.broom_angle += 0.01f;

	//hierarchy
	//    0       1         2         3           4
	// base tx, base ty, base tz, wingR roty, wingL roty
	snitch.opt[2] += 0.03f;
	if (flag) {
		snitch.opt[3] += 0.7f;
		snitch.opt[4] -= 0.7f;
	}
	else {
		snitch.opt[3] -= 0.7f;
		snitch.opt[4] += 0.7f;
	}

	if (snitch.opt[3] < -35) {
		flag = true;
	}
	else if (snitch.opt[3] > 35) {
		flag = false;
	}

	// spotlight
	sc.light_angle += 0.005f;
	if (sc.light_angle > 2.0 * M_PI) sc.light_angle -= 2.0 * M_PI;

	glutPostRedisplay();
}
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClearColor(0.71f, 0.905f, 0.905f, 1.0f);
	glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL);
	glUseProgram(sc.rendering_program);

	float x, z;
	x = 15.0 * cos(sc.camera_angle);
	z = 15.0 * sin(sc.camera_angle);	//반시계 방향 회전
	//카메라 평면 위에서 돌면서 물체 찍기
	//sc.view = lookAt(vec3(x, 3.0f, z), vec3(0.0f, 3.0f, 0.0f), vec3(0.0f, 2.0f, 0.0f));
	//sc.view = LookAt(vec3(0.0f, 1.5f, 20.0f + sc.camera_speed), vec3(0.0f, 2.0f, 0.0f + sc.camera_speed), vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(sc.v_loc, 1, GL_FALSE, value_ptr(sc.view));


	// Lighting 
	glUniform4fv(glGetUniformLocation(sc.rendering_program, "Light.Position"),
		1, value_ptr(vec4(10.0, 0.0f, 10.0f, 1.0f)));
	vec4 lightPos = vec4(15.0f * cos(sc.light_angle), 15.0f, 15.0f * sin(sc.light_angle), 1.0f);	//빛 회전
	vec4 direction = sc.view * vec4(-lightPos);
	glUniform4fv(glGetUniformLocation(sc.rendering_program, "Spot.Position"), 1, value_ptr(sc.view * lightPos));	//view곱해준다. 카메라 값
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Spot.direction"), direction[0], direction[1], direction[2]);

	/******** Mesh  */


	// ground
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.66f, 0.46f, 0.32f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.7f, 0.7f, 0.7f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.4f, 0.4f, 0.4f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 80.0f);
	glUniform1i(glGetUniformLocation(sc.rendering_program, "Tex1"), 0);
	mat4 ground;
	ground = translate(mat4(1.0), vec3(0.0, -350.0, 0.0));
	ground = scale(ground, vec3(10.0, 10.0, 10.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(ground));
	sc.ground->render();


	// sky1, -z-dir
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.7f, 0.7f, 0.7f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.6f, 0.6f, 0.6f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.4f, 0.4f, 0.4f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 80.0f);
	glUniform1i(glGetUniformLocation(sc.rendering_program, "Tex1"), 2);
	mat4 sky1;
	sky1 = translate(mat4(1.0), vec3(0.0, 3500.0, -2500.0));
	sky1 = rotate(sky1, radians(90.0f), vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(sky1));
	sc.sky1->render();


	// sky2, z-dir
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.7f, 0.7f, 0.7f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.6f, 0.6f, 0.6f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.4f, 0.4f, 0.4f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 80.0f);
	glUniform1i(glGetUniformLocation(sc.rendering_program, "Tex1"), 2);
	mat4 sky2;
	sky2 = translate(mat4(1.0), vec3(0.0, 3500.0, 2500.0));
	sky2 = rotate(sky2, radians(-90.0f), vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(sky2));
	sc.sky2->render();


	// sky3, z-dir
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.7f, 0.7f, 0.7f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.6f, 0.6f, 0.6f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.4f, 0.4f, 0.4f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 80.0f);
	glUniform1i(glGetUniformLocation(sc.rendering_program, "Tex1"), 2);
	mat4 sky3;
	sky3 = translate(mat4(1.0), vec3(2500.0, 3500.0, 0.0));
	sky3 = rotate(sky3, radians(-90.0f), vec3(0.0, 0.0, 1.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(sky3));
	sc.sky3->render();


	// sky4, z-dir
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.7f, 0.7f, 0.7f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.6f, 0.6f, 0.6f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.4f, 0.4f, 0.4f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 80.0f);
	glUniform1i(glGetUniformLocation(sc.rendering_program, "Tex1"), 2);
	mat4 sky4;
	sky4 = translate(mat4(1.0), vec3(-2500.0, 3500.0, 0.0));
	sky4 = rotate(sky4, radians(-90.0f), vec3(0.0, 0.0, 1.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(sky4));
	sc.sky4->render();


	// stadium
	//glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.94f, 0.60f, 0.51f);	//연한 다홍
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.92f, 0.62f, 0.33f);	//연두
	//glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.75f, 0.75f, 0.75f);	//흰색같은 회색
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.4f, 0.4f, 0.4f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 180.0f);
	glUniform1i(glGetUniformLocation(sc.rendering_program, "Tex1"), 1);
	mat4 stadium;
	stadium = translate(mat4(1.0), vec3(0.0, 235.0, 0.0));
	stadium = scale(stadium, vec3(0.5, 0.5, 0.5));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(stadium));
	sc.stadium->render();


	/***********************hierarchy*************************/
	// snitch_body-sphere
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 1.0f, 0.843f, 0.0f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.7f, 0.7f, 0.7f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.4f, 0.4f, 0.4f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 80.0f);
	// base hierarchical transformation
	sc.model_body = translate(mat4(1.0), vec3(0.0, 0.0, snitch.opt[2]));
	sc.mvs.push_back(sc.model_body);
	// base instance transformation
	sc.model_body = translate(sc.model_body, vec3(0.0, 0.0, 1.8));
	sc.model_body = scale(sc.model_body, vec3(15.0, 15.0, 15.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(sc.model_body));
	sc.snitch_body->render();
	sc.model_body = sc.mvs.back();


	// wingR
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.9f, 0.9f, 0.9f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.6f, 0.6f, 0.6f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.4f, 0.4f, 0.4f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 180.0f);
	// wingR hierarchical transformation
	mat4 model_R = mat4(1.0);
	model_R = translate(sc.model_body, vec3(-40.0, 20.0, 0.0));

	model_R = translate(model_R, vec3(42.0, 0.0, 0.0));
	model_R = rotate(model_R, (float)radians(snitch.opt[3]), vec3(0.0, 1.0, 0.0));
	model_R = translate(model_R, vec3(-42.0, 0.0, 0.0));

	// wingR instance transformation
	model_R = scale(model_R, vec3(3.0, 2.0, 2.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(model_R));
	sc.wingR->render();


	// wingL
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.9f, 0.9f, 0.9f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.7f, 0.7f, 0.7f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.4f, 0.4f, 0.4f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 180.0f);
	// wingL hierarchical transformation
	mat4 model_L = mat4(1.0);
	model_L = translate(sc.model_body, vec3(40.0, 20.0, 0.0));

	model_L = translate(model_L, vec3(-42.0, 0.0, 0.0));
	model_L = rotate(model_L, (float)radians(snitch.opt[4]), vec3(0.0, 1.0, 0.0));
	model_L = translate(model_L, vec3(42.0, 0.0, 0.0));

	// wingR instance transformation
	model_L = scale(model_L, vec3(3.0, 2.0, 2.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(model_L));
	sc.wingL->render();


	//wrand
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.15f, 0.15f, 0.15f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.219065f, 0.219065f, 0.219065f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.5f, 0.5f, 0.5f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 96.078431f);
	// wrand hierarchical transformation
	mat4 wrand;
	wrand = translate(sc.model_body, vec3(0.0, 65.0, 85.0));
	wrand = rotate(wrand, radians(80.0f), vec3(1.0, 0.0, 0.0));

	// wrand instance transformation
	wrand = scale(wrand, vec3(23.0, 23.0, 23.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(wrand));
	sc.wrand->render();

	/************************************************/
	// broom
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.365f, 0.255f, 0.176f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.7f, 0.7f, 0.7f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.4f, 0.4f, 0.4f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 180.0f);
	mat4 broom;
	float bx, bz;
	bx = 400.0 * cos(sc.broom_angle);
	bz = 400.0 * sin(sc.broom_angle);	//반시계 방향 회전5
	//broom = translate(mat4(1.0), vec3(bx, 2.0 + sc.broom_angle, bz));
	//broom = translate(mat4(1.0), vec3(10.0, 2.0, 0.0));

	//broom = rotate(broom, radians(sc.broom_angle), vec3(0.0, 1.0, 0.0));
	//broom = translate(mat4(1.0), vec3(50 - sc.broom_angle, 2.0, 0 + sc.broom_angle / 2));
	broom = translate(mat4(1.0), vec3(-50 + bx, 80.0 + sc.broom_angle * 7, bz));

	broom = scale(broom, vec3(0.5, 0.5, 0.5));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(broom));
	sc.broom->render();

	glutSwapBuffers();
}

void shutdown()
{
	glDeleteProgram(sc.rendering_program);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(sc.width, sc.height);
	glutCreateWindow("Harry Potter");
	glewInit();

	version();
	startup();
	glutDisplayFunc(render);
	glutIdleFunc(idle);

	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glutMainLoop();
	shutdown();
	return 0;
}
