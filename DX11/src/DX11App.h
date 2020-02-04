#include <Hazel.h>
#include "Hazel/Window.h"

#include "GameEngine/WindowingSystem/Window.h"
#include "GameEngine/GraphicsEngine/RenderSystem/RenderSystem.h"
#include "GameEngine/GraphicsEngine/RenderSystem/SwapChain/SwapChain.h"
#include "GameEngine/GraphicsEngine/RenderSystem/DeviceContext/DeviceContext.h"
#include "GameEngine/GraphicsEngine/RenderSystem/VertexBuffer/VertexBuffer.h"
#include "GameEngine/GraphicsEngine/RenderSystem/IndexBuffer/IndexBuffer.h"
#include "GameEngine/GraphicsEngine/RenderSystem/ConstantBuffer/ConstantBuffer.h"
#include "GameEngine/GraphicsEngine/RenderSystem/VertexShader/VertexShader.h"
#include "GameEngine/GraphicsEngine/RenderSystem/PixelShader/PixelShader.h"
#include "GameEngine/InputSystem/InputListener.h"
#include "GameEngine/Math/Matrix4x4.h"
#include "GameEngine/Math/Point.h"

#include "Hazel/Renderer/RendererAPI.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <iostream>
#include <chrono>



class DX11Layer : public Hazel::Layer
{

private:

	Hazel::Window* window;
	GLFWwindow* windowHandler;
	HWND m_hwnd;
	float width;
	float height;
	bool m_is_run;

	RenderSystem* m_render_system;
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	IndexBuffer* m_ib;
	ConstantBuffer* m_cb;
	VertexShader* m_vs;
	PixelShader* m_ps;

	// Camera
	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 2.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 20.0f;

public:

	DX11Layer();
	void OnUpdate(Hazel::Timestep timestep) override;
	void OnEvent(Hazel::Event& event) override;
	~DX11Layer();

private:

	void Create();
	void UpdateInputPolling(Hazel::Timestep timestep);
	bool OnWindowResizeEvent(Hazel::WindowResizeEvent& event);
	void UpdateScene();

};

class DX11App : public Hazel::Application
{

public:

	DX11App();
	~DX11App();

};

Hazel::Application* Hazel::CreateApplication()
{
	RendererAPI::SetAPI(RendererAPI::API::DX11);

	return new DX11App();
}
