#include "hzpch.h"

#include "RendererAPI.h"
#include "Hazel/Platform/OpenGL/OpenGLRendererAPI.h"
#include "Hazel/Platform/Vulkan/VulkanRendererAPI.h"
#include "Hazel/Platform/DX11/DX11RendererAPI.h"


namespace Hazel
{

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	RendererAPI* RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:    HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLRendererAPI();
		case RendererAPI::API::Vulkan:  return new VulkanRendererAPI();
		case RendererAPI::API::DX11:    return new DX11RendererAPI();
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
