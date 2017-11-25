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

#include <Magma\Systems\Resources\TextResource.hpp>

#include <algorithm>

#include <filesystem>


using namespace Magma;

int MagmaMain(const Locator& loc)
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

	VertexShader* vertexShader = loc.renderDevice->CreateVertexShader(
		"#version 410 core\n"
		"layout (location = 0) in vec3 vertPos;\n"
		"uniform float scale;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(vertPos.x * scale, vertPos.y * scale, vertPos.z * scale, 1.0);\n"
		"}"
	);

	PixelShader* pixelShader = loc.renderDevice->CreatePixelShader(
		"#version 410 core\n"
		"out vec4 fragColor;\n"
		"void main()\n"
		"{\n"
		"   fragColor = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
		"}"
	);

	Pipeline* pipeline = loc.renderDevice->CreatePipeline(vertexShader, pixelShader);

	loc.renderDevice->DestroyVertexShader(vertexShader);
	loc.renderDevice->DestroyPixelShader(pixelShader);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	VertexBuffer *vertexBuffer = loc.renderDevice->CreateVertexBuffer(sizeof(vertices), vertices);

	VertexElement vertexElement = { 0, VertexElementType::Float, 3, 0, 0, };
	VertexDescription *vertexDescription = loc.renderDevice->CreateVertexDescription(1, &vertexElement);

	VertexArray *vertexArray = loc.renderDevice->CreateVertexArray(1, &vertexBuffer, &vertexDescription);

	float scale = 0.0f;
	bool scaleUp = true;

	while (loc.core->IsRunning() && loc.window->IsOpen())
	{
		UIEvent event;
		while (loc.window->PollEvent(event))
		{
			switch (event.type)
			{
				case UIEvent::Type::Closed:
					loc.window->Close();
					break;
			}
		}

		loc.terminal->Update();
		loc.core->Update();
		loc.input->Update(1.0f / 60.0f);

		loc.renderDevice->Clear(0.1f, 0.2f, 0.3f);

		loc.renderDevice->SetPipeline(pipeline);
		loc.renderDevice->SetVertexArray(vertexArray);
		loc.renderDevice->DrawTriangles(0, 3);

		pipeline->GetParam("scale")->SetAsFloat(scale);
		if (scaleUp)
		{
			scale += 1.0f / 60.0f;
			if (scale >= 2.0f)
				scaleUp = false;
		}
		else
		{
			scale -= 1.0f / 60.0f;
			if (scale <= 0.0f)
				scaleUp = true;
		}

		loc.window->Display();
	}

	loc.renderDevice->DestroyVertexArray(vertexArray);
	loc.renderDevice->DestroyVertexDescription(vertexDescription);
	loc.renderDevice->DestroyVertexBuffer(vertexBuffer);
	loc.renderDevice->DestroyPipeline(pipeline);

	Terminal::RemoveCommand("send");
	Terminal::RemoveCommand("exit");

	return 0;
}