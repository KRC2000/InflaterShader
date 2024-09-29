#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <epoxy/gl.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "model.h"

unsigned int MakeShaderProgram(const char* vertShaderPath, const char* fragShaderPath);
struct Model* LoadModel(const char* path);
struct aiScene* LoadScene(const char* path);
char* readFileContents(char const* file_name);

#endif