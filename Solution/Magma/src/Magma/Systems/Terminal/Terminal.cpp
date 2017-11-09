#include "Terminal.hpp"

#include "..\..\Debug\Console.hpp"

#include <regex>
#include <string>

void Magma::Terminal::Update()
{
	while (MessageHandle msg = PopMessage())
	{
		std::cout << "Message of type {\"" << msg->GetTypeName() << "\"} sent, data: {" << *msg << "};" << std::endl;
	}
}

void Magma::Terminal::OnInput(const std::string& in)
{
	//std::regex regex("\\s?(\\w+)|\"(.*)\"\\s?");
	std::regex regex(R"(\s?(\w+)\s?|"([^"]+)\"\s?)");
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

	auto SendMsg = [this](std::string type, std::string dataType) {
		std::thread t([this, type, dataType]()
		{
			m_msgBus->SendMessage(type, dataType, std::cin);
		});
		t.detach();
	};

	if (command[0] == "exit")
		SendMsg("exit", "empty");
	else if (command[0] == "send" && command.size() == 2)
		SendMsg(command[1], "empty");
	else if (command[0] == "send" && command.size() == 3)
		SendMsg(command[1], command[2]);
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
