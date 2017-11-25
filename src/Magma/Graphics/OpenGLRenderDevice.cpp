#include "OpenGLRenderDevice.hpp"
#include "..\Utils\Utils.hpp"

#include <map>
#include <iostream>

#include <gl\glew.h>

using namespace Magma;

namespace Magma
{
	class OpenGLVertexShader : public VertexShader
	{
	public:
		OpenGLVertexShader(const char* code)
		{
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &code, NULL);
			glCompileShader(vertexShader);

			int success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				MAGMA_ERROR("Vertex shader compilation failed: " + std::string(infoLog));
			}
		}

		virtual ~OpenGLVertexShader() override
		{
			glDeleteShader(vertexShader);
		}

		int vertexShader = 0;
	};

	class OpenGLPixelShader : public PixelShader
	{
	public:
		OpenGLPixelShader(const char* code)
		{
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &code, NULL);
			glCompileShader(fragmentShader);

			int success;
			char infoLog[512];
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				MAGMA_ERROR("Pixel shader compilation failed: " + std::string(infoLog));
			}
		}

		virtual ~OpenGLPixelShader() override
		{
			glDeleteShader(fragmentShader);
		}

		int fragmentShader = 0;
	};

	class OpenGLPipelineParam;

	class OpenGLPipeline : public Pipeline
	{
	public:
		OpenGLPipeline(OpenGLVertexShader *vertexShader, OpenGLPixelShader *pixelShader)
		{
			// Attach shaders and link them
			shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader->vertexShader);
			glAttachShader(shaderProgram, pixelShader->fragmentShader);
			glLinkProgram(shaderProgram);

			// Check if there were errors when linking
			int success;
			char infoLog[512];
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
				MAGMA_ERROR("Pipeline linking failed: " + std::string(infoLog));
			}
		}

		virtual ~OpenGLPipeline() override
		{
			glDeleteProgram(shaderProgram);
		}

		PipelineParam* GetParam(const char* name) override;

		int shaderProgram = 0;
		std::map<std::string, OpenGLPipelineParam*> params;
	};

	class OpenGLPipelineParam : public PipelineParam
	{
	public:
		OpenGLPipelineParam(OpenGLPipeline* _pipeline, int _location) : pipeline(_pipeline), location(_location) {}

		virtual void SetAsInt(int value) override
		{
			glUseProgram(pipeline->shaderProgram);
			glUniform1i(location, value);
		}

		virtual void SetAsFloat(float value) override
		{
			glUseProgram(pipeline->shaderProgram);
			glUniform1f(location, value);
		}

		virtual void SetAsMat4(const float * value) override
		{
			glUseProgram(pipeline->shaderProgram);
			glUniformMatrix4fv(location, 1, GL_FALSE, value);
		}

		virtual void SetAsIntArray(int count, const int * values) override
		{
			glUseProgram(pipeline->shaderProgram);
			glUniform1iv(location, count, values);
		}

		virtual void SetAsFloatArray(int count, const float * values) override
		{
			glUseProgram(pipeline->shaderProgram);
			glUniform1fv(location, count, values);
		}

		virtual void SetAsMat4Array(int count, const float * values) override
		{
			glUseProgram(pipeline->shaderProgram);
			glUniformMatrix4fv(location, count, GL_FALSE, values);
		}

		OpenGLPipeline* pipeline;
		int location;
	};

	PipelineParam * OpenGLPipeline::GetParam(const char * name)
	{
		auto it = params.find(name);
		if (it == params.end())
		{
			int location = glGetUniformLocation(shaderProgram, name);
			if (location < 0)
			{
				MAGMA_WARNING("Failed to get param from pipeline, no param with this name");
				return nullptr;
			}
			OpenGLPipelineParam* param = new OpenGLPipelineParam
			(this, location);
			params.insert(it, std::make_pair(name, param));
			return param;
		}
		else
		{
			return it->second;
		}
	}

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:

		OpenGLVertexBuffer(long long size, const void *data)
		{
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			// Assume static vertex buffer for now
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}

		virtual ~OpenGLVertexBuffer() override
		{
			glDeleteBuffers(1, &vbo);
		}

		unsigned int vbo = 0;
	};

	class OpenGLVertexDescription : public VertexDescription
	{
	public:
		struct OpenGLVertexElement
		{
			GLuint index;
			GLint size;
			GLenum type;
			GLboolean normalized;
			GLsizei stride;
			const GLvoid *pointer;
		};

		OpenGLVertexDescription(unsigned int _numVertexElements, const VertexElement* vertexElements) : numVertexElements(_numVertexElements)
		{
			// Used to convert from our enum types to OpenGL types
			static GLenum toOpenGLType[] = { GL_BYTE, GL_SHORT, GL_INT, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT,
				GL_BYTE, GL_SHORT, GL_INT, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT, GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE };
			static GLboolean toOpenGLNormalized[] = { GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE,
				GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE };

			openGLVertexElements = new OpenGLVertexElement[numVertexElements];
			for (unsigned int i = 0; i < numVertexElements; i++)
			{
				openGLVertexElements[i].index = vertexElements[i].index;
				openGLVertexElements[i].size = vertexElements[i].size;
				openGLVertexElements[i].type = toOpenGLType[static_cast<size_t>(vertexElements[i].type)];
				openGLVertexElements[i].normalized = toOpenGLNormalized[static_cast<size_t>(vertexElements[i].type)];
				openGLVertexElements[i].stride = vertexElements[i].stride;
				openGLVertexElements[i].pointer = (char *)nullptr + vertexElements[i].offset;
			}
		}
		
		virtual ~OpenGLVertexDescription() override
		{
			delete[] openGLVertexElements;
		}

		unsigned int numVertexElements = 0;
		OpenGLVertexElement *openGLVertexElements = nullptr;
	};

	class OpenGLVertexArray : public VertexArray
	{
	public:

		OpenGLVertexArray(unsigned int numVertexBuffers, VertexBuffer **vertexBuffers, VertexDescription **vertexDescriptions)
		{
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			for (unsigned int i = 0; i < numVertexBuffers; i++)
			{
				OpenGLVertexBuffer *vertexBuffer = reinterpret_cast<OpenGLVertexBuffer *>(vertexBuffers[i]);
				OpenGLVertexDescription *vertexDescription = reinterpret_cast<OpenGLVertexDescription *>(vertexDescriptions[i]);

				glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->vbo);

				for (unsigned int j = 0; j < vertexDescription->numVertexElements; j++)
				{
					glEnableVertexAttribArray(vertexDescription->openGLVertexElements[j].index);
					glVertexAttribPointer(vertexDescription->openGLVertexElements[j].index,
										  vertexDescription->openGLVertexElements[j].size,
										  vertexDescription->openGLVertexElements[j].type,
										  vertexDescription->openGLVertexElements[j].normalized,
										  vertexDescription->openGLVertexElements[j].stride,
										  vertexDescription->openGLVertexElements[j].pointer);
				}
			}
		}

		virtual ~OpenGLVertexArray() override
		{
			glDeleteVertexArrays(1, &vao);
		}

		unsigned int vao = 0;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:

		OpenGLIndexBuffer(long long size, const void *data)
		{
			glGenBuffers(1, &ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			// Assume static index buffer for now
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}

		virtual ~OpenGLIndexBuffer() override
		{
			glDeleteBuffers(1, &ibo);
		}

		unsigned int ibo = 0;
	};

	class OpenGLTexture2D : public Texture2D
	{
	public:

		OpenGLTexture2D(int width, int height, const void *data = nullptr)
		{
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		virtual ~OpenGLTexture2D() override
		{
			glDeleteTextures(1, &texture);
		}

		unsigned int texture = 0;
	};

	class OpenGLRasterState : public RasterState
	{
	public:

		OpenGLRasterState(bool _cullEnabled = true, Winding _frontFace = Winding::CCW, Face _cullFace = Face::Back, RasterMode _rasterMode = RasterMode::Fill)
		{
			static const GLenum toOpenGLFrontFace[] = { GL_CW, GL_CCW };
			static const GLenum toOpenGLCullFace[] = { GL_FRONT, GL_BACK, GL_FRONT_AND_BACK };
			static const GLenum toOpenGLRasterMode[] = { GL_POINT, GL_LINE, GL_FILL };

			cullEnabled = _cullEnabled;
			frontFace = toOpenGLFrontFace[static_cast<size_t>(_frontFace)];
			cullFace = toOpenGLCullFace[static_cast<size_t>(_cullFace)];
			polygonMode = toOpenGLRasterMode[static_cast<size_t>(_rasterMode)];
		}

		bool cullEnabled;
		GLenum frontFace;
		GLenum cullFace;
		GLenum polygonMode;
	};

	class OpenGLDepthStencilState : public DepthStencilState
	{
	public:

		OpenGLDepthStencilState(
			bool			_depthEnabled = true,
			bool			_depthWriteEnabled = true,
			float			_depthNear = 0,
			float			_depthFar = 1,
			Compare			_depthCompare = Compare::Less,

			bool			_frontFaceStencilEnabled = false,
			Compare			_frontFaceStencilCompare = Compare::Always,
			StencilAction	_frontFaceStencilFail = StencilAction::Keep,
			StencilAction	_frontFaceStencilPass = StencilAction::Keep,
			StencilAction	_frontFaceDepthFail = StencilAction::Keep,
			int				_frontFaceRef = 0,
			unsigned int	_frontFaceReadMask = 0xFFFFFFFF,
			unsigned int	_frontFaceWriteMask = 0xFFFFFFFF,

			bool			_backFaceStencilEnabled = false,
			Compare			_backFaceStencilCompare = Compare::Always,
			StencilAction	_backFaceStencilFail = StencilAction::Keep,
			StencilAction	_backFaceStencilPass = StencilAction::Keep,
			StencilAction	_backFaceDepthFail = StencilAction::Keep,
			int				_backFaceRef = 0,
			unsigned int	_backFaceReadMask = 0xFFFFFFFF,
			unsigned int	_backFaceWriteMask = 0xFFFFFFFF)

		{
			static const GLenum toOpenGLCompare[] = { GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS };
			static const GLenum toOpenGLStencil[] = { GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT };

			depthEnabled = _depthEnabled;
			depthWriteEnabled = _depthWriteEnabled;
			depthNear = _depthNear;
			depthFar = _depthFar;
			depthFunc = toOpenGLCompare[static_cast<size_t>(_depthCompare)];

			frontFaceStencilEnabled = _frontFaceStencilEnabled;
			frontStencilFunc = toOpenGLCompare[static_cast<size_t>(_frontFaceStencilCompare)];
			frontFaceStencilFail = toOpenGLStencil[static_cast<size_t>(_frontFaceStencilFail)];
			frontFaceStencilPass = toOpenGLStencil[static_cast<size_t>(_frontFaceStencilPass)];
			frontFaceDepthFail = toOpenGLStencil[static_cast<size_t>(_frontFaceDepthFail)];
			frontFaceRef = _frontFaceRef;
			frontFaceReadMask = _frontFaceReadMask;
			frontFaceWriteMask = _frontFaceWriteMask;

			backFaceStencilEnabled = _backFaceStencilEnabled;
			backStencilFunc = toOpenGLCompare[static_cast<size_t>(_backFaceStencilCompare)];
			backFaceStencilFail = toOpenGLStencil[static_cast<size_t>(_backFaceStencilFail)];
			backFaceStencilPass = toOpenGLStencil[static_cast<size_t>(_backFaceStencilPass)];
			backFaceDepthFail = toOpenGLStencil[static_cast<size_t>(_backFaceDepthFail)];
			backFaceRef = _backFaceRef;
			backFaceReadMask = _backFaceReadMask;
			backFaceWriteMask = _backFaceWriteMask;
		}


		bool depthEnabled;
		bool depthWriteEnabled;
		float depthNear;
		float depthFar;
		GLenum depthFunc;

		bool frontFaceStencilEnabled;
		GLenum	frontStencilFunc;
		GLenum frontFaceStencilFail;
		GLenum frontFaceStencilPass;
		GLenum frontFaceDepthFail;
		GLint frontFaceRef;
		GLuint frontFaceReadMask;
		GLuint frontFaceWriteMask;

		bool backFaceStencilEnabled;
		GLenum backStencilFunc;
		GLenum backFaceStencilFail;
		GLenum backFaceStencilPass;
		GLenum backFaceDepthFail;
		GLint backFaceRef;
		GLuint backFaceReadMask;
		GLuint backFaceWriteMask;
	};
}

Magma::OpenGLRenderDevice::OpenGLRenderDevice()
{
	// Init glew
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		MAGMA_ERROR("Failed to init GLEW, error: " + std::string(reinterpret_cast<const char*>(glewGetErrorString(err))));
	}
	else
	{
		std::cout << "GLEW successfully inited. Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
	}

	// Set default raster state
	m_defaultRasterState = dynamic_cast<OpenGLRasterState*>(CreateRasterState());
	SetRasterState(m_defaultRasterState);

	// Set default depth/stencil state
	m_defaultDepthStencilState = dynamic_cast<OpenGLDepthStencilState*>(CreateDepthStencilState());
	SetDepthStencilState(m_defaultDepthStencilState);
}

VertexShader * Magma::OpenGLRenderDevice::CreateVertexShader(const char * code)
{
	return new OpenGLVertexShader(code);
}

void Magma::OpenGLRenderDevice::DestroyVertexShader(VertexShader * vertexShader)
{
	delete vertexShader;
}

PixelShader * Magma::OpenGLRenderDevice::CreatePixelShader(const char * code)
{
	return new OpenGLPixelShader(code);
}

void Magma::OpenGLRenderDevice::DestroyPixelShader(PixelShader * pixelShader)
{
	delete pixelShader;
}

Pipeline * Magma::OpenGLRenderDevice::CreatePipeline(VertexShader * vertexShader, PixelShader * pixelShader)
{
	return new OpenGLPipeline(reinterpret_cast<OpenGLVertexShader*>(vertexShader), reinterpret_cast<OpenGLPixelShader*>(pixelShader));
}

void Magma::OpenGLRenderDevice::DestroyPipeline(Pipeline * pipeline)
{
	delete pipeline;
}

void Magma::OpenGLRenderDevice::SetPipeline(Pipeline * pipeline)
{
	glUseProgram(reinterpret_cast<OpenGLPipeline*>(pipeline)->shaderProgram);
}

VertexBuffer * Magma::OpenGLRenderDevice::CreateVertexBuffer(long long size, const void * data)
{
	return new OpenGLVertexBuffer(size, data);
}

void Magma::OpenGLRenderDevice::DestroyVertexBuffer(VertexBuffer * vertexBuffer)
{
	delete vertexBuffer;
}

VertexDescription * Magma::OpenGLRenderDevice::CreateVertexDescription(unsigned int numVertexElements, const VertexElement * vertexElements)
{
	return new OpenGLVertexDescription(numVertexElements, vertexElements);
}

void Magma::OpenGLRenderDevice::DestroyVertexDescription(VertexDescription * vertexDescription)
{
	delete vertexDescription;
}

VertexArray * Magma::OpenGLRenderDevice::CreateVertexArray(unsigned int numVertexBuffers, VertexBuffer ** vertexBuffers, VertexDescription ** vertexDescriptions)
{
	return new OpenGLVertexArray(numVertexBuffers, vertexBuffers, vertexDescriptions);
}

void Magma::OpenGLRenderDevice::DestroyVertexArray(VertexArray * vertexArray)
{
	delete vertexArray;
}

void Magma::OpenGLRenderDevice::SetVertexArray(VertexArray * vertexArray)
{
	glBindVertexArray(reinterpret_cast<OpenGLVertexArray *>(vertexArray)->vao);
}

IndexBuffer * Magma::OpenGLRenderDevice::CreateIndexBuffer(long long size, const void * data)
{
	return new OpenGLIndexBuffer(size, data);
}

void Magma::OpenGLRenderDevice::DestroyIndexBuffer(IndexBuffer * indexBuffer)
{
	delete indexBuffer;
}

void Magma::OpenGLRenderDevice::SetIndexBuffer(IndexBuffer * indexBuffer)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, reinterpret_cast<OpenGLIndexBuffer *>(indexBuffer)->ibo);
}

Texture2D * Magma::OpenGLRenderDevice::CreateTexture2D(int width, int height, const void * data)
{
	return new OpenGLTexture2D(width, height, data);
}

void Magma::OpenGLRenderDevice::DestroyTexture2D(Texture2D * texture2D)
{
	delete texture2D;
}

void Magma::OpenGLRenderDevice::SetTexture2D(unsigned int slot, Texture2D * texture2D)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture2D ? reinterpret_cast<OpenGLTexture2D *>(texture2D)->texture : 0);

	// TO DO: Make parameters as option for 2D texture creation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

RasterState * Magma::OpenGLRenderDevice::CreateRasterState(bool cullEnabled, Winding frontFace, Face cullFace, RasterMode rasterMode)
{
	return new OpenGLRasterState(cullEnabled, frontFace, cullFace, rasterMode);
}

void Magma::OpenGLRenderDevice::DestroyRasterState(RasterState * rasterState)
{
	delete rasterState;
}

void Magma::OpenGLRenderDevice::SetRasterState(RasterState * rasterState)
{
	RasterState *oldRasterState = m_rasterState;

	if (rasterState)
		m_rasterState = dynamic_cast<OpenGLRasterState *>(rasterState);
	else
		m_rasterState = m_defaultRasterState;

	if (m_rasterState != oldRasterState)
	{
		if (m_rasterState->cullEnabled)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
		glFrontFace(m_rasterState->frontFace);
		glCullFace(m_rasterState->cullFace);
		glPolygonMode(GL_FRONT_AND_BACK, m_rasterState->polygonMode);
	}
}

DepthStencilState * Magma::OpenGLRenderDevice::CreateDepthStencilState(bool depthEnabled, bool depthWriteEnabled, float depthNear, float depthFar, Compare depthCompare, bool frontFaceStencilEnabled, Compare frontFaceStencilCompare, StencilAction frontFaceStencilFail, StencilAction frontFaceStencilPass, StencilAction frontFaceDepthFail, int frontFaceRef, unsigned int frontFaceReadMask, unsigned int frontFaceWriteMask, bool backFaceStencilEnabled, Compare backFaceStencilCompare, StencilAction backFaceStencilFail, StencilAction backFaceStencilPass, StencilAction backFaceDepthFail, int backFaceRef, unsigned int backFaceReadMask, unsigned int backFaceWriteMask)
{
	return new OpenGLDepthStencilState(depthEnabled, depthWriteEnabled, depthNear, depthFar, depthCompare, frontFaceStencilEnabled, frontFaceStencilCompare,
									   frontFaceStencilFail, frontFaceStencilPass, frontFaceDepthFail, frontFaceRef, frontFaceReadMask, frontFaceWriteMask, backFaceStencilEnabled,
									   backFaceStencilCompare, backFaceStencilFail, backFaceStencilPass, backFaceDepthFail, backFaceRef, backFaceReadMask, backFaceWriteMask);
}

void Magma::OpenGLRenderDevice::DestroyDepthStencilState(DepthStencilState * depthStencilState)
{
	delete depthStencilState;
}

void Magma::OpenGLRenderDevice::SetDepthStencilState(DepthStencilState * depthStencilState)
{
	DepthStencilState *oldDepthStencilState = m_depthStencilState;

	if (depthStencilState)
		m_depthStencilState = dynamic_cast<OpenGLDepthStencilState *>(depthStencilState);
	else
		m_depthStencilState = m_defaultDepthStencilState;

	if (m_depthStencilState != oldDepthStencilState)
	{
		if (m_depthStencilState->depthEnabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		glDepthFunc(m_depthStencilState->depthFunc);
		glDepthMask(m_depthStencilState->depthWriteEnabled ? GL_TRUE : GL_FALSE);
		glDepthRange(m_depthStencilState->depthNear, m_depthStencilState->depthFar);

		if (m_depthStencilState->frontFaceStencilEnabled || m_depthStencilState->backFaceStencilEnabled)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);

		// Front face
		glStencilFuncSeparate(GL_FRONT, m_depthStencilState->frontStencilFunc, m_depthStencilState->frontFaceRef, m_depthStencilState->frontFaceReadMask);
		glStencilMaskSeparate(GL_FRONT, m_depthStencilState->frontFaceWriteMask);
		glStencilOpSeparate(GL_FRONT, m_depthStencilState->frontFaceStencilFail, m_depthStencilState->frontFaceDepthFail, m_depthStencilState->frontFaceStencilPass);

		// Back face
		glStencilFuncSeparate(GL_BACK, m_depthStencilState->backStencilFunc, m_depthStencilState->backFaceRef, m_depthStencilState->backFaceReadMask);
		glStencilMaskSeparate(GL_BACK, m_depthStencilState->backFaceWriteMask);
		glStencilOpSeparate(GL_BACK, m_depthStencilState->backFaceStencilFail, m_depthStencilState->backFaceDepthFail, m_depthStencilState->backFaceStencilPass);
	}
}

void Magma::OpenGLRenderDevice::Clear(float red, float green, float blue, float alpha, float depth, int stencil)
{
	glClearColor(red, green, blue, alpha);
	glClearDepth(depth);
	glClearStencil(stencil);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Magma::OpenGLRenderDevice::DrawTriangles(int offset, int count)
{
	glDrawArrays(GL_TRIANGLES, offset, count);
}

void Magma::OpenGLRenderDevice::DrawTrianglesIndexed32(long long offset, int count)
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, reinterpret_cast<const void *>(offset));
}
