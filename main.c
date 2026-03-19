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
#include <Shapes/lightcube.h>
#include <Shader.h>

#include <model.h>

#include "normalcube.h"

//FANCY SHADER
#define SDL_MAIN_HANDLED

#define STRING_COUNT 1

#define NOEMNTS(x) (int)(sizeof(x) / sizeof((x)[0]))

static bool Running = false;
const char* HONOKA = "/home/ladyfuriae/Downloads/OpenGL/game/Models/Honoka/Honoka/Honoka.FBX";
const char* ACCORD = "/home/ladyfuriae/OpenGL/game/Models/Accord/Accord.obj";
const char* HONOKASWIMSUIT = "/home/ladyfuriae/Downloads/OpenGL/game/Models/HonokaSwimsuit/Swimsuit/HonokaSwimSuit.FBX";
const char* HONOKAFACHAN = "/home/ladyfuriae/Downloads/OpenGL/game/Models/FACHANHONOKA/FACHAN/HFACHAN.FBX";
const char* NEKOARC = "/home/ladyfuriae/OpenGL/game/Models/gurenia/source/neco-arc-my-version.glb";
const char* YOOOO = "/home/ladyfuriae/Downloads/Bug/OpenGL/game/Models/yo/yo.fbx";

int main()
{
   App app;
   if(!AppInit(&app))
      return -1;

   BindOpenGLToSDL();
   if (!BindGLContext(&app))
      return -1;

   SDL_Log("Init");

   const int ID_ModelProgram  = SetModelShader();
   const int ID_LightProgram = SetLightShader();
   const int ID_NormalCubeProgram = SetNormalCubeShader();
   glUseProgram(ID_NormalCubeProgram);
   Texture NormalCubeTex;
   LoadTexture(&NormalCubeTex, "/home/ladyfuriae/Downloads/OpenGL/game/textures/container2.png");
   CreateSample(ID_NormalCubeProgram, "material.diffuse", 0);
   CreateTexture(&NormalCubeTex, GL_TEXTURE_2D, 0, 1);
   SetTextureParams(&NormalCubeTex, GL_RGBA8);
   NormalCubeTex.Format = GL_RGBA;
   NormalCubeTex.InternType = GL_RGBA8;
   GenerateTexture(&NormalCubeTex);

   Texture SpecularCubeTex;
   LoadTexture(&SpecularCubeTex, "/home/ladyfuriae/Downloads/OpenGL/game/textures/container2_specular.png");
   CreateSample(ID_NormalCubeProgram, "material.specular", 1);
   CreateTexture(&SpecularCubeTex, GL_TEXTURE_2D, 1, 1);
   SetTextureParams(&SpecularCubeTex, GL_RGBA8);
   SpecularCubeTex.Format = GL_RGBA;
   SpecularCubeTex.InternType = GL_RGBA8;
   GenerateTexture(&SpecularCubeTex);

   if (!ID_ModelProgram)
      return -1;

   if(!ID_LightProgram)
      return -1;

   if (!ID_NormalCubeProgram)
      return -1;


   const int ULighTrans = glGetUniformLocation(ID_LightProgram, "Trans");
   const int UlightModel = glGetUniformLocation(ID_LightProgram, "Model");
   const int ULightView = glGetUniformLocation(ID_LightProgram, "View");
   const int ULightProjection = glGetUniformLocation(ID_LightProgram, "Projection");
   Running = true;

   glUseProgram(ID_ModelProgram);

   Model Honoka;
   LoadModel(&Honoka, HONOKAFACHAN, ID_ModelProgram);
   SendModel(&Honoka);
   CleanModel(&Honoka);

   Model SwimSuit;
   LoadModel(&SwimSuit, HONOKASWIMSUIT, ID_ModelProgram);
   SendModel(&SwimSuit);
   CleanModel(&SwimSuit);

   MoveModel(&Honoka, (vec3){0.0f, 0.0f, 0.0f});
   ScaleModel(&Honoka, (vec3){0.8f, 0.8f, 0.8f});

   MoveModel(&SwimSuit, (vec3){0.6f, 1.6f, 0.6f});
   ScaleModel(&SwimSuit, (vec3){0.8f, 0.8f, 0.8f});

   const int Timer = glGetUniformLocation(ID_ModelProgram, "Timer");

   //MODEL UNIFORMS
   ModelUniform ModelUniforms;
   ModelUniforms.PositionMatrix = glGetUniformLocation(ID_ModelProgram, "Trans");
   ModelUniforms.ModelMatrix = glGetUniformLocation(ID_ModelProgram, "Model");
   ModelUniforms.ViewMatrix = glGetUniformLocation(ID_ModelProgram, "View");
   ModelUniforms.ProjectionMatrix = glGetUniformLocation(ID_ModelProgram, "Projection");

   const int ULight = glGetUniformLocation(ID_ModelProgram, "LightColor");
   ModelUniforms.ObjectColor = glGetUniformLocation(ID_ModelProgram, "ObjColor");
   ModelUniforms.Specular = glGetUniformLocation(ID_ModelProgram, "Specular");
   ModelUniforms.Shininess = glGetUniformLocation(ID_ModelProgram, "Shininess");
   glUniform3f(ULight, 1.0f, 1.0f, 1.0f);

   unsigned int VAO_Cube = PrepareLightCube(CubeVertices, 36);

   glEnable(GL_DEPTH_TEST);

   Camera camera;
   PrepareCamera(&camera);
   SetViewCameraMatrix(&camera);

   glm_mat4_identity(camera.ViewMatrix);
   mat4 ProjectionMatrix = GLM_MAT4_IDENTITY_INIT;

   glm_perspective(glm_rad(90.0f), 1920.0f/1080.0f , 0.01f, 100.0f, ProjectionMatrix);
   bool isEnabled = false;
   SDL_SetWindowRelativeMouseMode(app.Window, isEnabled);
   float mouseX, mouseY;
   const bool* State = SDL_GetKeyboardState(nullptr);

   vec3 lightPos = {-1.2f, 1.0f, -2.0f };
   glUseProgram(ID_ModelProgram);

   int lightpos_ID = glGetUniformLocation(ID_ModelProgram, "LightPosition");
   int lightcolor_ID = glGetUniformLocation(ID_ModelProgram, "LightColor");
   int ViewPos_ID = glGetUniformLocation(ID_ModelProgram, "ViewPos");

   glUniform3f(lightpos_ID, lightPos[0], lightPos[1], lightPos[2]);
   glUniform3f(lightcolor_ID, 1.0f, 1.0f, 1.0f);
   mat4 LightTransMatrix = GLM_MAT4_IDENTITY_INIT;
   glm_translate(LightTransMatrix, lightPos);

   float deltaTime = 0.0f;
   float lastFrame = 0.0f;

   //SetNormalCubeUniforms
   glUseProgram(ID_NormalCubeProgram);
   const int uNCTtransM = glGetUniformLocation(ID_NormalCubeProgram, "Trans");
   const int uNCTModelM = glGetUniformLocation(ID_NormalCubeProgram, "Model");
   const int uNCViewM = glGetUniformLocation(ID_NormalCubeProgram, "View");
   const int uNCProjM = glGetUniformLocation(ID_NormalCubeProgram, "Projection");

   const int uNCAmbient = glGetUniformLocation(ID_NormalCubeProgram, "material.ambient");
   const int uNCDiff = glGetUniformLocation(ID_NormalCubeProgram, "material.diffuse");
   const int uNCSpec = glGetUniformLocation(ID_NormalCubeProgram, "material.specular");
   const int uNCShininess = glGetUniformLocation(ID_NormalCubeProgram, "material.shininess");
   const int uNCLightColor = glGetUniformLocation(ID_NormalCubeProgram, "LightColor");
   const int uNCViewPos = glGetUniformLocation(ID_NormalCubeProgram, "ViewPos");
   const int uNCObjColor = glGetUniformLocation(ID_NormalCubeProgram, "ObjColor");

   const int uNCLightPos = glGetUniformLocation(ID_NormalCubeProgram, "light.position");
   const int uNCLighDir = glGetUniformLocation(ID_NormalCubeProgram, "light.direction");
   const int uNCLightCutoff = glGetUniformLocation(ID_NormalCubeProgram, "light.cutOff");
   const int uNCLightOuterCutoff = glGetUniformLocation(ID_NormalCubeProgram, "light.outerCutOff");

   const int uNCLightAmbient = glGetUniformLocation(ID_NormalCubeProgram, "light.ambient");
   const int uNCLightDiffuse = glGetUniformLocation(ID_NormalCubeProgram, "light.diffuse");
   const int uNCLightSpecular = glGetUniformLocation(ID_NormalCubeProgram, "light.specular");

   const int uNCLightConstant = glGetUniformLocation(ID_NormalCubeProgram, "light.constant");
   const int uNCLightLinear = glGetUniformLocation(ID_NormalCubeProgram, "light.linear");
   const int uNCLightQuadratic = glGetUniformLocation(ID_NormalCubeProgram, "light.quadratic");

   vec3 NCPos = {-9.0f, 1.0f, 1.0f};

   mat4 NCModelMatrix = GLM_MAT4_IDENTITY_INIT;
   mat4 NCTransMatrix = GLM_MAT4_IDENTITY_INIT;

   glm_translate(NCTransMatrix, NCPos);
   glm_scale(NCModelMatrix, (vec3){1.0f, 1.0f, 1.0f});

   glUniformMatrix4fv(uNCTtransM, 1, GL_FALSE, (const float*)NCTransMatrix);
   glUniformMatrix4fv(uNCProjM, 1, GL_FALSE, (const float*)ProjectionMatrix);
   glUniform3f(uNCLightColor, 1.0f, 1.0f, 1.0f);
   glUniform3f(uNCLightPos, lightPos[0], lightPos[1], lightPos[2]);

   //glUniform3f(uNCAmbient, 0.5f,0.2f,0.6f);
   //glUniform3f(uNCDiff, 0.5f,0.5f, 0.5f);
   //glUniform3f(uNCSpec, 0.5f, 0.5f, 0.5f);
   glUniform1f(uNCShininess, 64.0f);

   glUniform3f(uNCLightAmbient, 0.2f, 0.2f, 0.2f);
   glUniform3f(uNCLightDiffuse, 0.9f, 0.9f, 0.9f);
   glUniform3f(uNCLightSpecular, 12.0f, 12.0f, 12.0f);
   glUniform1f(uNCLightConstant, 1.0f);
   glUniform1f(uNCLightLinear, 0.07f);
   glUniform1f(uNCLightQuadratic, 0.032f);

   unsigned int NCVAO = PrepareNormalCube(NormalCubeVertices, 36);
   float cutOff = 1.0f;
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

         else if (event.type == SDL_EVENT_MOUSE_WHEEL)
         {
            if (event.wheel.y < 0)
            {
               printf("Up\n");
               cutOff++;
            }
            else if (event.wheel.y > 0)
            {
               printf("Down\n");
               cutOff--;
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

      const float Time = SDL_GetTicks() / 1000.0f;
      deltaTime = Time - lastFrame;
      lastFrame = Time;

      mat4 ModelMatrix = GLM_MAT4_IDENTITY_INIT;

      

       glUseProgram(ID_LightProgram);
       //glUniform1f(Timer, Time);
       glUniformMatrix4fv(UlightModel, 1, GL_FALSE,(const float*)ModelMatrix);
       glUniformMatrix4fv(ULightView, 1, GL_FALSE,(const float*)camera.ViewMatrix);
       glUniformMatrix4fv(ULightProjection, 1, GL_FALSE,(const float*)ProjectionMatrix);
       glUniformMatrix4fv(ULighTrans, 1, GL_FALSE,(const float*)LightTransMatrix);
       glBindVertexArray(VAO_Cube);
       glDrawArrays(GL_TRIANGLES, 0, 36);

      glUseProgram(ID_NormalCubeProgram);

      //glm_rotate(NCTransMatrix, glm_rad(0.5f), (vec3){0.5f, 1.0f, 1.0f});
      glm_mat4_identity(NCTransMatrix);
      float time = SDL_GetTicks() / 1000.0f;
      //glm_translate(NCTransMatrix, (vec3){sinf(time * 0.3f) *50.0f, 0.0f, 0.0f});
      glUniformMatrix4fv(uNCViewM, 1, GL_FALSE, (const float*)camera.ViewMatrix);
      glUniformMatrix4fv(uNCTModelM, 1, GL_FALSE, (const float*)NCModelMatrix);
      glUniformMatrix4fv(uNCTtransM, 1, GL_FALSE, (const float*)NCTransMatrix);
      glUniform3f(uNCViewPos, camera.CameraPos[0], camera.CameraPos[1], camera.CameraPos[2]);
      glUniform3f(uNCLightPos, camera.CameraPos[0], camera.CameraPos[1], camera.CameraPos[2]);
      glUniform3f(uNCLighDir, camera.CameraFront[0], camera.CameraFront[1], camera.CameraFront[2]);
      glUniform1f(uNCLightCutoff, cosf(glm_rad(cutOff)));
      glUniform1f(uNCLightOuterCutoff, cosf(glm_rad(cutOff*1.3f)));


      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, NormalCubeTex.ID);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, SpecularCubeTex.ID);
      glBindVertexArray(NCVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      glUseProgram(ID_ModelProgram);
      glm_rotate(SwimSuit.PosMatrix, glm_rad(0.9f), (vec3){0.0f, 1.0f, 0.0f});
      glm_rotate(Honoka.PosMatrix, glm_rad(0.9f), (vec3){0.0f, 1.0f, 0.0f});
      RenderModel(&Honoka, &camera, &ModelUniforms, ProjectionMatrix);
      RenderModel(&SwimSuit, &camera, &ModelUniforms, ProjectionMatrix);

      SDL_GL_SwapWindow(app.Window);
   }
}
