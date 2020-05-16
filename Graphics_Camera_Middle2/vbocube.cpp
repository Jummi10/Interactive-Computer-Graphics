//11.01
#include "GL/vbocube.h"
#include "GL/cg.h"

#include <cstdio>

VBOCube::VBOCube()
{
	float side = 1.0f;
	float side2 = side / 2.0f;

	float v[24 * 4] = {
		// Front, +z 방향으로 법선벡터 지정
		-side2, -side2, side2, 1.0,
		side2, -side2, side2,  1.0,
		side2,  side2, side2,  1.0,
		-side2,  side2, side2,  1.0,
		// Right, +x
		side2, -side2, side2,  1.0,
		side2, -side2, -side2,  1.0,
		side2,  side2, -side2,  1.0,
		side2,  side2, side2,  1.0,
		// Back, -z
		-side2, -side2, -side2,  1.0,
		-side2,  side2, -side2,  1.0,
		side2,  side2, -side2,  1.0,
		side2, -side2, -side2,  1.0,
		// Left, -x
		-side2, -side2, side2,  1.0,
		-side2,  side2, side2,  1.0,
		-side2,  side2, -side2,  1.0,
		-side2, -side2, -side2,  1.0,
		// Bottom, -y
		-side2, -side2, side2,  1.0,
		-side2, -side2, -side2,  1.0,
		side2, -side2, -side2,  1.0,
		side2, -side2, side2,  1.0,
		// Top, +y
		-side2,  side2, side2,  1.0,
		side2,  side2, side2,  1.0,
		side2,  side2, -side2,  1.0,
		-side2,  side2, -side2,  1.0
	};

	float n[24 * 3] = {
		// Front
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		// Right
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		// Back
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		// Left
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		// Bottom
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		// Top
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	float tex[24 * 2] = {
		// Front
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Right
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Back
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Left
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Bottom
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Top
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	GLuint el[] = {//그림그리는 순서 element 순서
		0,1,2,0,2,3,
		4,5,6,4,6,7,
		8,9,10,8,10,11,
		12,13,14,12,14,15,
		16,17,18,16,18,19,
		20,21,22,20,22,23
	};

	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//큐브에 버퍼 4개 생성
	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 24 * 4 * sizeof(float), v, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), n, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, 24 * 2 * sizeof(float), tex, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));//location 지정해주는거
	glEnableVertexAttribArray(2);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), el, GL_STATIC_DRAW);

	//원래 어플리케이션 어레이를 돌려버린다. 큐브값으로
	glBindVertexArray(0);
}

/*
1. cube.vaoHandle의 VA
-Buffer
-

2.plane.vaoHandle->plane에도 vaoHandle이 따로 있어서 그 안의 vertex array들은 ~~가 있다.
*/

void VBOCube::render() { //private 변수 -vbocube.h
	glBindVertexArray(vaoHandle);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
}//array 에있는걸 그대로 그리는 것이 아니라 element에 있는 순서대로 그린다.
