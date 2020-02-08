#pragma once

#include <glm/glm.hpp>


namespace Hazel
{

	class Camera
	{
	public:

		Camera(float left, float right, float bottom, float top);
		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; };
		inline void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); };
		float GetRotation() const { return m_Rotation; };
		inline void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); };


		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; };
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; };
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; };

	private:
		void RecalculateViewMatrix();

	private:

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;

		float m_FOV = 45.0f;
		float m_Aspect = 1280.0f / 720.0f;
		float m_Near = 1.0f;
		float m_Far = 150.0f;

	};

}