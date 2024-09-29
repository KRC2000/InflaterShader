#ifndef MODEL_H
#define MODEL_H

#include <cglm/cglm.h>

typedef struct Model {
	mat4 transform;
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	float* data;
	unsigned int dataCount;
	unsigned int* elements;
	unsigned int elementsCount;
} Model;

#endif