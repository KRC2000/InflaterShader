#include "util.h"

unsigned int MakeShaderProgram(const char* vertShaderPath, const char* fragShaderPath) {
	char* vertShaderCode = readFileContents(vertShaderPath);
	char* fragShaderCode = readFileContents(fragShaderPath);

	unsigned int vertShader, fragShader;
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertShader, 1, &vertShaderCode, NULL);
	glShaderSource(fragShader, 1, &fragShaderCode, NULL);
	glCompileShader(vertShader);
	glCompileShader(fragShader);

	// Handle failed shader compilations
	int success;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
		printf("Vertex shader compilation failed: %s", infoLog);
		return -1;
	}
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
		printf("Fragment shader compilation failed: %s", infoLog);
		return -1;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	printf("Shader program created.\n");

	return shaderProgram;
}

static void printChildren(struct aiNode* node, char* path) {
	printf("NodeName: %s; ChildrenNodes: %i\n", node->mName.data, node->mNumChildren);
};

Model* LoadModel(const char* path) {
	Model* m = malloc(sizeof(Model));
	glm_mat4_identity(m->transform);

	struct aiScene* scene = LoadScene(path);

	m->dataCount = scene->mMeshes[0]->mNumVertices * 3 * 2; // twice for normals
	m->data = malloc(m->dataCount * sizeof(float));

	m->elementsCount = scene->mMeshes[0]->mNumFaces * 3;
	m->elements = malloc(m->elementsCount * sizeof(unsigned int));

	unsigned int idx = 0;
	for (size_t i = 0; i < scene->mMeshes[0]->mNumVertices; i++) {
		m->data[idx] = scene->mMeshes[0]->mVertices[i].x;
		m->data[idx+1] = scene->mMeshes[0]->mVertices[i].y;
		m->data[idx+2] = scene->mMeshes[0]->mVertices[i].z;

		m->data[idx+3] = scene->mMeshes[0]->mNormals[i].x;
		m->data[idx+4] = scene->mMeshes[0]->mNormals[i].y;
		m->data[idx+5] = scene->mMeshes[0]->mNormals[i].z;

		idx += 6;
	}

	idx = 0;
	for (size_t i = 0; i < scene->mMeshes[0]->mNumFaces; i++) {
		m->elements[idx] = scene->mMeshes[0]->mFaces[i].mIndices[0];
		m->elements[idx+1] = scene->mMeshes[0]->mFaces[i].mIndices[1];
		m->elements[idx+2] = scene->mMeshes[0]->mFaces[i].mIndices[2];
		idx += 3;
	}

	glGenVertexArrays(1, &m->vao);
	glBindVertexArray(m->vao);

	glGenBuffers(1, &m->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->dataCount, m->data, GL_STATIC_DRAW);

	glGenBuffers(1, &m->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m->elementsCount, m->elements, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// check OpenGL error
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("OpenGl error at the end of LoadModel: %s\n", err);
	}

	return m;
}

struct aiScene* LoadScene(const char* path) {

	const struct aiScene* scene =
		aiImportFile(path, aiProcess_Triangulate);

	// If the import failed, report it
	if (!scene) {
		// DoTheErrorLogging(aiGetErrorString());
		printf("Could not load the model: %s.\n", path);
		return NULL;
	}

	printf("Scene loaded: %s\n", path);

	return scene;
}

char * readFileContents(char const *file_name)
{
	FILE *file = fopen(file_name, "r");
	if (!file)
	{
		printf("Could not open a file %s.\n", file_name);
		perror("");
		system("pwd");
	}

	fseek(file, 0, SEEK_END); // go to the end of the file
	size_t size = ftell(file); // get current pos==size
	rewind(file); // go to start of the file
	char *str = malloc(size+1);

	if (!str)
		printf("Failed to allocate memory.");

	fread(str, 1, size, file);
	str[size] = '\0';

	fclose(file);

	return str;
}