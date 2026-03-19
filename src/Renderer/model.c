#include <assert.h>
#include <model.h>
#include <stdio.h>
#include <stdlib.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <material.h>
#include <glad/glad.h>
#include <memory.h>
#include <cglm/cglm.h>

#define BUFFER_OBJECT_COUNT 1
#define aPOS_ID 0
#define aNORMAL_ID 1
#define aColor_ID 1
#define aTexPos_ID 2
#define FIGURE_COMPONENT_NUM 3
#define TEX_COMPONENT_NUM 2
#define NORMAL_COMPONENTS_NUM 3

void LoadGeometry(Mesh* Mesh, const struct aiMesh* aiMesh);
void LoadMaterial(Material* mat, const struct aiMaterial* material, const char* ModelPath);
void GetTextureMode(Material* mat, const struct aiMaterial* material);

unsigned int GenAndBindVAO()
{
   unsigned int ID_VAO;
   glGenVertexArrays(BUFFER_OBJECT_COUNT, &ID_VAO);
   glBindVertexArray(ID_VAO);
   return ID_VAO;
}

unsigned int GenAndBindVBO()
{
   unsigned int ID_VBO;
   glGenBuffers(BUFFER_OBJECT_COUNT, &ID_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, ID_VBO);
   return ID_VBO;
}

unsigned int GenAndBindEBO()
{
   unsigned int ID_EBO;
   glGenBuffers(BUFFER_OBJECT_COUNT, &ID_EBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID_EBO);
   return ID_EBO;
}

void LoadModel(Model* Model, const char* Path, int IDProgram)
{
   const struct aiScene* scene = aiImportFile(Path,
      aiProcess_Triangulate |
      aiProcess_FlipUVs |
    aiProcess_GenSmoothNormals |
    aiProcess_JoinIdenticalVertices |
    aiProcess_PreTransformVertices);

   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
   {
      fprintf(stderr, "Error al cargar el modelo: %s\n", aiGetErrorString());
      assert(false);
      return;
   }

   Model->MeshCount = scene->mNumMeshes;
   Model->Meshes = (Mesh*)malloc(sizeof(Mesh) * Model->MeshCount);
   Model->ProgramID = IDProgram;
   aiMatrix4x4_to_glm(&scene->mRootNode->mTransformation, Model->ModelMatrix);
   glm_mat4_identity(Model->PosMatrix);

   for (unsigned int i = 0; i < Model->MeshCount; i++)
   {
      Mesh Mesh;
      Material mat;

      Mesh.VerticesCount = scene->mMeshes[i]->mNumVertices;
      Mesh.numIndices = scene->mMeshes[i]->mNumFaces * 3;

      const struct aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];

      LoadMaterial(&mat, material, Path);
      GetTextureMode(&mat, material);
      Mesh.Mat = mat;
      Mesh.Indices  = (unsigned int*)malloc(sizeof(unsigned int) * Mesh.numIndices);
      Mesh.Vertices = (MeshVertex*)malloc(sizeof(MeshVertex) * Mesh.VerticesCount);
      LoadGeometry(&Mesh, scene->mMeshes[i]);

      Model->Meshes[i] = Mesh;
   }
}

void aiMatrix4x4_to_glm(const struct aiMatrix4x4 *from, mat4 to)
{
   to[0][0] = from->a1; to[1][0] = from->a2; to[2][0] = from->a3; to[3][0] = from->a4;
   to[0][1] = from->b1; to[1][1] = from->b2; to[2][1] = from->b3; to[3][1] = from->b4;
   to[0][2] = from->c1; to[1][2] = from->c2; to[2][2] = from->c3; to[3][2] = from->c4;
   to[0][3] = from->d1; to[1][3] = from->d2; to[2][3] = from->d3; to[3][3] = from->d4;
}


void LoadGeometry(Mesh* Mesh, const struct aiMesh* aiMesh)
{
   int index = 0;
   for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
   {
      for (unsigned int j = 0; j <  aiMesh->mFaces[i].mNumIndices; j++)
      {
         Mesh->Indices[index++] = aiMesh->mFaces[i].mIndices[j];
         assert(index <=Mesh->numIndices);
      }
   }

   for (unsigned int j = 0; j < aiMesh->mNumVertices; j++)
   {
      MeshVertex vertex;

      vertex.Vertices[0] = aiMesh->mVertices[j].x;
      vertex.Vertices[1] = aiMesh->mVertices[j].y;
      vertex.Vertices[2] = aiMesh->mVertices[j].z;

      vertex.Normals[0]  = aiMesh->mNormals[j].x;
      vertex.Normals[1]  = aiMesh->mNormals[j].y;
      vertex.Normals[2]  = aiMesh->mNormals[j].z;

      vertex.TexCoord[0] = aiMesh->mTextureCoords[0][j].x;
      vertex.TexCoord[1] = aiMesh->mTextureCoords[0][j].y;

      Mesh->Vertices[j] = vertex;
   }
}

//TODO: Support more texture types
void LoadMaterial(Material* mat, const struct aiMaterial* material, const char* ModelPath)
{
   float Alpha = 0.5f;
   if (aiGetMaterialFloatArray(material, AI_MATKEY_GLTF_ALPHACUTOFF, &Alpha, nullptr) == AI_SUCCESS)
      mat->AlphaCutoff = Alpha;

      mat->HasBlend = (aiGetMaterialTextureCount(material, aiTextureType_OPACITY) > 0) ? true : false;

      struct aiColor4D diffuse;
   if (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS)
   {
      mat->DiffCol[0] = diffuse.r;
      mat->DiffCol[1] = diffuse.g;
      mat->DiffCol[2] = diffuse.b;
   }
   else
   {
      mat->DiffCol[0] = 1.0f;
      mat->DiffCol[1] = 1.0f;
      mat->DiffCol[2] = 1.0f;
   }

   struct aiColor4D specular;

   if (aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular) == AI_SUCCESS)
   {
      mat->SpecCol[0] = specular.r;
      mat->SpecCol[1] = specular.g;
      mat->SpecCol[2] = specular.b;
   }
   else
   {
      mat->SpecCol[0] = 1.0f;
      mat->SpecCol[1] = 1.0f;
      mat->SpecCol[2] = 1.0f;
   }

   aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &mat->Shininess , NULL);

   unsigned int TexQuant = 0;

   for (enum aiTextureType TexType = aiTextureType_NONE; TexType < aiTextureType_UNKNOWN; TexType++)
   {
      TexQuant += aiGetMaterialTextureCount(material, TexType);

   }

   if (TexQuant < 1)
   {
      mat->TexCount = 0;
      mat->Textures = nullptr;
      return;
   }

   mat->Textures = (Texture*)malloc(TexQuant * sizeof(Texture));
   mat->TexCount = TexQuant;

   int index = 0;
   for (enum aiTextureType type = aiTextureType_NONE; type < aiTextureType_UNKNOWN; type++)
   {
      unsigned int count = aiGetMaterialTextureCount(material, type);

      for (unsigned int i = 0; i < count; i++ )
      {
         Texture tex = {0};
         struct aiString path;
         aiGetMaterialTexture(material, type, i, &path, nullptr, NULL, NULL, NULL, NULL, NULL);

         char FinalPath[256] = {0};
         GetTexturePath(ModelPath, path.data , FinalPath);
         LoadTexture(&tex, FinalPath);
         strncpy(tex.path, FinalPath, 256);

         tex.Format = (tex.nrChannels > 3) ? GL_RGBA : GL_RGB;
         tex.InternType = (tex.nrChannels > 3) ? GL_RGBA8 : GL_RGB8;

         switch (type)
         {
            case aiTextureType_DIFFUSE:
               tex.CHANGE_THIS_SHIT = ALBEDO;
               break;
            case aiTextureType_SPECULAR:
               tex.CHANGE_THIS_SHIT = SPECULAR;
               break;
            default: tex.CHANGE_THIS_SHIT = TEXUNKOWN;
         }

         mat->Textures[index++] = tex;
      }
   }
}


void GetTextureMode(Material* mat, const struct aiMaterial* material)
{
   enum aiTextureMapping mappingMode;

   aiGetMaterialIntegerArray(material, AI_MATKEY_MAPPING(aiTextureType_DIFFUSE, 0), (int*)&mappingMode, nullptr);

      switch (mappingMode)
      {
         case aiTextureMapMode_Wrap:
            mat->TexMode = GL_REPEAT;
            break;
         case  aiTextureMapMode_Clamp:
            mat->TexMode = GL_CLAMP_TO_EDGE;
            break;
         case aiTextureMapMode_Mirror:
            mat->TexMode = GL_MIRRORED_REPEAT;
            break;
         case aiTextureMapMode_Decal:
            mat->TexMode = GL_CLAMP_TO_EDGE;
            break;
         default:
            mat->TexMode = GL_REPEAT;
      }

}

void SendModel(Model* Model)
{

   CreateSample(Model->ProgramID, "TexDiff", 0);
   CreateSample(Model->ProgramID, "TexSpecular", 1);

   for(unsigned int i = 0; i < Model->MeshCount; i++)
   {
      Mesh* mesh = &Model->Meshes[i];
      for (unsigned int texindx = 0; texindx < mesh->Mat.TexCount; texindx++)
      {
         Texture* tex =  &mesh->Mat.Textures[texindx];
         switch (tex->CHANGE_THIS_SHIT) //TODO: Support more texture types
         {
            case TEXUNKOWN: tex->SamplerID = -1; break;

            case ALBEDO:
               tex->SamplerID =  0;
               break;
            case SPECULAR:
               tex->SamplerID =  1;
               break;
            case NONE:
               break;
         }

         CreateTexture(tex, GL_TEXTURE_2D, tex->SamplerID, 1);
         SetTextureParams(tex, Model->Meshes[i].Mat.TexMode);
         GenerateTexture(tex);
      }

      const unsigned int VAO = GenAndBindVAO();
      GenAndBindVBO();
      GenAndBindEBO();

      glBufferData(GL_ARRAY_BUFFER,(long)(Model->Meshes[i].VerticesCount * sizeof(MeshVertex)), Model->Meshes[i].Vertices, GL_STATIC_DRAW);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,(long)(Model->Meshes[i].numIndices * sizeof(unsigned int)), Model->Meshes[i].Indices, GL_STATIC_DRAW);

      glVertexAttribPointer(aPOS_ID, FIGURE_COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(MeshVertex), nullptr);
      glEnableVertexAttribArray(aPOS_ID);
      glVertexAttribPointer(aNORMAL_ID, NORMAL_COMPONENTS_NUM, GL_FLOAT, GL_FALSE,  sizeof(MeshVertex), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(aNORMAL_ID);
      glVertexAttribPointer(aTexPos_ID, TEX_COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(MeshVertex), (void*)(6 * sizeof(float)));
      glEnableVertexAttribArray(aTexPos_ID);

      glBindVertexArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      Model->Meshes[i].VAO = VAO;
   }
}

void MoveModel(Model* Model, vec3 pos)
{
   glm_translate(Model->PosMatrix, pos);
}

void ScaleModel(Model* Model, vec3 scale)
{
   glm_scale(Model->ModelMatrix, scale);
}

void RenderModel(Model* Model, Camera* camera, ModelUniform* uniforms, mat4 ProjectionMatrix)
{
   for (unsigned int i = 0; i < Model->MeshCount; i++)
   {
      Material* mat = &Model->Meshes[i].Mat;
      for (unsigned int texindx = 0; texindx < mat->TexCount; texindx++) {
         const Texture* tex = &mat->Textures[texindx];

         if (tex->SamplerID < 0)
            continue;

         glActiveTexture(GL_TEXTURE0 + tex->SamplerID);
         glBindTexture(GL_TEXTURE_2D, tex->ID);
      }

      if (mat->HasBlend)
      {
         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      }
      else
         glDisable(GL_BLEND);

      //glUniform1f(Timer, Time);
      glUniform3f(uniforms->CameraPos, camera->CameraPos[0], camera->CameraPos[1], camera->CameraPos[2]);
      glUniform3f(uniforms->ObjectColor, mat->DiffCol[0], mat->DiffCol[1], mat->DiffCol[2]);
      glUniform3f(uniforms->Specular, mat->SpecCol[0], mat->SpecCol[1], mat->SpecCol[2]);
      glUniform1f(uniforms->Shininess, mat->Shininess);
      glUniformMatrix4fv(uniforms->ModelMatrix, 1, GL_FALSE,(const float*)Model->ModelMatrix);
      glUniformMatrix4fv(uniforms->ViewMatrix, 1, GL_FALSE,(const float*)camera->ViewMatrix);
      glUniformMatrix4fv(uniforms->ProjectionMatrix, 1, GL_FALSE,(const float*)ProjectionMatrix);
      glUniformMatrix4fv(uniforms->PositionMatrix, 1, GL_FALSE,(const float*)Model->PosMatrix);
      glBindVertexArray(Model->Meshes[i].VAO);
      glDrawElements(GL_TRIANGLES, (int)Model->Meshes[i].numIndices, GL_UNSIGNED_INT, nullptr);
   }
}

void CleanModel(const Model* Model)
{
   for (unsigned int i = 0; i < Model->MeshCount; i++)
   {
      free(Model->Meshes[i].Vertices);
      free(Model->Meshes[i].Indices);
      Model->Meshes[i].Vertices = nullptr;
      Model->Meshes[i].Indices = nullptr;
   }
}
