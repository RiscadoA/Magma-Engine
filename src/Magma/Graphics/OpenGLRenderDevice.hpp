#pragma once

#include "RenderDevice.hpp"

namespace Magma
{

	class OpenGLRasterState;
	class OpenGLDepthStencilState;

	class OpenGLRenderDevice : public RenderDevice
	{
	public:
		OpenGLRenderDevice();

		// Inherited via RenderDevice
		virtual VertexShader * CreateVertexShader(const char * code) override;
		virtual void DestroyVertexShader(VertexShader * vertexShader) override;
		virtual PixelShader * CreatePixelShader(const char * code) override;
		virtual void DestroyPixelShader(PixelShader * pixelShader) override;
		virtual Pipeline * CreatePipeline(VertexShader * vertexShader, PixelShader * pixelShader) override;
		virtual void DestroyPipeline(Pipeline * pipeline) override;
		virtual void SetPipeline(Pipeline * pipeline) override;
		virtual VertexBuffer * CreateVertexBuffer(long long size, const void * data = nullptr) override;
		virtual void DestroyVertexBuffer(VertexBuffer * vertexBuffer) override;
		virtual VertexDescription * CreateVertexDescription(unsigned int numVertexElements, const VertexElement * vertexElements) override;
		virtual void DestroyVertexDescription(VertexDescription * vertexDescription) override;
		virtual VertexArray * CreateVertexArray(unsigned int numVertexBuffers, VertexBuffer ** vertexBuffers, VertexDescription ** vertexDescriptions) override;
		virtual void DestroyVertexArray(VertexArray * vertexArray) override;
		virtual void SetVertexArray(VertexArray * vertexArray) override;
		virtual IndexBuffer * CreateIndexBuffer(long long size, const void * data = nullptr) override;
		virtual void DestroyIndexBuffer(IndexBuffer * indexBuffer) override;
		virtual void SetIndexBuffer(IndexBuffer * indexBuffer) override;
		virtual Texture2D * CreateTexture2D(int width, int height, const void * data = nullptr) override;
		virtual void DestroyTexture2D(Texture2D * texture2D) override;
		virtual void SetTexture2D(unsigned int slot, Texture2D * texture2D) override;
		virtual RasterState * CreateRasterState(bool cullEnabled = true, Winding frontFace = Winding::CCW, Face cullFace = Face::Back, RasterMode rasterMode = RasterMode::Fill) override;
		virtual void DestroyRasterState(RasterState * rasterState) override;
		virtual void SetRasterState(RasterState * rasterState) override;
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
														   unsigned int backFaceWriteMask = 0xFFFFFFFF) override;
		virtual void DestroyDepthStencilState(DepthStencilState * depthStencilState) override;
		virtual void SetDepthStencilState(DepthStencilState * depthStencilState) override;
		virtual void Clear(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f, float depth = 1.0f, int stencil = 0) override;
		virtual void DrawTriangles(int offset, int count) override;
		virtual void DrawTrianglesIndexed32(long long offset, int count) override;

	private:
		OpenGLRasterState* m_rasterState = nullptr;
		OpenGLRasterState* m_defaultRasterState = nullptr;

		OpenGLDepthStencilState* m_depthStencilState = nullptr;
		OpenGLDepthStencilState* m_defaultDepthStencilState = nullptr;
	};
}