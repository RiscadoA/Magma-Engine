#include <iostream>
#include <fstream>

#include <Magma\Debug\WindowsConsole.hpp>
#include <Magma\Systems\MessageBus.hpp>
#include <Magma\Systems\Core\Core.hpp>
#include <Magma\Systems\Terminal\Terminal.hpp>
#include <Magma\Systems\Input\Input.hpp>
#include <Magma\Systems\Resources\ResourcesManager.hpp>
#include <Magma\Utils\Globals.hpp>
#include <Magma\Window\GLFWWindow.hpp>
#include <Magma\Graphics\RenderDevice.hpp>
#include <Magma\Systems\Scene\Scene.hpp>

#include <Magma\Systems\Resources\TextResource.hpp>

#include <algorithm>

#include <filesystem>


using namespace Magma;

void MagmaInit(const Locator& loc)
{
	Terminal::AddCommand("exit", [&loc](const std::vector<std::string>& arguments) { loc.core->Terminate(); });
	Terminal::AddCommand("send", [&loc](const std::vector<std::string>& arguments)
	{
		if (arguments.size() == 1)
		{
			loc.msgBus->SendMessage(arguments[0], "empty", std::cin);
		}
		else if (arguments.size() == 2)
		{
			loc.msgBus->SendMessage(arguments[0], arguments[1], std::cin);
		}
		else MAGMA_WARNING("Failed to execute command \"send\", invalid number of arguments, 1/2 expected (message type, data type {default empty})");
	});

	{
		std::ifstream ifs("keybinds.xml");
		if (ifs.is_open())
		{
			ifs >> *loc.input;
		}
		ifs.close();
	}

	(*loc.input)["Horizontal"].SetNegativeKey(Keyboard::Key::A);
	(*loc.input)["Horizontal"].SetPositiveKey(Keyboard::Key::D);
	(*loc.input)["Horizontal"].SetSpeed(1.5f);

	(*loc.input)["Vertical"].SetNegativeKey(Keyboard::Key::W);
	(*loc.input)["Vertical"].SetPositiveKey(Keyboard::Key::S);
	(*loc.input)["Vertical"].SetSpeed(1.5f);
}

void MagmaTerminate(const Locator& loc)
{
	Terminal::RemoveCommand("send");
	Terminal::RemoveCommand("exit");
}

void MagmaUpdate(const Locator& loc, float deltaTime)
{

}