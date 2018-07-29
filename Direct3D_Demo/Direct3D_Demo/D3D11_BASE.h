#pragma once
#ifndef _D3D11_BASE_H
#define _D3D11_BASE_H

#include <d3d11.h>
#include <d3dcommon.h>

/*
This is a base class that can:
1. Initialize Direct3D
2. Release Direct3D objects created during startup
3. Store member variables for our Direct3D objects that are not demo-specific
4. Provide a way to *load* demo-specific content
5. Provide a way to *unload* demo-specific content
6. Provide the ability to perform specific updates once per frame
7. Provide the specific rendering code
Any additional functionallity should be added by extending the class and overriding its virtual methods
*/
class D3D11_BASE
{
public:
	D3D11_BASE();
	virtual ~D3D11_BASE();

	bool Initialize(HINSTANCE hInstanse, HWND hWnd);
	void Shutdown();
	
	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
protected:
	HINSTANCE _hInstanse;
	HWND _hWnd;
	D3D_DRIVER_TYPE _driverType;
	D3D_FEATURE_LEVEL _featureLevel;

	ID3D11Device* _d3dDevice;
	ID3D11DeviceContext* _d3dContext;
	IDXGISwapChain* _swapChain;
	ID3D11RenderTargetView* _backBufferTarget;
};

#endif // !_D3D11_BASE_CLASS