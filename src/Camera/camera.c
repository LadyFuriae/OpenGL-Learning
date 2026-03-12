#include<camera.h>
#include<SDL3/SDL.h>

void PrepareCamera(Camera* camera)
{
    camera->CameraPos[0] = 0.0f;
    camera->CameraPos[1] = 0.0f;
    camera->CameraPos[2] = 3.0f;

    camera->CameraFront[0] = 0.0f;
    camera->CameraFront[1] = 0.0f;
    camera->CameraFront[2] = -1.0f;

    camera->CameraUp[0] = 0.0f;
    camera->CameraUp[1] = 1.0f;
    camera->CameraUp[2] = 0.0f;

    camera->Yaw = -90.0f;
    camera->Pitch = 0.0f;
    camera->CameraSpeed = 1.0f;
    camera->Sensitivity = 0.1f;
}

void CalcCamMov(Camera* camera, float* mouseX, float* mouseY)
{
    camera->Yaw += *mouseX * camera->Sensitivity;
    camera->Pitch -= *mouseY * camera->Sensitivity;
}

void UpdateCamera(Camera* camera)
{
    float RadYaw = glm_rad(camera->Yaw);
    float RadPitch = glm_rad(camera->Pitch);

    vec3 front;
    front[0] = cosf(RadPitch) * cosf(RadYaw);
    front[1] = sinf(RadPitch);
    front[2] = sinf(RadYaw) * cosf(RadPitch);

    glm_normalize_to(front, camera->CameraFront);
}

void SetViewCameraMatrix(Camera* camera)
{
    vec3 direction;
    glm_vec3_add(camera->CameraPos, camera->CameraFront, direction);
    glm_lookat(camera->CameraPos, direction, camera->CameraUp, camera->ViewMatrix);
}

void ManageKeyboard(const bool* State, Camera* camera)
{
    if (State[SDL_SCANCODE_D])
    {
        vec3 cross;
        glm_cross(camera->CameraFront, camera->CameraUp, cross);
        glm_normalize(cross);
        glm_vec3_scale(cross, camera->CameraSpeed, cross);
        glm_vec3_add(camera->CameraPos, cross, camera->CameraPos);
    }
    if (State[SDL_SCANCODE_A])
    {
        vec3 cross;
        glm_cross(camera->CameraFront, camera->CameraUp, cross);
        glm_normalize(cross);
        glm_vec3_scale(cross, camera->CameraSpeed, cross);
        glm_vec3_sub(camera->CameraPos, cross, camera->CameraPos);
    }
    if (State[SDL_SCANCODE_W])
    {
        vec3 finalPos;
        glm_vec3_scale(camera->CameraFront, camera->CameraSpeed, finalPos);
        glm_vec3_add(camera->CameraPos, finalPos, camera->CameraPos);
    }
    if (State[SDL_SCANCODE_S])
    {
        vec3 finalPos;
        glm_vec3_scale(camera->CameraFront, camera->CameraSpeed, finalPos);
        glm_vec3_sub(camera->CameraPos, finalPos, camera->CameraPos);
    }
}
