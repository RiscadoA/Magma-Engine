#include "UIEvent.hpp"

void Magma::UIEvent::Serialize(std::ostream & stream) const
{
	stream << (int32_t)type;
	switch (type)
	{
	case UIEvent::Type::KeyPressed:
	case UIEvent::Type::KeyReleased:
		stream << " " << (int32_t)keyPressed.code << " " << keyPressed.shift << " " << keyPressed.control << " " << keyPressed.alt;
		break;

	case UIEvent::Type::MouseButtonPressed:
	case UIEvent::Type::MouseButtonReleased:
		stream << " " << (int32_t)mouseButton.button << " " << mouseButton.x << " " << mouseButton.y;
		break;

	case UIEvent::Type::MouseMoved:
		stream << " " << mouseMoved.x << " " << mouseMoved.y;
		break;

	case UIEvent::Type::MouseWheelScrolled:
		stream << " " << (int32_t)mouseWheelScrolled.wheel << " " << mouseWheelScrolled.delta << " " << mouseWheelScrolled.x << " " << mouseWheelScrolled.y;
		break;

	case UIEvent::Type::Resized:
		stream << " " << resized.width << " " << resized.height;
		break;

	case UIEvent::Type::TextEntered:
		stream << " " << textEntered.unicode;
		break;
	}
}

void Magma::UIEvent::Deserialize(std::istream & stream)
{
	stream >> reinterpret_cast<int32_t&>(type);
	switch (type)
	{
		switch (type)
		{
		case UIEvent::Type::KeyPressed:
		case UIEvent::Type::KeyReleased:
			stream >> reinterpret_cast<int32_t&>(keyPressed.code) >> keyPressed.shift >> keyPressed.control >> keyPressed.alt;
			break;

		case UIEvent::Type::MouseButtonPressed:
		case UIEvent::Type::MouseButtonReleased:
			stream >> reinterpret_cast<int32_t&>(mouseButton.button) >> mouseButton.x >> mouseButton.y;
			break;

		case UIEvent::Type::MouseMoved:
			stream >> mouseMoved.x >> mouseMoved.y;
			break;

		case UIEvent::Type::MouseWheelScrolled:
			stream >> reinterpret_cast<int32_t&>(mouseWheelScrolled.wheel) >> mouseWheelScrolled.delta >> mouseWheelScrolled.x >> mouseWheelScrolled.y;
			break;

		case UIEvent::Type::Resized:
			stream >> resized.width >> resized.height;
			break;

		case UIEvent::Type::TextEntered:
			stream >> textEntered.unicode;
			break;
		}
	}
}
