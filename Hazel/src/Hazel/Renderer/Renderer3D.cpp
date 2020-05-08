#include "hzpch.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
#include "RenderCommand.h"
#include "Hazel/Models/Quad.h"
#include "Hazel/Models/Cube.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Hazel
{
	Renderer3D::SceneData* Renderer3D::m_SceneData = new Renderer3D::SceneData;

	struct RendererStorage
	{
		Ref<VertexArray> CubeVertexArray;
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static RendererStorage* s_Data;

	void Renderer3D::Init()
	{
		HZ_PROFILE_FUNCTION();

		HZ_INFO("Renderer3D Init");

		RenderCommand::Init();

		s_Data = new RendererStorage();

		/* Begin Cube vertex array */
		s_Data->CubeVertexArray = VertexArray::Create();

		Ref<VertexBuffer> cubeVB;
		cubeVB = VertexBuffer::Create(Cube::vertices, sizeof(Cube::vertices));
		cubeVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
		});

		s_Data->CubeVertexArray->AddVertexBuffer(cubeVB);

		Ref<IndexBuffer> cubeIB;
		cubeIB = IndexBuffer::Create(Cube::indices, sizeof(Cube::indices) / sizeof(uint32_t));
		s_Data->CubeVertexArray->SetIndexBuffer(cubeIB);
		/* End Cube vertex array */

		/* Begin Quad vertex array */
		s_Data->QuadVertexArray = VertexArray::Create();

		Ref<VertexBuffer> quadVB;
		quadVB = VertexBuffer::Create(Quad::vertices, sizeof(Quad::vertices));
		quadVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			});

		s_Data->QuadVertexArray->AddVertexBuffer(quadVB);

		Ref<IndexBuffer> quadIB;
		quadIB = IndexBuffer::Create(Quad::indices, sizeof(Quad::indices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(quadIB);
		/* End Quad vertex array */

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/Renderer3D_Texture.glsl");
	}

	void Renderer3D::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer3D::BeginScene(Camera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer3D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer3D::EndScene()
	{

	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), size);
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->WhiteTexture->Bind();
		
		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer3D::DrawCube(const glm::mat4& transform, const glm::vec4& color)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->WhiteTexture->Bind();
		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), size);
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		texture->Bind();
		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer3D::DrawCube(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		texture->Bind();
		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer3D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->WhiteTexture->Bind();
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer3D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		texture->Bind();
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer3D::Submit(Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
		const glm::mat4& viewProjectionMatrix, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", viewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);
		shader->SetFloat("u_TilingFactor", 1.0f);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}
