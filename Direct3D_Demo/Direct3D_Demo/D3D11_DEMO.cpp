#include "D3D11_DEMO.h"

void D3D11_DEMO::Update(float dt)
{

}

void D3D11_DEMO::Render()
{
	if (this->_d3dContext == 0)
	{
		return;
	}
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f }; // RGBA black color
	this->_d3dContext->ClearRenderTargetView(this->_backBufferTarget, clearColor);
	this->_swapChain->Present(0, 0);
}
