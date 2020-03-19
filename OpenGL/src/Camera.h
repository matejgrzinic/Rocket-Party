#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
    glm::vec3 m_Position;
    glm::vec3 m_WorldUp;
    glm::vec3 m_Up;
    glm::vec3 m_Front = glm::vec3(0.0f, 2.0f, -1.0f);
    glm::vec3 m_Right;

    float xo = 0;
    float yo = 1;
    glm::vec3 offset = glm::vec3(0.0f, 0.0f, -2.0f);
    
    float m_Yaw;
    float m_Pitch;

    float m_MovementSpeed = 3.0f;
    float m_MouseSensitivity = 0.10f;
    float m_Zoom = 45.0f;

    glm::vec3 m_Following;

    void updateCameraVectors();
public:
    int m_Mode = 0;
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 worldup = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);
    glm::mat4 GetViewMatrix();
    inline glm::vec3 GetPosition() { return m_Position; };
    inline glm::vec3 GetFront() { return m_Front; };
    inline glm::vec3 GetRight() { return m_Right; };
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessKeyboard(Movement direction, float deltaTime);
    void ProcessMouseScroll(float yOffset);
    inline float GetZoom() { return m_Zoom; };
    void setCamera(glm::vec3 position);
    void SetFollow(glm::vec3 pos);
    void refreshFollow();
    inline void setFront(glm::vec3 front) { m_Front = front; };
    inline void otherWay() { xo *= -1; };
};