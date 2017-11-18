#include <iostream>
#include <fstream>

#include <Magma\Debug\WindowsConsole.hpp>
#include <Magma\Systems\MessageBus.hpp>
#include <Magma\Systems\Core\Core.hpp>
#include <Magma\Systems\Terminal\Terminal.hpp>
#include <Magma\Utils\Globals.hpp>
#include <Magma\Window\GLFWWindow.hpp>

#include <algorithm>

#include <filesystem>

using namespace Magma;

int main(int argc, char** argv)
{
	Console::Init<WindowsConsole>();
	auto msgBus = std::make_shared<MessageBus>(128, 512);
	auto core = std::make_shared<Core>();
	auto terminal = std::make_shared<Terminal>();

	core->Init(msgBus);
	terminal->Init(msgBus);

	Terminal::AddCommand("exit", [core](const std::vector<std::string>& arguments) { core->Terminate(); });
	Terminal::AddCommand("send", [msgBus](const std::vector<std::string>& arguments)
	{
		if (arguments.size() == 1)
		{
			msgBus->SendMessage(arguments[0], "empty", std::cin);
		}
		else if (arguments.size() == 2)
		{
			msgBus->SendMessage(arguments[0], arguments[1], std::cin);
		}
		else MAGMA_WARNING("Failed to execute command \"send\", invalid number of arguments, 1/2 expected (message type, data type {default empty})");
	});

	auto window = std::shared_ptr<Window>(new GLFWWindow());

	{
		std::ifstream ifs("config");
		if (ifs.is_open())
		{
			ifs >> *window;
		}
		ifs.close();
	}

	window->Open();
	window->SetVSyncEnabled(true);

	while (core->IsRunning() && window->IsOpen())
	{
		UIEvent event;
		while (window->PollEvent(event))
		{
			switch (event.type)
			{
				case UIEvent::Type::Closed:
					window->Close();
					break;
			}
			std::cout << event << std::endl;
		}

		terminal->Update();
		core->Update();

		window->Display();
	}
	
	{
		std::ofstream ofs("config");
		if (ofs.is_open())
		{
			ofs << *window << std::endl;
		}
		ofs.close();
	}

	window->Close();
	window = nullptr;

	Terminal::RemoveCommand("send");
	Terminal::RemoveCommand("exit");

	terminal->Terminate();
	core->Terminate();

	terminal = nullptr;
	core = nullptr;
	msgBus = nullptr;

	return 0;
}