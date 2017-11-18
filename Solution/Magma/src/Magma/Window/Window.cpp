#include "Window.hpp"

Magma::Window* Magma::Window::s_activeWindow = nullptr;

Magma::Window::Window()
{
	// Set default settings
	m_windowSize = glm::uvec2(640, 480);
	m_windowTitle = "Window";
	m_windowMode = Window::Mode::Windowed;
	m_windowContextSettings = Window::ContextSettings::Default;
}

Magma::Window::~Window()
{
	if (s_activeWindow == this)
		s_activeWindow = nullptr;
}

bool Magma::Window::IsOpen()
{
	return this->DerivedIsOpen();
}

void Magma::Window::Open(glm::uvec2 windowSize, const std::string & windowTitle, Window::Mode windowMode, const Window::ContextSettings& windowContextSettings)
{
	m_windowSize = windowSize;
	m_windowTitle = windowTitle;
	m_windowMode = windowMode;
	m_windowContextSettings = windowContextSettings;

	this->DerivedOpen();
	this->MakeActive();
}

void Magma::Window::Open()
{
	this->DerivedOpen();
	this->MakeActive();
}

void Magma::Window::Close()
{
	this->DerivedClose();
}

void Magma::Window::Resize(glm::uvec2 windowSize)
{
	m_windowSize = windowSize;

	this->DerivedResize();
}

void Magma::Window::SetTitle(const std::string & windowTitle)
{
	m_windowTitle = windowTitle;

	this->DerivedSetTitle();
}

void Magma::Window::SetMode(Window::Mode windowMode)
{
	m_windowMode = windowMode;

	this->DerivedSetMode();
}

Magma::Window::ContextSettings Magma::Window::ContextSettings::Default = Magma::Window::ContextSettings();

Magma::Window::ContextSettings::ContextSettings()
{
	depthBits = 0;
	stencilBits = 0;
	antialiasingLevel = 2;
	majorVersion = 4;
	minorVersion = 0;
}

void Magma::Window::ContextSettings::Serialize(std::ostream & stream) const
{
	stream << depthBits << std::endl;
	stream << stencilBits << std::endl;
	stream << antialiasingLevel << std::endl;
	stream << majorVersion << std::endl;
	stream << minorVersion;
}

void Magma::Window::ContextSettings::Deserialize(std::istream & stream)
{

}

bool Magma::Window::PollEvent(Magma::UIEvent & event)
{
	return this->DerivedPollEvent(event);
}

void Magma::Window::Display()
{
	this->DerivedDisplay();
}

void Magma::Window::MakeActive()
{
	s_activeWindow = this;
	this->DerivedMakeActive();
}

void Magma::Window::Serialize(std::ostream & stream) const
{
	stream << m_windowSize.x << " " << m_windowSize.y << std::endl;
	stream << m_windowTitle << std::endl;
	stream << (int32_t)m_windowMode << std::endl;
	stream << m_windowContextSettings;
}

void Magma::Window::Deserialize(std::istream & stream)
{
	bool wasOpen = this->IsOpen();
	if (wasOpen)
		this->Close();

	stream >> m_windowSize.x >> m_windowSize.y;
	stream.ignore(64, L'\n');
	std::getline(stream, m_windowTitle);
	stream >> reinterpret_cast<int32_t&>(m_windowMode);
	stream >> m_windowContextSettings;

	if (wasOpen)
		this->Open();
}
