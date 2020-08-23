#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "EditorLayer.h"


namespace Hazel {

	class HazelEditor : public Application
	{
	public:
		HazelEditor()
			: Application("Hazel Editor")
		{
			PushLayer(new EditorLayer());
		}

		~HazelEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		RendererAPI::SetAPI(RendererAPI::API::OpenGL);

		return new HazelEditor();
	}

}
