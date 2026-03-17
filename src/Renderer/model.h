#ifndef GPU_MODEL_H
#define GPU_MODEL_H

#include <mesh.h>
#include <assimp/GltfMaterial.h>
#include <Camera/camera.h>
#include "modeluniform.h"
#include "cglm/simd/sse2/mat4.h"

typedef struct
{
    Mesh* Meshes;
    unsigned int MeshCount;
    int ProgramID;
    mat4 ModelMatrix;
    mat4 PosMatrix;
}Model;

unsigned int GenAndBindVAO();
unsigned int GenAndBindVBO();
unsigned int GenAndBindEBO();
void LoadModel(Model* Model, const char* Path, int IDProgram);
void aiMatrix4x4_to_glm(const struct aiMatrix4x4 *from, mat4 to);
void SendModel(Model* Model);
void MoveModel(Model* Model, vec3 pos);
void ScaleModel(Model* Model, vec3 scale);
void CleanModel(const Model* Model);
void RenderModel(Model* Model, Camera* camera, ModelUniform* uniforms, mat4 ProjectionMatrix);



#endif //GPU_MODEL_H