#include "Input.hpp"

#include "..\..\Utils\XMLDocument.hpp"

#include <sstream>

void Magma::Input::SetWindow(std::shared_ptr<Magma::Window> window)
{
	m_window = window;
}

void Magma::Input::Update(float deltaTime)
{
	std::lock_guard<std::mutex> lockGuard(m_inputAxes_mutex);
	if (m_window == nullptr)
		MAGMA_ERROR("Input system needs a window to be updated");
	else
		for (auto& p : m_inputAxes)
		{
			auto& inputAxis = p.second;
			std::lock_guard<std::mutex> inputAxisLockGuard(inputAxis.m_mutex);

			bool changed = false;

			if (m_window->IsKeyboardKeyPressed(inputAxis.m_positiveKey))
			{
				changed = true;
				inputAxis.m_value = glm::mix(inputAxis.m_value, 1.0f, glm::clamp(0.5f * deltaTime * inputAxis.m_speed * 25, 0.0f, 1.0f));
			}
			if (m_window->IsKeyboardKeyPressed(inputAxis.m_negativeKey))
			{
				changed = true;
				inputAxis.m_value = glm::mix(inputAxis.m_value, -1.0f, glm::clamp(0.5f * deltaTime * inputAxis.m_speed * 25, 0.0f, 1.0f));
			}

			if (m_window->IsMouseButtonPressed(inputAxis.m_positiveMouseButton))
			{
				changed = true;
				inputAxis.m_value = glm::mix(inputAxis.m_value, 1.0f, glm::clamp(0.5f * deltaTime * inputAxis.m_speed * 25, 0.0f, 1.0f));
			}
			if (m_window->IsMouseButtonPressed(inputAxis.m_negativeMouseButton))
			{
				changed = true;
				inputAxis.m_value = glm::mix(inputAxis.m_value, -1.0f, glm::clamp(0.5f * deltaTime * inputAxis.m_speed * 25, 0.0f, 1.0f));
			}

			if (!changed)
				inputAxis.m_value = glm::mix(inputAxis.m_value, 0.0f, glm::clamp(0.5f * deltaTime * inputAxis.m_decaySpeed * 25, 0.0f, 1.0f));

			if (abs(inputAxis.m_value) < (1.0f / 1000.0f))
				inputAxis.m_value = 0.0f;
		}
}

Magma::InputAxis & Magma::Input::operator[](const std::string & axisName)
{
	std::lock_guard<std::mutex> lockGuard(m_inputAxes_mutex);
	return m_inputAxes[axisName];
}

void Magma::Input::DerivedInit()
{
	
}

void Magma::Input::DerivedTerminate()
{

}

void Magma::Input::Serialize(std::ostream & stream) const
{
	std::lock_guard<std::mutex> lockGuard(m_inputAxes_mutex);

	XMLDocument doc;
	auto root = doc.GetRoot();
	root->SetName("input");

	auto axes = root->AddChild("axes");
	for (auto& i : m_inputAxes)
	{
		auto axis = axes->AddChild("axis");
		axis->SetAttribute("name", i.first);
		std::stringstream ss;
		ss << i.second;
		axis->SetText(ss.str());
	}
	
	stream << doc;
}

void Magma::Input::Deserialize(std::istream & stream)
{
	std::lock_guard<std::mutex> lockGuard(m_inputAxes_mutex);

	XMLDocument doc;
	stream >> doc;

	auto root = doc.GetRoot();
	auto axes = root->GetChild("axes");
	for (auto& c : axes->GetChildren("axis"))
	{
		std::stringstream ss(c->GetText());
		ss >> m_inputAxes[c->GetAttribute("name")];
	}
}

void Magma::InputAxis::Serialize(std::ostream & stream) const
{
	stream << m_speed << " ";
	stream << m_decaySpeed << " ";
	stream << static_cast<int32_t>(m_positiveMouseButton) << " ";
	stream << static_cast<int32_t>(m_negativeMouseButton) << " ";
	stream << static_cast<int32_t>(m_positiveKey) << " ";
	stream << static_cast<int32_t>(m_negativeKey);
}

void Magma::InputAxis::Deserialize(std::istream & stream)
{
	stream >> m_speed;
	stream >> m_decaySpeed;
	stream >> reinterpret_cast<int32_t&>(m_positiveMouseButton);
	stream >> reinterpret_cast<int32_t&>(m_negativeMouseButton);
	stream >> reinterpret_cast<int32_t&>(m_positiveKey);
	stream >> reinterpret_cast<int32_t&>(m_negativeKey);
}
