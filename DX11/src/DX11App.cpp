#include "DX11App.h"

#include "Hazel/Window.h"
#include "Hazel/Input.h"
#include "Hazel/Core.h"

#include "GameEngine/Math/Vector3D.h"
#include "GameEngine/Math/Matrix4x4.h"
#include "GameEngine/InputSystem/InputSystem.h"
#include "GameEngine/GraphicsEngine/GraphicsEngine.h"

#include <iostream>
#include <Windows.h>
#include <exception>


struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

DX11App::DX11App()
{
	PushLayer(new DX11Layer());
	// PushOverlay(new Hazel::ImGuiLayer());

	window = &(Application::Get().GetWindow());
	windowHandler = (GLFWwindow*)window->GetNativeWindow();

	m_render_system = GraphicsEngine::get()->getRenderSystem();

	try
	{
		Run();
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void DX11App::Create()
{
	RECT rect = this->getClientWindowRect();
	m_swap_chain = m_render_system->createSwapChain(this->m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	m_world_cam.setTranslation(Vector3D(0.0f, 0.0f, -4.0f));

	vertex vertex_list[] =
	{
		//          POSITION                      COLOR                       COLOR
		//          X   -  Y  -  Z                R  -  G  -  B               R  -  G  -  B
		// Front face
		{ Vector3D(-0.5f, -0.5f, -0.5f), Vector3D(1.0f, 0.0f, 0.0f), Vector3D(1.0f, 0.0f, 0.0f) }, // POS0
		{ Vector3D(-0.5f,  0.5f, -0.5f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f) }, // POS1
		{ Vector3D(0.5f,  0.5f, -0.5f), Vector3D(0.0f, 0.0f, 1.0f), Vector3D(0.0f, 0.0f, 1.0f) }, // POS2
		{ Vector3D(0.5f, -0.5f, -0.5f), Vector3D(1.0f, 1.0f, 0.0f), Vector3D(1.0f, 1.0f, 0.0f) }, // POS3

		// Back face
		{ Vector3D(0.5f, -0.5f,  0.5f), Vector3D(1.0f, 1.0f, 0.0f), Vector3D(1.0f, 0.0f, 0.0f) }, // POS4
		{ Vector3D(0.5f,  0.5f,  0.5f), Vector3D(0.0f, 0.0f, 1.0f), Vector3D(0.0f, 1.0f, 0.0f) }, // POS5
		{ Vector3D(-0.5f,  0.5f,  0.5f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 0.0f, 1.0f) }, // POS6
		{ Vector3D(-0.5f, -0.5f,  0.5f), Vector3D(1.0f, 0.0f, 0.0f), Vector3D(1.0f, 1.0f, 0.0f) }, // POS7
	};

	unsigned int index_list[] =
	{
		// Front side
		0, 1, 2, // First triangle
		2, 3, 0, // Second triangle
		// Back side
		4, 5, 6,
		6, 7, 4,
		// Top side
		1, 6, 5,
		5, 2, 1,
		// Bottom side
		7, 0, 3,
		3, 4, 7,
		// Right side
		3, 2, 5,
		5, 4, 3,
		// Left side
		7, 6, 1,
		1, 0, 7,
	};

	// Vertex buffer
	UINT size_vertex_list = ARRAYSIZE(vertex_list);
	void* shader_byte_code = nullptr;
	UINT size_shader = 0;

	// Vertex shader
	m_render_system->compileVertexShader(L"shaders/VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vb = m_render_system->createVertexBuffer(vertex_list, sizeof(vertex), size_vertex_list, shader_byte_code, size_shader);
	m_vs = m_render_system->createVertexShader(shader_byte_code, size_shader);
	m_render_system->releaseCompiledShader();

	// Pixel shader
	m_render_system->compilePixelShader(L"shaders/PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = m_render_system->createPixelShader(shader_byte_code, size_shader);
	m_render_system->releaseCompiledShader();

	// Index buffer
	UINT size_index_list = ARRAYSIZE(index_list);
	m_ib = m_render_system->createIndexBuffer(index_list, size_index_list);

	// Constant buffer
	constant cc;
	cc.m_time = 0;
	m_cb = m_render_system->createConstantBuffer(&cc, sizeof(constant));
}

void DX11App::Run()
{
	OnUpdate();
}

void DX11App::OnUpdate()
{
	// Clear the render target
	m_render_system->getImmediateDeviceContext()->clearRenderTargetColor(m_swap_chain, 0.2f, 0.4f, 0.8f, 1);

	// Set Viewport of render target in which we have to draw
	RECT rect = this->getClientWindowRect();
	m_render_system->getImmediateDeviceContext()->setViewportSize(rect.right - rect.left, rect.bottom - rect.top);

	this->Update();

	m_render_system->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	m_render_system->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	// Set default shader in the graphics pipeline to be able to draw 
	m_render_system->getImmediateDeviceContext()->setVertexShader(m_vs);
	m_render_system->getImmediateDeviceContext()->setPixelShader(m_ps);

	// Set the vertices of the triangle to draw
	m_render_system->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	// Set index buffer
	m_render_system->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	// Finally draw the triangle
	// GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
	m_render_system->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);

	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = (float)::GetTickCount64(); // time in milliseconds
	m_delta_time = m_old_delta ? ((m_new_delta - m_old_delta) / 1000.0f) : 0.0f;
}

void DX11App::Update()
{
	constant cc;
	cc.m_time = (unsigned int)::GetTickCount64();

	cc.m_world.setIdentity();

	Matrix4x4 temp;
	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;

	Vector3D new_pos =
		m_world_cam.getTranslation() +
		world_cam.getXDirection() * (m_right * cam_speed) +
		world_cam.getYDirection() * (m_up * cam_speed) +
		world_cam.getZDirection() * (m_forward * cam_speed);
	world_cam.setTranslation(new_pos);
	m_world_cam = world_cam;
	world_cam.inverse();

	cc.m_view = world_cam;

	float fov = 1.0f; // 1.57f;
	float aspect = width / height;
	float znear = 0.1f;
	float zfar = 100.0f;
	cc.m_proj.setPerspectiveFovLH(fov, aspect, znear, zfar);

	m_cb->update(m_render_system->getImmediateDeviceContext(), &cc);
}

RECT DX11App::getClientWindowRect()
{
	RECT rect;
	::GetClientRect(this->m_hwnd, &rect);
	return rect;
}

DX11App::~DX11App()
{
	delete m_ps;
	delete m_vs;
	delete m_cb;
	delete m_ib;
	delete m_vb;
	delete m_swap_chain;

}
