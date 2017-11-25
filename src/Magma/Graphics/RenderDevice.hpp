#pragma once

// This architecture is based on https://github.com/amesgames/RenderDevice

namespace Magma
{
	/// <summary>
	///		Encapsulates a vertex shader
	/// </summary>
	class VertexShader
	{
	public:
		virtual ~VertexShader() = default;
	protected:
		// Ensure these are never created directly
		VertexShader() = default;
	};

	/// <summary>
	///		Encapsulates a pixel shader
	/// </summary>
	class PixelShader
	{
	public:
		virtual ~PixelShader() = default;
	protected:
		// Ensure these are never created directly
		PixelShader() = default;
	};

	/// <summary>
	///		Encapsulates a shader pipeline uniform parameter
	/// </summary>
	class PipelineParam
	{
	public:
		virtual ~PipelineParam() = default;

		virtual void SetAsInt(int value) = 0;
		virtual void SetAsFloat(float value) = 0;
		virtual void SetAsMat4(const float *value) = 0;
		virtual void SetAsIntArray(int count, const int *values) = 0;
		virtual void SetAsFloatArray(int count, const float *values) = 0;
		virtual void SetAsMat4Array(int count, const float *values) = 0;
	protected:
		// Ensure these are never created directly
		PipelineParam() = default;
	};

	/// <summary>
	///		Encapsulates a shader pipeline
	/// </summary>
	class Pipeline
	{
	public:
		virtual ~Pipeline() = default;

		virtual PipelineParam *GetParam(const char *name) = 0;
	protected:
		// Ensure these are never created directly
		Pipeline() = default;
	};

	/// <summary>
	///		Encapsulates a vertex buffer
	/// </summary>
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;
	protected:
		// Ensure these are never created directly
		VertexBuffer() = default;
	};

	/// <summary>
	///		Encapsulates a vertex buffer semantic description
	/// </summary>
	class VertexDescription
	{
	public:
		virtual ~VertexDescription() = default;
	protected:
		// Ensure these are never created directly
		VertexDescription() = default;
	};

	/// <summary>
	///		Encapsulates a collection of vertex buffers and their semantic descriptions
	/// </summary>
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;
	protected:
		// Ensure these are never created directly
		VertexArray() = default;
	};

	/// <summary>
	///		Encapsulates an index buffer
	/// </summary>
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

	protected:
		// Ensure these are never created directly
		IndexBuffer() = default;
	};

	/// <summary>
	///		Encapsulates a 2D texture
	/// </summary>
	class Texture2D
	{
	public:
		virtual ~Texture2D() = default;
	protected:
		// Ensure these are never created directly
		Texture2D() = default;
	};

	enum class VertexElementType
	{
		Bytee = 0,
		Short,
		Int,

		UByte,
		UShort,
		UInt,

		ByteNormalize,
		ShortNormalize,
		IntNormalize,

		UByteNormalize,
		UShortNormalize,
		UIntNormalize,

		HalfFloat,
		Float,
		Double
	};

	/// <summary>
	///		Describes a vertex element within a vertex buffer
	/// </summary>
	struct VertexElement
	{
		/// <summary>
		///		Location binding for vertex element
		/// </summary>
		unsigned int index;

		/// <summary>
		///		Type of vertex element
		/// </summary>
		VertexElementType type;

		/// <summary>
		///		Number of components
		/// </summary>
		int size;

		/// <summary>
		///		Number of bytes between each successive element (leave zero for this to be assumed to be size times size of type)
		/// </summary>
		int stride;

		/// <summary>
		///		Offset where first occurrence of this vertex element resides in the buffer
		/// </summary>
		long long offset;
	};

	/// <summary>
	///		Encapsulates the rasterizer state
	/// </summary>
	class RasterState
	{
	public:
		virtual ~RasterState() = default;
	protected:
		// Ensure these are never created directly
		RasterState() = default;
	};

	enum class Winding
	{
		CW = 0,
		CCW,
		Count
	};

	enum class Face
	{
		Front = 0,
		Back,
		FrontAndBack,
		Count
	};

	enum class RasterMode
	{
		Point = 0,
		Line,
		Fill,
		Count
	};

	/// <summary>
	///		Encapsulates the depth/stencil state
	/// </summary>
	class DepthStencilState
	{
	public:
		virtual ~DepthStencilState() = default;
	protected:
		// Ensure these are never created directly
		DepthStencilState() = default;
	};

	enum class Compare
	{
		// Test comparison never passes
		Never = 0,

		// Test comparison passes if the incoming value is less than the stored value.
		Less,

		// Test comparison passes if the incoming value is equal to the stored value.
		Equal,

		// Test comparison passes if the incoming value is less than or equal to the stored value.
		LEqual,

		// Test comparison passes if the incoming value is greater than the stored value.
		Greater,

		// Test comparison passes if the incoming value is not equal to the stored value.
		NotEqual,

		// Test comparison passes if the incoming value is greater than or equal to the stored value.
		GEqual,

		// Test comparison always passes.
		Always,

		Count
	};

	enum class StencilAction
	{
		// Keeps the current value.
		Keep = 0,

		// Sets the stencil buffer to zero.
		Zero,

		// Sets the stencil buffer to the reference value masked with the write mask.
		Replace,

		// Increments the current stencil buffer value and clamps to maximum unsigned value.
		Incr,

		// Increments the current stencil buffer value and wraps the stencil buffer to zero when passing the maximum representable unsigned value.
		IncrWrap,

		// Decrements the current stencil buffer value and clamps to zero.
		Decr,

		// Decrements the current stencil buffer value and wraps the stencil buffer value to the maximum unsigned value.
		DecrWrap,

		// Bitwise invert of the current stencil buffer value.
		Invert,

		Count
	};

	
	/// <summary>
	///		Encapsulates the render device API.
	/// </summary>
	class RenderDevice
	{
	public:
		virtual ~RenderDevice() = default;

		/// <summary>
		///		Create a vertex shader from the supplied code (assumed GLSL)
		/// </summary>
		/// <param name="code">Vertex shader code</param>
		/// <returns>Vertex shader</returns>
		virtual VertexShader *CreateVertexShader(const char *code) = 0;

		/// <summary>
		///		Destroys a vertex shader
		/// </summary>
		/// <param name="vertexShader">Vertex shader</param>
		virtual void DestroyVertexShader(VertexShader *vertexShader) = 0;

		/// <summary>
		///		Create a pixel shader from the supplied code (assumed GLSL)
		/// </summary>
		/// <param name="code">Pixel shader code</param>
		/// <returns>Pixel shader</returns>
		virtual PixelShader *CreatePixelShader(const char *code) = 0;

		/// <summary>
		///		Destroys a pixel shader
		/// </summary>
		/// <param name="vertexShader">Pixel shader</param>
		virtual void DestroyPixelShader(PixelShader *pixelShader) = 0;

		/// <summary>
		///		Create a linked shader pipeline given a vertex and pixel shader
		/// </summary>
		/// <param name="vertexShader">Vertex shader</param>
		/// <param name="pixelShader">Pixel shader</param>
		/// <returns>Shader pipeline</returns>
		virtual Pipeline *CreatePipeline(VertexShader *vertexShader, PixelShader *pixelShader) = 0;

		/// <summary>
		///		Destroys a shader pipeline
		/// </summary>
		/// <param name="pipeline">Shader pipeline</param>
		virtual void DestroyPipeline(Pipeline *pipeline) = 0;

		/// <summary>
		///		Sets a shader pipeline as active for subsequent draw commands
		/// </summary>
		/// <param name="pipeline">Shader pipeline</param>
		virtual void SetPipeline(Pipeline *pipeline) = 0;

		/// <summary>
		///		Creates a vertex buffer
		/// </summary>
		/// <param name="size">Buffer size</param>
		/// <param name="data">Buffer data</param>
		/// <returns>Vertex buffer</returns>
		virtual VertexBuffer *CreateVertexBuffer(long long size, const void *data = nullptr) = 0;

		/// <summary>
		///		Destroys a vertex buffer
		/// </summary>
		/// <param name="vertexBuffer">Vertex buffer</param>
		virtual void DestroyVertexBuffer(VertexBuffer *vertexBuffer) = 0;

		/// <summary>
		///		Creates a vertex description given an array of VertexElement structures
		/// </summary>
		/// <param name="numVertexElements">Number of VertexElement structures</param>
		/// <param name="vertexElements">Array of VertexElement structures</param>
		/// <returns>Vertex description</returns>
		virtual VertexDescription *CreateVertexDescription(unsigned int numVertexElements, const VertexElement *vertexElements) = 0;

		/// <summary>
		///		Destroys a vertex description
		/// </summary>
		/// <param name="vertexDescription">Vertex description</param>
		virtual void DestroyVertexDescription(VertexDescription *vertexDescription) = 0;

		/// <summary>
		///		Creates a vertex array given an array of vertex buffers and associated vertex descriptions; the arrays must be the same size.
		/// </summary>
		/// <param name="numVertexBuffers">Number of vertex buffers</param>
		/// <param name="vertexBuffers">Vertex buffers array</param>
		/// <param name="vertexDescriptions">Vertex descriptions array</param>
		/// <returns>Vertex array</returns>
		virtual VertexArray *CreateVertexArray(unsigned int numVertexBuffers, VertexBuffer **vertexBuffers, VertexDescription **vertexDescriptions) = 0;

		/// <summary>
		///		Destroys a vertex array
		/// </summary>
		/// <param name="vertexArray">Vertex array</param>
		virtual void DestroyVertexArray(VertexArray *vertexArray) = 0;

		/// <summary>
		///		Set a vertex array as active for subsequent draw commands
		/// </summary>
		/// <param name="vertexArray">Vertex array</param>
		virtual void SetVertexArray(VertexArray *vertexArray) = 0;

		/// <summary>
		///		Create an index buffer	
		/// </summary>
		/// <param name="size">Index buffer size</param>
		/// <param name="data">Index buffer data</param>
		/// <returns></returns>
		virtual IndexBuffer *CreateIndexBuffer(long long size, const void *data = nullptr) = 0;

		/// <summary>
		///		Destroy an index buffer
		/// </summary>
		/// <param name="indexBuffer">Index buffer</param>
		virtual void DestroyIndexBuffer(IndexBuffer *indexBuffer) = 0;

		/// <summary>
		///		Set an index buffer as active for subsequent draw commands
		/// </summary>
		/// <param name="indexBuffer">Index buffer</param>
		virtual void SetIndexBuffer(IndexBuffer *indexBuffer) = 0;

		/// <summary>
		///		Create a 2D texture.
		/// 
		///		Data is assumed to consist of 32-bit pixel values where
		///		1 byte is used for each of the red, green and blue components,
		///		from lowest to highest byte order. The most significant byte
		///		is ignored.
		/// </summary>
		/// <param name="width">Texture width</param>
		/// <param name="height">Texture height</param>
		/// <param name="data">Texture data</param>
		/// <returns>Texture</returns>
		virtual Texture2D *CreateTexture2D(int width, int height, const void *data = nullptr) = 0;

		/// <summary>
		///		Destroy a 2D texture.
		/// </summary>
		/// <param name="texture2D">Texture</param>
		virtual void DestroyTexture2D(Texture2D *texture2D) = 0;

		/// <summary>
		///		Set a 2D texture as active on a slot for subsequent draw commands
		/// </summary>
		/// <param name="slot">Texture slot</param>
		/// <param name="texture2D">Texture</param>
		virtual void SetTexture2D(unsigned int slot, Texture2D *texture2D) = 0;

		/// <summary>
		///		Creates a raster state
		/// </summary>
		/// <param name="cullEnabled">Cull enabled?</param>
		/// <param name="frontFace">Winding mode for front face</param>
		/// <param name="cullFace">Cull face mode</param>
		/// <param name="rasterMode">Raster mode</param>
		/// <returns></returns>
		virtual RasterState *CreateRasterState(bool cullEnabled = true,
											   Winding frontFace = Winding::CCW,
											   Face cullFace = Face::Back,
											   RasterMode rasterMode = RasterMode::Fill) = 0;

		/// <summary>
		///		Destroy a raster state
		/// </summary>
		/// <param name="rasterState">Raster state</param>
		virtual void DestroyRasterState(RasterState *rasterState) = 0;

		/// <summary>
		///		 Set a raster state for subsequent draw commands.
		/// </summary>
		/// <param name="rasterState">Raster state</param>
		virtual void SetRasterState(RasterState *rasterState) = 0;

		/// <summary>
		///		Create a depth/stencil state.
		/// </summary>«
		/// <returns>Depth/stencil state</returns>
		virtual DepthStencilState *CreateDepthStencilState(bool depthEnabled = true,
														   bool depthWriteEnabled = true, float depthNear = 0, float depthFar = 1,
														   Compare depthCompare = Compare::Less, bool frontFaceStencilEnabled = false,
														   Compare frontFaceStencilCompare = Compare::Always,
														   StencilAction frontFaceStencilFail = StencilAction::Keep,
														   StencilAction frontFaceStencilPass = StencilAction::Keep,
														   StencilAction frontFaceDepthFail = StencilAction::Keep,
														   int frontFaceRef = 0, unsigned int frontFaceReadMask = 0xFFFFFFFF,
														   unsigned int frontFaceWriteMask = 0xFFFFFFFF,
														   bool backFaceStencilEnabled = false,
														   Compare backFaceStencilCompare = Compare::Always,
														   StencilAction backFaceStencilFail = StencilAction::Keep,
														   StencilAction backFaceStencilPass = StencilAction::Keep,
														   StencilAction backFaceDepthFail = StencilAction::Keep,
														   int backFaceRef = 0, unsigned int backFaceReadMask = 0xFFFFFFFF,
														   unsigned int backFaceWriteMask = 0xFFFFFFFF) = 0;

		/// <summary>
		///		Destroy a depth/stencil state.
		/// </summary>
		/// <param name="depthStencilState">Depth/stencil state</param>
		virtual void DestroyDepthStencilState(DepthStencilState *depthStencilState) = 0;

		/// <summary>
		///		Set a depth/stencil state for subsequent draw commands
		/// </summary>
		/// <param name="depthStencilState">Depth/stencil state</param>
		virtual void SetDepthStencilState(DepthStencilState *depthStencilState) = 0;

		/// <summary>
		///		Clear the default render target's color buffer, depth buffer, and stencil buffer to the specified values
		/// </summary>
		/// <param name="red">Red channel</param>
		/// <param name="green">Green channel</param>
		/// <param name="blue">Blue channel</param>
		/// <param name="alpha">Alpha channel</param>
		/// <param name="depth">Depth channel</param>
		/// <param name="stencil">Stencil channel</param>
		virtual void Clear(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f, float depth = 1.0f, int stencil = 0) = 0;

		/// <summary>
		///		Draw a collection of triangles using the currently active shader pipeline and vertex array data
		/// </summary>
		/// <param name="offset">Starting offset in vertex array</param>
		/// <param name="count">Triangle count</param>
		virtual void DrawTriangles(int offset, int count) = 0;

		/// <summary>
		///		Draw a collection of triangles using the currently active shader pipeline, vertex array data,
		///		and index buffer
		/// </summary>
		/// <param name="offset">Starting offset in vertex array</param>
		/// <param name="count">Triangle count</param>
		virtual void DrawTrianglesIndexed32(long long offset, int count) = 0;
	};
}