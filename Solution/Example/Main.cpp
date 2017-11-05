#include <iostream>

#include <Magma\Debug\WindowsConsole.hpp>
#include <Magma\Utils\Math.hpp>
#include <Magma\Systems\MessageBus.hpp>

#include <algorithm>

#include <filesystem>

using namespace Magma;

class IntMessage : public Message
{
public:
	IntMessage(int value) : m_value(value) {}

	int m_value;
};

class Listener : public MessageListener
{
public:
	void Update()
	{
		while (MessageHandle msg = PopMessage())
		{
			if (msg->GetTypeName() == "ShowInt")
			{
				std::cout << "Int showed: " << msg->As<IntMessage>().m_value << std::endl;
			}
		}
	}

	// Inherited via MessageListener
	virtual void DerivedInit() override
	{
		this->SubscribeToAll();
	}

	virtual void DerivedTerminate() override
	{
		
	}
};

int main(int argc, char** argv)
{
	Console::Init<WindowsConsole>();

	auto msgBus = std::make_shared<MessageBus>(64, 512);
	auto listener = std::make_shared<Listener>();
	listener->Init(msgBus);

	msgBus->SendMessage<IntMessage>("ShowInt", 2);
	listener->Update();

	while (true)
	{
		std::string str;
		std::getline(std::cin, str);
		if (str.empty())
			continue;
		if (str == "exit")
			break;
		else
			MAGMA_WARNING("Unknown command \"" + str + "\"");
		std::cout << "Received \"" << str << "\"" << std::endl;
	}

	listener->Terminate();
	listener = nullptr;
	msgBus = nullptr;

	return 0;
}