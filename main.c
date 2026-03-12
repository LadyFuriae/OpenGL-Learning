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

#include <Shader.h>

//FANCY SHADER

#define aPOS_ID 0
#define aColor_ID 1
#define aTexPos_ID 2

#define SDL_MAIN_HANDLED
#define BUFFER_OBJECT_COUNT 1
#define STRING_COUNT 1
#define FIGURE_COMPONENT_NUM 3
#define TEX_COMPONENT_NUM 2
#define NOEMNTS(x) (int)(sizeof(x) / sizeof((x)[0]))

static bool Running = false;


int LinkingSucces = false;

typedef struct
{
   float x, y, z;
  // float r, g, b;
   float t, s;
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
   glVertexAttribPointer(aTexPos_ID, TEX_COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(aTexPos_ID);
   //glVertexAttribPointer(aColor_ID, FIGURE_FIGURE_COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(3 * sizeof(float)));
   //glEnableVertexAttribArray(aColor_ID);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      
   return ID_VAO;
}

int main()
{

   BindOpenGLToSDL();
   App app;

   if(!AppInit(&app))
      return -1;

   if (!BindGLContext(&app))
      return -1;

   SDL_Log("Init");

   Vertex Vertices1[] =
   {
      {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f},
  {  0.5f, -0.5f, -0.5f,  1.0f, 0.0f},
      {  0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
      {  0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
      { -0.5f,  0.5f, -0.5f,  0.0f, 1.0f},
      { -0.5f, -0.5f, -0.5f,  0.0f, 0.0f},
      { -0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
      {  0.5f, -0.5f,  0.5f,  1.0f, 0.0f},
      {  0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
      {  0.5f,  0.5f,  0.5f,  1.0f, 1.0f},
      { -0.5f,  0.5f,  0.5f,  0.0f, 1.0f},
      { -0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
      { -0.5f,  0.5f,  0.5f,  1.0f, 0.0f},
      { -0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
      { -0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
      { -0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
      { -0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
      { -0.5f,  0.5f,  0.5f,  1.0f, 0.0f},
      {  0.5f,  0.5f,  0.5f,  1.0f, 0.0f},
      {  0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
      {  0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
      {  0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
      {  0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
      {  0.5f,  0.5f,  0.5f,  1.0f, 0.0f},
      { -0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
      {  0.5f, -0.5f, -0.5f,  1.0f, 1.0f},
      {  0.5f, -0.5f,  0.5f,  1.0f, 0.0f},
      {  0.5f, -0.5f,  0.5f,  1.0f, 0.0f},
      { -0.5f, -0.5f,  0.5f,  0.0f, 0.0f},
      { -0.5f, -0.5f, -0.5f,  0.0f, 1.0f},
      { -0.5f,  0.5f, -0.5f,  0.0f, 1.0f},
      {  0.5f,  0.5f, -0.5f,  1.0f, 1.0f},
      {  0.5f,  0.5f,  0.5f,  1.0f, 0.0f},
      {  0.5f,  0.5f,  0.5f,  1.0f, 0.0f},
      { -0.5f,  0.5f,  0.5f,  0.0f, 0.0f},
      { -0.5f,  0.5f, -0.5f,  0.0f, 1.0f}
   };

   unsigned int Indices1[] =
   {
      0,1,2,
      0,2,3
   };

   //glEnable(GL_BLEND);// you enable blending function
   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
   const int ID_Program = ManageShaders();
   if (!ID_Program)
      return -1;

   glUseProgram(ID_Program);
   CreateSample(ID_Program, "Texture1", 0);
   CreateSample(ID_Program, "Texture2", 1);

   Texture ContainerTexture;
   LoadTexture(&ContainerTexture, "../game/textures/container.jpg");
   CreateTexture(&ContainerTexture, GL_TEXTURE_2D, 0,  1, GL_RGB);
   SetTextureParams(&ContainerTexture);
   GenerateTexture(&ContainerTexture);

   Texture HappyTexture;
   LoadTexture(&HappyTexture, "../game/textures/awesomeface.png");
   CreateTexture(&HappyTexture, GL_TEXTURE_2D,1, 1, GL_RGBA);
   SetTextureParams(&HappyTexture);
   GenerateTexture(&HappyTexture);

   const unsigned int VAO1 = PrepareRectangle(Vertices1, NOEMNTS(Vertices1), Indices1, sizeof(Indices1));
   //const unsigned int VAO2 = PrepareRectangle(Vertices2, NOEMNTS(Vertices2), Indices2, sizeof(Indices2));
   
   Running = true;
   const int Timer = glGetUniformLocation(ID_Program, "Timer");
   const int ID_X = glGetUniformLocation(ID_Program, "xCoordinate");
   const int ID_Y = glGetUniformLocation(ID_Program, "yCoordinate");

   float x = 0.0f;
   float y = 0.0f;

   const int MatrixUniform = glGetUniformLocation(ID_Program, "Trans");

   const int UModel = glGetUniformLocation(ID_Program, "Model");
   const int UView = glGetUniformLocation(ID_Program, "View");
   const int UProjection = glGetUniformLocation(ID_Program, "Projection");
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
   while(Running)
   {
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

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      const float Time = SDL_GetTicks() / 1000.0f;
      deltaTime = Time - lastFrame;
      lastFrame = Time;

      camera.CameraSpeed = 2.5f * deltaTime;
      glm_rotate(Trans, glm_rad(0.5f), (vec3){0.5f, 1.0f, 0.0f});

      mat4 ModelMatrix = GLM_MAT4_IDENTITY_INIT;


      glUniformMatrix4fv(UModel, 1, GL_FALSE,(const float*)ModelMatrix);
      glUniformMatrix4fv(UView, 1, GL_FALSE,(const float*)camera.ViewMatrix);
      glUniformMatrix4fv(UProjection, 1, GL_FALSE,(const float*)ProjectionMatrix);


      glUniform1f(ID_X, x);
      glUniform1f(ID_Y, y);



      glUniform1f(Timer, Time);
      glUniformMatrix4fv(MatrixUniform, 1, GL_FALSE,(const float*)Trans);

      glUseProgram(ID_Program);

      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);



      glBindVertexArray(VAO1);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      SDL_GL_SwapWindow(app.Window);
   }
}