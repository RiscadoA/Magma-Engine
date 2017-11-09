#include <iostream>

#include <Magma\Debug\WindowsConsole.hpp>
#include <Magma\Systems\MessageBus.hpp>
#include <Magma\Systems\Core\Core.hpp>
#include <Magma\Systems\Terminal\Terminal.hpp>

#include <algorithm>

#include <filesystem>

using namespace Magma;

int main(int argc, char** argv)
{
	Console::Init<WindowsConsole>();
	auto msgBus = std::make_shared<MessageBus>(64, 512);
	auto core = std::make_shared<Core>();
	auto terminal = std::make_shared<Terminal>();

	core->Init(msgBus);
	terminal->Init(msgBus);

	while (core->IsRunning())
	{
		terminal->Update();

		core->Update();
	}

	terminal->Terminate();
	core->Terminate();

	terminal = nullptr;
	core = nullptr;
	msgBus = nullptr;

	return 0;
}