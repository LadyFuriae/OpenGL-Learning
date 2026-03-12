#ifndef GPU_CAMERA_H
#define GPU_CAMERA_H
#include <../external/cglm/cglm.h>
#include <../external/glad/include/glad/glad.h>

typedef struct
{
    float CameraSpeed;
    float Yaw;
    float Pitch;
    float Sensitivity;
    vec3 CameraPos;
    vec3 CameraFront;
    vec3 CameraUp;
    mat4 ViewMatrix;

} Camera;

void PrepareCamera(Camera* camera);
void CalcCamMov(Camera* camera, float* mouseX, float* mouseY);
void ManageKeyboard(const bool* State, Camera* camera);
void UpdateCamera(Camera* camera);
void SetViewCameraMatrix(Camera* camera);

#endif //GPU_CAMERA_H