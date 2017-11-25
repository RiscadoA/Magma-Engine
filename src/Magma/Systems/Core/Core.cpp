#include "Core.hpp"

void Magma::Core::Update()
{
	while (MessageHandle msg = this->PopMessage())
	{
		if (msg->GetTypeName() == "exit")
		{
			m_running = false;
		}
	}
}

void Magma::Core::DerivedInit()
{
	this->Subscribe("exit");

	m_running = true;
}

void Magma::Core::DerivedTerminate()
{
	m_running = false;
}

using namespace Magma;

#include "..\Terminal\Terminal.hpp"
#include "..\Input\Input.hpp"
#include "..\Resources\ResourcesManager.hpp"

#ifdef MAGMA_IS_WINDOWS
#include "..\..\Debug\WindowsConsole.hpp"
#endif

#ifdef MAGMA_USING_DIRECTX
#include "..\..\Window\Win32Window.hpp"
int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
#else
#include "..\..\Window\GLFWWindow.hpp"
#include "..\..\Graphics\OpenGLRenderDevice.hpp"
int main()
#endif
{
#ifdef MAGMA_IS_WINDOWS
	Console::Init<WindowsConsole>();
#endif

	Locator locator;

	locator.msgBus = std::make_shared<MessageBus>(128, 512);
	locator.core = std::make_shared<Core>();
	locator.terminal = std::make_shared<Terminal>();
	locator.resourcesManager = std::make_shared<ResourcesManager>();
	locator.input = std::make_shared<Input>();

	locator.core->Init(locator.msgBus);
	locator.terminal->Init(locator.msgBus);
	// Init resources manager + load resources info
	locator.resourcesManager->Init(locator.msgBus);
	locator.resourcesManager->LoadInfo("..\resources");
	// Open window and init input system
#ifdef MAGMA_USING_DIRECTX
	locator.window = std::make_shared<Win32Window>(hInstance, nCmdShow);
	locator.window->Open();
#else
	locator.window = std::make_shared<GLFWWindow>();
	locator.window->Open();
	locator.renderDevice = std::make_shared<OpenGLRenderDevice>();
#endif
	locator.input->SetWindow(locator.window);
	locator.input->Init(locator.msgBus);

	int returnCode = MagmaMain(locator);

	locator.input->Terminate();
	locator.window->Close();
	locator.resourcesManager->Terminate();
	locator.terminal->Terminate();
	locator.core->Terminate();

	return returnCode;
}