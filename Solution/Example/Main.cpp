#include <iostream>
#include <fstream>

#include <Magma\Debug\WindowsConsole.hpp>
#include <Magma\Systems\MessageBus.hpp>
#include <Magma\Systems\Core\Core.hpp>
#include <Magma\Systems\Terminal\Terminal.hpp>
#include <Magma\Systems\Input\Input.hpp>
#include <Magma\Utils\Globals.hpp>
#include <Magma\Window\GLFWWindow.hpp>

#include <algorithm>

#include <filesystem>

#include <GL\glew.h>

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
	auto input = std::make_shared<Input>();
	input->SetWindow(window);
	input->Init(msgBus);

	glewInit();

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

	{
		std::ifstream ifs("keybinds.xml");
		if (ifs.is_open())
		{
			ifs >> *input;
		}
		ifs.close();
	}

	(*input)["Horizontal"].SetNegativeKey(Keyboard::Key::A);
	(*input)["Horizontal"].SetPositiveKey(Keyboard::Key::D);
	(*input)["Horizontal"].SetSpeed(1.5f);

	(*input)["Vertical"].SetNegativeKey(Keyboard::Key::W);
	(*input)["Vertical"].SetPositiveKey(Keyboard::Key::S);
	(*input)["Vertical"].SetSpeed(1.5f);

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
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_QUADS);
		if ((*input)["Horizontal"].Value() < 0.0f)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if ((*input)["Horizontal"].Value() > 0.0f)
			glColor3f(0.0f, 0.0f, 1.0f);
		else
			glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(-1.0f, 0.5f);
		glVertex2f((*input)["Horizontal"].Value(), 0.5f);
		glVertex2f((*input)["Horizontal"].Value(), 1.0f);
		glVertex2f(-1.0f, 1.0f);
		glEnd();

		terminal->Update();
		core->Update();
		input->Update(1.0f / 60.0f);

		window->Display();
	}
	
	{
		std::ofstream ofs("keybinds.xml");
		if (ofs.is_open())
		{
			ofs << *input << std::endl;
		}
		ofs.close();
	}

	{
		std::ofstream ofs("config");
		if (ofs.is_open())
		{
			ofs << *window << std::endl;
		}
		ofs.close();
	}

	input->Terminate();
	input = nullptr;
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