#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>
#include <cglm/cglm.h>
#include <src/Renderer/texture.h>
#include <string.h>
#include <src/Camera/camera.h>
#include <src/App/App.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/GltfMaterial.h>

#include <Shader.h>

//FANCY SHADER

#define aPOS_ID 0
#define aNORMAL_ID 1
#define aColor_ID 1
#define aTexPos_ID 2

#define SDL_MAIN_HANDLED
#define BUFFER_OBJECT_COUNT 1
#define STRING_COUNT 1
#define FIGURE_COMPONENT_NUM 3
#define TEX_COMPONENT_NUM 2
#define NORMAL_COMPONENTS_NUM 3
#define NOEMNTS(x) (int)(sizeof(x) / sizeof((x)[0]))

static bool Running = false;

int LinkingSucces = false;

typedef struct
{
   float Vertices[3];
   float Normals[3];
   float TexCoord[2];
}ModelVertex;

typedef struct
{
   unsigned int VerticesCount;
   unsigned int* Indices;
   unsigned int numIndices;
   unsigned int VAO;
   ModelVertex* Vertices;
   char TextureDiffuse[256];
   char TextureSpecular[256];
   unsigned int TexDiffID;
   unsigned int TexSpecularID;
   bool HasBlend;
   bool HasSpecularMap;
   float DiffCol[3];
   float SpecCol[3];
   float Shininess;
   float AlphaCutoff;
   unsigned int TexMode;
}Mesh;

typedef struct
{
   Mesh* MeshList;
   int MeshCount;
   struct aiNode* RootNode;
   struct aiNode* Children;
   struct aiScene* Scene;
}Model;

typedef struct
{
   float x, y, z;
  // float r, g, b;
   //float t, s;
   float nx, ny, nz;
} Vertex;

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


unsigned int PrepareRectangle(Vertex Vertices[], int VerticesCount, unsigned int Indices[], int IndicesCount)
{
   unsigned int ID_VAO = GenAndBindVAO();
   //int ID_VBO;
   //int ID_VBO =
   GenAndBindVBO();
   //unsigned int EBO = GenAndBindEBO();
   GenAndBindEBO();
   //Create buffer to the GPU
   glBufferData(GL_ARRAY_BUFFER,(long)(VerticesCount * sizeof(Vertex)), Vertices, GL_STATIC_DRAW);
   //Bind EBO to the GL_ARRAY_BUFFER
   //glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesCount, Indices, GL_STATIC_DRAW);
   glVertexAttribPointer(aPOS_ID, FIGURE_COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(Vertex), nullptr);
   glEnableVertexAttribArray(aPOS_ID);
   //glVertexAttribPointer(aTexPos_ID, TEX_COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(3 * sizeof(float)));
   //glEnableVertexAttribArray(aTexPos_ID);
   glVertexAttribPointer(aNORMAL_ID, NORMAL_COMPONENTS_NUM, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(aNORMAL_ID);
   //glVertexAttribPointer(aColor_ID, FIGURE_FIGURE_COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(3 * sizeof(float)));
   //glEnableVertexAttribArray(aColor_ID);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   return ID_VAO;
}

void GetTexturePath(const char* ModelPath, const char* TexturePath, char* FinalPath)
{
   char* name = strrchr(TexturePath, '\\');
   if (!name)
   {
      name = strrchr(TexturePath, '/');
   }

   name++;


   memcpy(FinalPath, ModelPath, strlen(ModelPath));

   char* ptr = strrchr(FinalPath, '\\');
   if (!ptr)
   {
      ptr = strrchr(FinalPath, '/');
   }
   ptr++;
   for (; *name != '\0'; name++, ptr++)
   {
      *ptr = *name;
   }
   *ptr = '\0';
}

void PrepareFour(Mesh Model[], const unsigned int MeshCount, const char* ModelPath, unsigned int ID_CubeProgram)
{
   CreateSample(ID_CubeProgram, "Texture1", 0);
   CreateSample(ID_CubeProgram, "Texture2", 1);

   for (unsigned int i = 0; i < MeshCount; i++)
   {
      unsigned int VAO = GenAndBindVAO();
      int VBO = GenAndBindVBO();
      int EBO = GenAndBindEBO();

      char FinalPath1[128];
      GetTexturePath(ModelPath, Model[i].TextureDiffuse , FinalPath1);

      Texture ContainerTexture;
      LoadTexture(&ContainerTexture, FinalPath1);
      ContainerTexture.Format = (ContainerTexture.nrChannels > 3) ? GL_RGBA : GL_RGB;
      ContainerTexture.InternType = (ContainerTexture.nrChannels > 3) ? GL_RGBA8 : GL_RGB8;
      CreateTexture(&ContainerTexture, GL_TEXTURE_2D, 0,  1, ContainerTexture.Format);
      SetTextureParams(&ContainerTexture, Model[i].TexMode);
      GenerateTexture(&ContainerTexture);
      Model[i].TexDiffID = ContainerTexture.ID_Texture;

      if (Model[i].HasSpecularMap)
      {
         char FinalPath2[128];
         GetTexturePath(ModelPath, Model[i].TextureSpecular , FinalPath2);

         Texture SecondContainer;
         LoadTexture(&SecondContainer, FinalPath2);
         SecondContainer.Format = (SecondContainer.nrChannels > 3) ? GL_RGBA : GL_RGB;
         SecondContainer.InternType = (SecondContainer.nrChannels > 3) ? GL_SRGB8_ALPHA8 : GL_RGB;
         CreateTexture(&SecondContainer, GL_TEXTURE_2D, 1,  1, SecondContainer.Format);
         SetTextureParams(&SecondContainer, Model[i].TexMode);
         GenerateTexture(&SecondContainer);
         Model[i].TexSpecularID = SecondContainer.ID_Texture;
      }

      glBufferData(GL_ARRAY_BUFFER,(long)(Model[i].VerticesCount * sizeof(ModelVertex)), Model[i].Vertices, GL_STATIC_DRAW);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,(long)(Model[i].numIndices * sizeof(unsigned int)), Model[i].Indices, GL_STATIC_DRAW);

      glVertexAttribPointer(aPOS_ID, FIGURE_COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(ModelVertex), nullptr);
      glEnableVertexAttribArray(aPOS_ID);

      glVertexAttribPointer(aNORMAL_ID, NORMAL_COMPONENTS_NUM, GL_FLOAT, GL_FALSE,  sizeof(ModelVertex), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(aNORMAL_ID);

      glVertexAttribPointer(aTexPos_ID, TEX_COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(ModelVertex), (void*)(6 * sizeof(float)));
      glEnableVertexAttribArray(aTexPos_ID);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


      Model[i].VAO = VAO;
   }
}

void aiMatrix4x4_to_glm(const struct aiMatrix4x4 *from, mat4 to)
{
   to[0][0] = from->a1; to[1][0] = from->a2; to[2][0] = from->a3; to[3][0] = from->a4;
   to[0][1] = from->b1; to[1][1] = from->b2; to[2][1] = from->b3; to[3][1] = from->b4;
   to[0][2] = from->c1; to[1][2] = from->c2; to[2][2] = from->c3; to[3][2] = from->c4;
   to[0][3] = from->d1; to[1][3] = from->d2; to[2][3] = from->d3; to[3][3] = from->d4;
}

int main()
{
   const char* HONOKA = "/home/ladyfuriae/OpenGL/game/Models/Honoka/Honoka/Honoka.FBX";
   const char* ACCORD = "/home/ladyfuriae/OpenGL/game/Models/Accord/Accord.obj";
   const char* HONOKASWIMSUIT = "/home/ladyfuriae/OpenGL/game/Models/HonokaSwimsuit/Swimsuit/HonokaSwimSuit.FBX";
   const char* HONOKAFACHAN = "/home/ladyfuriae/OpenGL/game/Models/FACHANHONOKA/FACHAN/HFACHAN.FBX";
   const struct aiScene* scene = aiImportFile(HONOKAFACHAN,
      aiProcess_Triangulate |
      aiProcess_FlipUVs |
    aiProcess_GenSmoothNormals |
    aiProcess_JoinIdenticalVertices |
    aiProcess_PreTransformVertices);

   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
   {
      fprintf(stderr, "Error al cargar el modelo: %s\n", aiGetErrorString());
      assert(false);
      return -1;
   }

   Mesh FourMeshes[scene->mNumMeshes];

   for (unsigned int i = 0; i < scene->mNumMeshes; i++)
   {
      Mesh FourMesh = {0};
      FourMesh.DiffCol[0] = 1.0f;
      FourMesh.DiffCol[1] = 1.0f;
      FourMesh.DiffCol[2] = 1.0f;
      FourMesh.VerticesCount = scene->mMeshes[i]->mNumVertices;
      FourMesh.numIndices = scene->mMeshes[i]->mNumFaces * 3;
      FourMesh.Vertices = (ModelVertex*)malloc(sizeof(ModelVertex) * FourMesh.VerticesCount);
      FourMesh.Indices = (unsigned int*)malloc(sizeof(unsigned int) * FourMesh.numIndices);

      const struct aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
      struct aiString path;
      enum aiTextureMapping mappingMode;
      float Alpha = 0.5f;
      if (aiGetMaterialFloatArray(material, AI_MATKEY_GLTF_ALPHACUTOFF, &Alpha, nullptr) == AI_SUCCESS)
      {
         FourMesh.AlphaCutoff = Alpha;
      }

      if (aiGetMaterialIntegerArray(material, AI_MATKEY_MAPPING(aiTextureType_DIFFUSE, 0), (int*)&mappingMode, nullptr) == AI_SUCCESS)
      {
         switch (mappingMode)
         {
            case aiTextureMapMode_Wrap:
               FourMesh.TexMode = GL_REPEAT;
               break;
            case  aiTextureMapMode_Clamp:
               FourMesh.TexMode = GL_CLAMP_TO_EDGE;
               break;
            case aiTextureMapMode_Mirror:
               FourMesh.TexMode = GL_MIRRORED_REPEAT;
               break;
            case aiTextureMapMode_Decal:
               FourMesh.TexMode = GL_CLAMP_TO_EDGE;
         }
      }

      FourMesh.HasSpecularMap = false;
      FourMesh.HasBlend = (aiGetMaterialTextureCount(material, aiTextureType_OPACITY) > 0) ? true : false;

      if (aiGetMaterialTexture(material, aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
         strncpy(FourMesh.TextureDiffuse, path.data, sizeof(FourMesh.TextureDiffuse) - 1);
      }

      // Cargar Shininess/Specular (Tipo 8)
      if (aiGetMaterialTexture(material, 8, 0, &path, NULL, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
      {
         strncpy(FourMesh.TextureSpecular, path.data, sizeof(FourMesh.TextureSpecular) - 1);
         FourMesh.HasSpecularMap = true;
      }


      int index = 0;
      for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
      {
         for (unsigned int k = 0; k < scene->mMeshes[i]->mFaces[j].mNumIndices; k++)
         {
            FourMesh.Indices[index++] = scene->mMeshes[i]->mFaces[j].mIndices[k];
         }
      }

      struct aiColor4D diffuse;
      if (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS) {
         printf("Malla %d - Color difuso: %f %f %f %f\n", i, diffuse.r, diffuse.g, diffuse.b, diffuse.a);
         FourMesh.DiffCol[0] = diffuse.r;
         FourMesh.DiffCol[1] = diffuse.g;
         FourMesh.DiffCol[2] = diffuse.b;
      } else {
         printf("Malla %d - Sin color difuso específico\n", i);
      }
      struct aiColor4D specular;

      if (aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular) == AI_SUCCESS)
      {
         printf("Malla %d - Color specular: %f %f %f %f\n", i, specular.r, specular.g, specular.b, specular.a);
         FourMesh.SpecCol[0] = specular.r;
         FourMesh.SpecCol[1] = specular.g;
         FourMesh.SpecCol[2] = specular.b;
      }
      float Shininess;
      if(aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &Shininess, NULL) == AI_SUCCESS)
         FourMesh.Shininess = Shininess;

      for (unsigned int j = 0; j < FourMesh.VerticesCount; j++)
      {
         ModelVertex vertex;

         vertex.Vertices[0] = scene->mMeshes[i]->mVertices[j].x;
         vertex.Vertices[1] = scene->mMeshes[i]->mVertices[j].y;
         vertex.Vertices[2] = scene->mMeshes[i]->mVertices[j].z;

         vertex.Normals[0] = scene->mMeshes[i]->mNormals[j].x;
         vertex.Normals[1] = scene->mMeshes[i]->mNormals[j].y;
         vertex.Normals[2] = scene->mMeshes[i]->mNormals[j].z;

         vertex.TexCoord[0] = scene->mMeshes[i]->mTextureCoords[0][j].x;
         vertex.TexCoord[1] = scene->mMeshes[i]->mTextureCoords[0][j].y;

         FourMesh.Vertices[j] = vertex;
      }

      FourMeshes[i] = FourMesh;
   }




   App app;
   if(!AppInit(&app))
      return -1;

   BindOpenGLToSDL();
   if (!BindGLContext(&app))
      return -1;

   SDL_Log("Init");

   Vertex Vertices1[] =
   {
         {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
         { 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
         { 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
         { 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
         {-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
         {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f},

         {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
         { 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
         { 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
         { 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
         {-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
         {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f},

         {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f},
         {-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
         {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
         {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
         {-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f},
         {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f},

         { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f},
         { 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
         { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
         { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
         { 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f},
         { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f},

         {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},
         { 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},
         { 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
         { 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
         {-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
         {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},

         {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f},
         { 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f},
         { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
         { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
         {-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
         {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f}
   };

   unsigned int Indices1[] =
   {
      0,1,2,
      0,2,3
   };

   const int ID_CubeProgram  = SetCubeShader();
   const int ID_LightProgram = SetLightShader();

   if (!ID_CubeProgram)
      return -1;

   if(!ID_LightProgram)
      return -1;



//
   //Texture HappyTexture;
   //LoadTexture(&HappyTexture, "../game/textures/awesomeface.png");
   //CreateTexture(&HappyTexture, GL_TEXTURE_2D,1, 1, GL_RGBA);
   //SetTextureParams(&HappyTexture);
   //GenerateTexture(&HappyTexture);

   glUseProgram(ID_CubeProgram);
   //const unsigned int VAO1 = PrepareRectangle(Vertices1, NOEMNTS(Vertices1), Indices1, sizeof(Indices1));
   const unsigned int VAOLight = PrepareRectangle(Vertices1, NOEMNTS(Vertices1), Indices1, sizeof(Indices1));
   PrepareFour(FourMeshes, NOEMNTS(FourMeshes),HONOKAFACHAN ,ID_CubeProgram);

   Model FourModel;
   FourModel.MeshList = FourMeshes;
   FourModel.MeshCount = NOEMNTS(FourMeshes);
   struct aiNode RootNode = *scene->mRootNode;
   FourModel.RootNode = &RootNode;


   Running = true;
   const int Timer = glGetUniformLocation(ID_CubeProgram, "Timer");

   const int MatrixUniform = glGetUniformLocation(ID_CubeProgram, "Trans");
   const int UModel = glGetUniformLocation(ID_CubeProgram, "Model");
   const int UView = glGetUniformLocation(ID_CubeProgram, "View");
   const int UProjection = glGetUniformLocation(ID_CubeProgram, "Projection");


   const int ULighTrans = glGetUniformLocation(ID_LightProgram, "Trans");
   const int UlightModel = glGetUniformLocation(ID_LightProgram, "Model");
   const int ULightView = glGetUniformLocation(ID_LightProgram, "View");
   const int ULightProjection = glGetUniformLocation(ID_LightProgram, "Projection");

   const int ULight = glGetUniformLocation(ID_CubeProgram, "LightColor");
   const int UobjColor = glGetUniformLocation(ID_CubeProgram, "ObjColor");
   const int USpecular = glGetUniformLocation(ID_CubeProgram, "Specular");
   const int UShininess = glGetUniformLocation(ID_CubeProgram, "Shininess");

   glUniform3f(ULight, 1.0f, 1.0f, 1.0f);

   mat4 Trans = GLM_MAT4_IDENTITY_INIT;

   glEnable(GL_DEPTH_TEST);

   Camera camera;
   PrepareCamera(&camera);
   SetViewCameraMatrix(&camera);
   float deltaTime = 0.0f;
   float lastFrame = 0.0f;
   glm_mat4_identity(camera.ViewMatrix);
   mat4 ProjectionMatrix = GLM_MAT4_IDENTITY_INIT;

   glm_perspective(glm_rad(90.0f), 1920.0f/1080.0f , 0.1f, 100.0f, ProjectionMatrix);
   bool isEnabled = false;
   SDL_SetWindowRelativeMouseMode(app.Window, isEnabled);
   float mouseX, mouseY;
   const bool* State = SDL_GetKeyboardState(nullptr);

   vec3 lightPos = {-1.2f, 1.0f, -2.0f };
   glUseProgram(ID_CubeProgram);
   int lightpos_ID = glGetUniformLocation(ID_CubeProgram, "LightPosition");
   int lightcolor_ID = glGetUniformLocation(ID_CubeProgram, "LightColor");
   int ViewPos_ID = glGetUniformLocation(ID_CubeProgram, "ViewPos");
   glUniform3f(lightpos_ID, lightPos[0], lightPos[1], lightPos[2]);
   glUniform3f(lightcolor_ID, 1.0f, 1.0f, 1.0f);
   mat4 LightTransMatrix = GLM_MAT4_IDENTITY_INIT;
   glm_translate(LightTransMatrix, lightPos);

   vec3 FourPos = {0.0f, 0.0f, -0.0f};

   mat4 FOURFINALMATRIXPOS;
   aiMatrix4x4_to_glm(&FourModel.RootNode->mTransformation, FOURFINALMATRIXPOS);
   glm_translate(FOURFINALMATRIXPOS, FourPos);

   mat4 FourModelMatrix = GLM_MAT4_IDENTITY_INIT;
   vec3 FourScale = {0.5f, 0.5f, 0.5f};
   glm_scale(FourModelMatrix, FourScale);

   while(Running) {
      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
         if(event.type == SDL_EVENT_QUIT)
         {
            Running = false;
         }

         else if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
         {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
               isEnabled = !isEnabled;
               SDL_SetWindowRelativeMouseMode(app.Window, isEnabled);
            }
         }
      }
      ManageKeyboard(State, &camera);
      SDL_GetRelativeMouseState(&mouseX, &mouseY);

      if(isEnabled)
      {
         CalcCamMov(&camera, &mouseX, &mouseY);

         if (camera.Pitch > 89.0f) camera.Pitch = 89.0f;
         if (camera.Pitch < -89.0f) camera.Pitch = -89.0f;

         UpdateCamera(&camera);
      }
      SetViewCameraMatrix(&camera);

      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      const float Time = SDL_GetTicks() / 1000.0f;
      deltaTime = Time - lastFrame;
      lastFrame = Time;


      glm_rotate(FOURFINALMATRIXPOS, glm_rad(0.2f), (vec3){0.0f, 1.0f, 0.0f});

      mat4 ModelMatrix = GLM_MAT4_IDENTITY_INIT;

      //glUseProgram(ID_CubeProgram);
//
      //glUniform3f(ViewPos_ID, camera.CameraPos[0], camera.CameraPos[1], camera.CameraPos[2]);
//
      //glUniform1f(Timer, Time);
      //glUniformMatrix4fv(UModel, 1, GL_FALSE,(const float*)ModelMatrix);
      //glUniformMatrix4fv(UView, 1, GL_FALSE,(const float*)camera.ViewMatrix);
      //glUniformMatrix4fv(UProjection, 1, GL_FALSE,(const float*)ProjectionMatrix);
      //glUniformMatrix4fv(MatrixUniform, 1, GL_FALSE,(const float*)Trans);
//
      //glBindVertexArray(VAO1);
      //glDrawArrays(GL_TRIANGLES, 0, 36);

      glUseProgram(ID_LightProgram);
      glUniform1f(Timer, Time);
      glUniformMatrix4fv(UlightModel, 1, GL_FALSE,(const float*)ModelMatrix);
      glUniformMatrix4fv(ULightView, 1, GL_FALSE,(const float*)camera.ViewMatrix);
      glUniformMatrix4fv(ULightProjection, 1, GL_FALSE,(const float*)ProjectionMatrix);
      glUniformMatrix4fv(ULighTrans, 1, GL_FALSE,(const float*)LightTransMatrix);
      glBindVertexArray(VAOLight);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      mat4 Accumulated = GLM_MAT4_IDENTITY_INIT;

      for (int i = 0; i < FourModel.MeshCount; i++)
      {

         glUseProgram(ID_CubeProgram);

         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, FourModel.MeshList[i].TexDiffID);
         glUniform1i(glGetUniformLocation(ID_CubeProgram, "TexDiff"), 0);

         if (FourModel.MeshList[i].HasSpecularMap)
         {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, FourModel.MeshList[i].TexSpecularID);
            glUniform1i(glGetUniformLocation(ID_CubeProgram, "TexSpecular"), 1);
         }

         if (FourModel.MeshList[i].HasBlend)
         {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         }
         else
         {
            glDisable(GL_BLEND);
         }

         glUniform1f(Timer, Time);
         glUniform3f(ViewPos_ID, camera.CameraPos[0], camera.CameraPos[1], camera.CameraPos[2]);
         glUniform3f(UobjColor, FourModel.MeshList[i].DiffCol[0], FourModel.MeshList[i].DiffCol[1], FourModel.MeshList[i].DiffCol[2]);
         glUniform3f(USpecular, FourModel.MeshList[i].SpecCol[0], FourModel.MeshList[i].SpecCol[1], FourModel.MeshList[i].SpecCol[2]);
         glUniform1f(UShininess, FourModel.MeshList[i].Shininess);
         glUniformMatrix4fv(UModel, 1, GL_FALSE,(const float*)FourModelMatrix);
         glUniformMatrix4fv(UView, 1, GL_FALSE,(const float*)camera.ViewMatrix);
         glUniformMatrix4fv(UProjection, 1, GL_FALSE,(const float*)ProjectionMatrix);
         glUniformMatrix4fv(MatrixUniform, 1, GL_FALSE,(const float*)FOURFINALMATRIXPOS);
         glBindVertexArray(FourModel.MeshList[i].VAO);
         glDrawElements(GL_TRIANGLES, (int)FourModel.MeshList[i].numIndices, GL_UNSIGNED_INT, 0);
      }
      SDL_GL_SwapWindow(app.Window);
   }


}
