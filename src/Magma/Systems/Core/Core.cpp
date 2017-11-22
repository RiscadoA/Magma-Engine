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
