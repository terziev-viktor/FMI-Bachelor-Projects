#pragma once
#ifndef DX11_BASE_H_
#define DX11_BASE_H_

#include <d3d11.h>
#include <D3DX11.h>
#include <DxErr.h>

class DX11_BASE
{
public:
	DX11_BASE();

	virtual ~DX11_BASE();

	bool Initialize(HINSTANCE hInstance, HWND hwnd);

	void Shutdown();

	bool CompileD3DShader(const char * path, const char * entry, const char * shaderModel, ID3DBlob ** buffer);

	// Override with demo specifics, if any...
	virtual bool LoadContent();

	// Override with demo specifics, and call DX11_BASE::UnloadContext at the end
	virtual void UnloadContent();

	// Update scene of game loop
	virtual void Update(float dt) = 0;

	// Render scene of game loop
	virtual void Render() = 0;

protected:
	HINSTANCE hInstance_;
	HWND hWnd_;
	D3D_DRIVER_TYPE driverType_;
	D3D_FEATURE_LEVEL featureLevel_;
	ID3D11Device * d3dDevice_;
	ID3D11DeviceContext * d3dContext_;
	IDXGISwapChain * swapChain_;
	ID3D11RenderTargetView * backBufferTarget_;
};

#endif // !DX11_BASE
