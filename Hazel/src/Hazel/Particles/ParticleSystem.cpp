#include <hzpch.h>

#include "ParticleSystem.h"

#include "Hazel/Particles/Random.h"
#include "Hazel/Models/Cube.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>


namespace Hazel
{
	ParticleSystem::ParticleSystem()
	{
		m_ParticlePool.resize(1000);
	}

	void ParticleSystem::OnUpdate(Hazel::Timestep ts)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * (float)ts;
			particle.Rotation = glm::vec3(
				particle.Rotation.x + Random::Float() * particle.RotationVelocity.x * (float)ts,
				particle.Rotation.y + Random::Float() * particle.RotationVelocity.y * (float)ts,
				particle.Rotation.z + Random::Float() * particle.RotationVelocity.z * (float)ts);
		}
	}

	void ParticleSystem::OnRender(Hazel::Camera& camera)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			// Fade away particles
			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
			color.a = color.a * life;

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

			// Render
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), particle.Position)
				* glm::rotate(glm::mat4(1.0f), particle.Rotation.x, { 1.0f, 0.0f, 0.0f })
				* glm::rotate(glm::mat4(1.0f), particle.Rotation.y, { 0.0f, 1.0f, 0.0f })
				* glm::rotate(glm::mat4(1.0f), particle.Rotation.z, { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { size, size, size });

			Hazel::Renderer::DrawCube(transform, color);
		}
	}

	void ParticleSystem::Emit(const ParticleProps& particleProps)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = particleProps.Position;
		particle.Rotation = glm::vec3(
			particleProps.RotationVelocity.x * Random::Float() * 2.0f * glm::pi<float>(),
			particleProps.RotationVelocity.y * Random::Float() * 2.0f * glm::pi<float>(),
			particleProps.RotationVelocity.z * Random::Float() * 2.0f * glm::pi<float>());

		// Velocity
		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);
		particle.Velocity.z += particleProps.VelocityVariation.z * (Random::Float() - 0.5f);

		// Rotation velocity
		particle.RotationVelocity = particleProps.RotationVelocity;
		particle.RotationVelocity.x += particleProps.RotationVelocity.x * (Random::Float() - 0.5f);
		particle.RotationVelocity.y += particleProps.RotationVelocity.y * (Random::Float() - 0.5f);
		particle.RotationVelocity.z += particleProps.RotationVelocity.z * (Random::Float() - 0.5f);

		// Color
		particle.ColorBegin = particleProps.ColorBegin;
		particle.ColorEnd = particleProps.ColorEnd;

		// Lifetime
		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;

		// Size
		particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
		particle.SizeEnd = particleProps.SizeEnd;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}
}
