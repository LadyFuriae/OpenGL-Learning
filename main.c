#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include <stdio.h>
#include<glad/glad.h>
#include<cglm/cglm.h>

#define SDL_MAIN_HANDLED
#define BUFFER_OBJECT_COUNT 1
#define STRING_COUNT 1
#define INFO_LOG_SIZE 512
#define MAX_SHADER_SIZE 1024
#define aPOS_ID 0
#define aColor_ID 1
#define COMPONENT_NUM 3
#define NOEMNTS(x) (int)(sizeof(x) / sizeof((x)[0]))

static bool Running = false;

int LinkingSucces = false;

typedef struct 
{
   SDL_Window* Window; 
   SDL_GLContext GL_Context;
} AppContext;

typedef struct
{     
   FILE* ShaderFile;
   char ShaderData[MAX_SHADER_SIZE];
}ShaderSource;

typedef struct
{
   const char* Title;
   int Width;
   int Height;
   SDL_WindowFlags Flags;

} WindowProperties;

WindowProperties GET_DEFAULT_WINDOW_PROPERTIES()
{
   WindowProperties Properties = 
   {
    .Title = "OpenGL Learning",
    .Width = 800,
    .Height = 600,
    .Flags = 0
   };

   return Properties;
}

typedef struct
{
   float x, y, z;
   float r, g, b;
} Vertex;

AppContext* IntializeApp()
{  
   if(!SDL_Init(SDL_INIT_VIDEO))
   {
      SDL_Log("Failed to initialize SDL device");
      return nullptr;
   }

   SDL_Log("SDL device ready!");

   AppContext* Context = (AppContext*)calloc(1,sizeof(AppContext));

   WindowProperties W_Properties = GET_DEFAULT_WINDOW_PROPERTIES();
   W_Properties.Flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
   Context->Window = SDL_CreateWindow(W_Properties.Title, W_Properties.Width, W_Properties.Height, W_Properties.Flags);

   if(!Context->Window)
      return nullptr;

	return Context;
}


void BindOpenGLToSDL()
{
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

void BindGLContext(AppContext* Context)
{
   Context->GL_Context = SDL_GL_CreateContext(Context->Window);
   if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) 
   {
      SDL_Log("GLAD initilization failed");
   }
   
   printf("GLAD intialized\n");
}

int GenAndBindVAO()
{
   unsigned int ID_VAO;
   glGenVertexArrays(BUFFER_OBJECT_COUNT, &ID_VAO);
   glBindVertexArray(ID_VAO);
   return ID_VAO;
}

int GenAndBindVBO()
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


int PrepareRectangle(Vertex Vertices[], int VerticesCount, unsigned int Indices[], int IndicesCount)
{

   int ID_VAO = GenAndBindVAO();
   //int ID_VBO;
   int ID_VBO = GenAndBindVBO();
   unsigned int EBO = GenAndBindEBO();
   //Create buffer to the GPU
   glBufferData(GL_ARRAY_BUFFER, VerticesCount * sizeof(Vertex), Vertices, GL_STATIC_DRAW);
   //Bind EBO to the GL_ARRAY_BUFFER
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesCount, Indices, GL_STATIC_DRAW);
   glVertexAttribPointer(aPOS_ID, COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)0);
   glEnableVertexAttribArray(aPOS_ID);
   //glVertexAttribPointer(aColor_ID, COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(3 * sizeof(float)));
   //glEnableVertexAttribArray(aColor_ID);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      
   return ID_VAO;
}

int CreateShader(const char* src, int Type)
{
   int ShaderID = glCreateShader(Type);
   glShaderSource(ShaderID, STRING_COUNT, &src, NULL);
   return ShaderID;
}

int CheckShaderCompilationStatus(int ID_Shader, const char* Type)
{
   char infoLog[INFO_LOG_SIZE];

   int CompilationSuccess;
   glGetShaderiv(ID_Shader, GL_COMPILE_STATUS, &CompilationSuccess);
   if(!CompilationSuccess)
   {
      glGetShaderInfoLog(ID_Shader, INFO_LOG_SIZE, NULL, infoLog);
      SDL_Log(" Shader compilation failed\nShader ID: %d\nLog: %s\n", ID_Shader, infoLog);
      return -1;
   }
   SDL_Log("%s Shader created succesfully!\n", Type);

   return 1;
}

void AddShadersToProgram(int ShaderProgramID, int ShaderIDs[], int Count)
{
   for(int i = 0; i < Count; i++)
   {
      glAttachShader(ShaderProgramID, ShaderIDs[i]);
   }
}

int LinkShaders(int ID_ShaderProgram)
{
   char infoLog[512];
   glLinkProgram(ID_ShaderProgram);
   glGetProgramiv(ID_ShaderProgram, GL_LINK_STATUS, &LinkingSucces);

   if(!LinkingSucces)
   {
      glGetProgramInfoLog(ID_ShaderProgram, INFO_LOG_SIZE, NULL, infoLog);
      SDL_Log("Shader Program linkage failed.\nProgram ID: %d\nLog: %s\n", ID_ShaderProgram, infoLog);
      return -1;
   }

   return 1;
}

void CleanShaders(int Shaders[], int Count)
{
   for(int i = 0; i < Count; i++)
   {
      glDeleteShader(Shaders[i]);
   }
}

ShaderSource GetShaderFromFile(const char* FileName)
{
   ShaderSource src;
   src.ShaderFile = fopen(FileName, "r");

   unsigned short IndexSource = 0;

   memset(src.ShaderData,0, sizeof(src.ShaderData));
   
   char BufferData[sizeof(src.ShaderData)];

   char* ptr;
   while ((ptr = fgets(BufferData, sizeof(src.ShaderData), src.ShaderFile)))
   {
      for(; *ptr != '\0'  && IndexSource <= sizeof(src.ShaderData); IndexSource++, ptr++)
      {
         src.ShaderData[IndexSource] = *ptr;

         if(*ptr == '\n')
         {
            IndexSource++;
            break;
         }
      }
   }

   fclose(src.ShaderFile);
   return src;
}

int ManageShaders()
{

   ShaderSource VertexShader = GetShaderFromFile("../VertexShader.glsl");
   ShaderSource FragmentShader = GetShaderFromFile("../FragmentShader.glsl");

   int ID_VertexShader = CreateShader((const char*)VertexShader.ShaderData, GL_VERTEX_SHADER);
   int ID_FragmentShader = CreateShader((const char*)FragmentShader.ShaderData, GL_FRAGMENT_SHADER);

   glCompileShader(ID_VertexShader);
   CheckShaderCompilationStatus(ID_VertexShader, "Vertex");
   glCompileShader(ID_FragmentShader);
   CheckShaderCompilationStatus(ID_FragmentShader, "Fragment");

   int ID_ShaderProgram = glCreateProgram();
   int Shaders[] = {ID_VertexShader, ID_FragmentShader};

   AddShadersToProgram(ID_ShaderProgram, Shaders, NOEMNTS(Shaders));
   LinkShaders(ID_ShaderProgram);
   CleanShaders(Shaders, NOEMNTS(Shaders));

   return ID_ShaderProgram;
}

int main()
{

   BindOpenGLToSDL();
   
   AppContext* Context = IntializeApp();
   
   if(!Context)
   {
      SDL_Log("Failed creating SDL Window: %s", SDL_GetError());
      return -1;
   }
   
   BindGLContext(Context);
   SDL_Log("Init");
   
   
   Vertex Vertices1[] = 
   {
      { -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f}, //0
      {  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f}, //1
      {  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f}, //2
      { -0.5f,  0.5f, 0.0f, 0.4f, 0.1f, 0.9f}  //3
   };
   Vertex Vertices2[] = 
   {
      { -0.9f, -0.0f, 0.0f, 1.0f, 0.0f, 0.0f}, //0
      { -1.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f}, //1
      { -0.8f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f}, //2
      { -0.9f,  0.5f, 0.0f, 0.4f, 0.1f, 0.9f}  //3
   };
   unsigned int Indices1[] =
   {
      0,1,2,
      0,2,3
   };  

   unsigned int Indices2[] =
   {
   0,1,3,
   0,3,2
   };  


   int VAO1 = PrepareRectangle(Vertices1, NOEMNTS(Vertices1), Indices1, sizeof(Indices1));
   int VAO2 = PrepareRectangle(Vertices2, NOEMNTS(Vertices2), Indices2, sizeof(Indices2));

   int ID_ShaderProgram = ManageShaders();
   
   Running = true;
   int ColorLocation = glGetUniformLocation(ID_ShaderProgram, "fColor");
   int Timer = glGetUniformLocation(ID_ShaderProgram, "Timer");
   int ID_X = glGetUniformLocation(ID_ShaderProgram, "xCoordinate");
   int ID_Y = glGetUniformLocation(ID_ShaderProgram, "yCoordinate");
   float diffValue = 0.0f;
   glUseProgram(ID_ShaderProgram);

   float x = 0.0f;
   float y = 0.0f;

   const bool* State = SDL_GetKeyboardState(NULL);

   while(Running)
   {

      float Time = SDL_GetTicks() / 1000.0f;
      //float R = (sin(Time * 2) / 2.0f) + 0.5f;
      //float G = (sin(Time * 2) / 2.0f) + 0.5f;
      //float B = (sin(Time * 2) / 2.0f) + 0.5f;
      glUniform1f(ID_X, x);
      glUniform1f(ID_Y, y);

      //diffValue = (sin(Time) /2.0f) + 0.5f;


      glUniform1f(Timer, Time);

      glUseProgram(ID_ShaderProgram);

      glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      glBindVertexArray(VAO1);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
      
      glBindVertexArray(VAO2);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);

      SDL_GL_SwapWindow(Context->Window);

      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
         switch (event.type)
	      {
	      case SDL_EVENT_QUIT:
         
	      	Running = false;
            break;
         
	      default:
         
	      	break;
	      }
      }

      if (State[SDL_SCANCODE_D])
      {
        x += 0.1f;
      }
      if (State[SDL_SCANCODE_A])
      {
         x -= 0.1f;
      }
      if (State[SDL_SCANCODE_W])
      {
         y += 0.1f;
      }
      if (State[SDL_SCANCODE_S])
      {
          y -= 0.1f;
      }


   }
}