#include "Camera.h"
#include <iostream>
#include <string>


Camera::Camera(glm::vec3 position, glm::vec3 worldup, float yaw, float pitch)
: m_Position(position), m_WorldUp(worldup), m_Yaw(yaw), m_Pitch(pitch)
{
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    if (m_Mode == 0)
    {
        m_Yaw += xoffset;
        m_Pitch += yoffset;

        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
        updateCameraVectors();
    }
    else if (m_Mode == 1)
    {
        xo -= xoffset;
        yo -= yoffset * 0.01;

        if (yo > 0.9)
            yo = 0.9;
        else if (yo < 0.0)
            yo = 0.0;
        refreshFollow();

    }   
}

void Camera::ProcessKeyboard(Movement direction, float deltaTime)
{
    float velocity = m_MovementSpeed * deltaTime;

    if (direction == FORWARD)
        m_Position += m_Front * velocity;
    if (direction == BACKWARD)
        m_Position -= m_Front * velocity;
    if (direction == LEFT)
        m_Position -= m_Right * velocity;
    if (direction == RIGHT)
        m_Position += m_Right * velocity;
    //m_Position.y = 1;
}

void Camera::ProcessMouseScroll(float yOffset)
{
    if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
        m_Zoom -= yOffset / 10;
    if (m_Zoom <= 1.0f)
        m_Zoom = 1.0f;
    if (m_Zoom >= 45.0f)
        m_Zoom = 45.0f;
}

void Camera::setCamera(glm::vec3 position)
{
    m_Position = position;
}

void Camera::SetFollow(glm::vec3 pos)
{    
    m_Following = pos;
    refreshFollow();
    m_Mode = 1;
}

void Camera::refreshFollow()
{
    if (m_Mode == 1)
    {
        float k = 5;

        float y = sin(yo);
        float x = (sin(xo * 0.02) * (1 - y) * k) * -1;
        float z = (cos(xo * 0.02) * (1 - y) * k) * -1;
        y = y * k;

        m_Position = glm::vec3(x + m_Following.x, y + m_Following.y, z + m_Following.z);
        glm::vec3 kot = glm::vec3(-x, -y + 1, -z);

        m_Front = glm::normalize(kot);        
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    }
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}