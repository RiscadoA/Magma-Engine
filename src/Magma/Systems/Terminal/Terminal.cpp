#include "Terminal.hpp"

#include "..\..\Debug\Console.hpp"
#include "..\..\Utils\Globals.hpp"

#include <regex>
#include <string>

std::map<std::string, std::function<void(const std::vector<std::string>&)>> Magma::Terminal::s_commands = {};
std::mutex Magma::Terminal::s_commands_mutex;

void Magma::Terminal::Update()
{
	while (MessageHandle msg = PopMessage())
	{
		std::cout << "Message of type {\"" << msg->GetTypeName() << "\"} sent, data: {" << *msg << "};" << std::endl;
	}
}

void Magma::Terminal::AddCommand(const std::string & name, std::function<void(const std::vector<std::string>&)> command)
{
	std::lock_guard<std::mutex> lockGuard(s_commands_mutex);
	if (!s_commands.insert({ name, command }).second)
		MAGMA_WARNING("Failed to add command \"" + name + "\", command with this name already exists");
}

void Magma::Terminal::RemoveCommand(const std::string & name)
{
	std::lock_guard<std::mutex> lockGuard(s_commands_mutex);
	s_commands.erase(name);
}

void Magma::Terminal::ExecuteCommand(const std::string & name, const std::vector<std::string>& parameters)
{
	std::lock_guard<std::mutex> lockGuard(s_commands_mutex);
	auto i = s_commands.find(name);
	if (i == s_commands.end())
		MAGMA_WARNING("Failed to execute command \"" + name + "\", no command found with this name");
	else
		i->second(parameters);
}

void Magma::Terminal::OnInput(const std::string& in)
{
	std::regex regex(R"(\s?(\S+)\s?|"([^"]+)\"\s?)");
	std::sregex_iterator iter(in.begin(), in.end(), regex);
	std::sregex_iterator end;

	std::vector<std::string> command;

	while (iter != end)
	{
		if ((*iter)[2].str().empty())
			command.push_back((*iter)[1]);
		else
			command.push_back((*iter)[2]);
		++iter;
	}

	if (command.empty())
		return;

	if (command[0][0] == '/')
	{
		command[0].erase(command[0].begin());
		std::string cmd = command[0];
		command.erase(command.begin());
		std::thread t(ExecuteCommand, cmd, command);
		t.detach();
	}

	/*auto SendMsg = [this](std::string type, std::string dataType) {
		std::thread t([this, type, dataType]()
		{
			m_msgBus->SendMessage(type, dataType, std::cin);
		});
		t.detach();
	};

	auto ExecuteInThread = [](auto l)
	{
		std::thread t(l);
		t.detach();
	};*/



	/*if (command[0] == "exit")
		SendMsg("exit", "empty");
	else if (command[0] == "send" && command.size() == 2)
		SendMsg(command[1], "empty");
	else if (command[0] == "send" && command.size() == 3)
		SendMsg(command[1], command[2]);
	else if (command[0] == "set" && command.size() == 2)
		ExecuteInThread([this, command]() { std::cin >> Globals::Get(command[1]); });
	else if (command[0] == "get" && command.size() == 2)
		ExecuteInThread([this, command]() { std::cout << Globals::Get(command[1]); });
	else
		MAGMA_WARNING("Unknown command \"" + command[0] + "\"!");*/
}

void Magma::Terminal::DerivedInit()
{
	this->SubscribeToAll();

	m_iCallback = Console::AddInputCallback(std::bind(&Terminal::OnInput, this, std::placeholders::_1));
}

void Magma::Terminal::DerivedTerminate()
{
	Console::RemoveInputCallback(m_iCallback);
}
