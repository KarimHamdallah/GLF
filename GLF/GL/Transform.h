#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

struct TransformComponent
{
public:
	TransformComponent() = default;
	TransformComponent(const glm::vec3& Position, const glm::vec3& Rotation, const glm::vec3& Scale)
		: m_Position(Position), m_Rotation(Rotation), m_Scale(Scale)
	{}

	glm::mat4 GetTransformationMatrix() const
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
		glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_Scale);
		return translation * rotation * scale;
	}

	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };
};
