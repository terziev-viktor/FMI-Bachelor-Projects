#include "KeyboardDemo.h"
#include <xnamath.h>

bool KeyboardDemo::LoadContent()
{
	ID3DBlob * vsBuffer = NULL;
	bool CompileResult = CompileD3DShader("SolidGreenColor.fx", "VS_Main", "vs_4_0", &vsBuffer);

	if (!CompileResult)
	{
		MessageBox(0, "Error loading vertex shader.", "Compile Error", MB_OK);
		return false;
	}
	HRESULT d3dResult =
		this->d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, &customColorVS_);
	if (FAILED(d3dResult))
	{
		if (vsBuffer)
		{
			vsBuffer->Release();
		}
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT totalLayoutElements = ARRAYSIZE(solidColorLayout);
	d3dResult = this->d3dDevice_->CreateInputLayout(solidColorLayout, totalLayoutElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &this->inputLayout_);
	vsBuffer->Release();
	if (FAILED(d3dResult))
	{
		return false;
	}
	ID3DBlob * psBuffer = NULL;
	CompileResult = CompileD3DShader("SolidGreenColor.fx", "PS_Main", "ps_4_0", &psBuffer);
	if (!CompileResult)
	{
		if (psBuffer)
		{
			psBuffer->Release();
		}
		MessageBox(0, "Error loading pixel shader", "Compile Error", MB_OK);
		return false;
	}
	d3dResult = this->d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), 0, &this->customColorPS_);
	psBuffer->Release();
	if (FAILED(d3dResult))
	{
		return false;
	}
	// Create a triangle
	VertexPos vertices[] =
	{
		XMFLOAT3(0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f)
	};
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 3;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;
	d3dResult = this->d3dDevice_->CreateBuffer(&vertexDesc,
		&resourceData, &vertexBuffer_);
	if (FAILED(d3dResult))
	{
		return false;
	}
	D3D11_BUFFER_DESC constBufferDesc;
	ZeroMemory(&constBufferDesc, sizeof(constBufferDesc));
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.ByteWidth = sizeof(XMFLOAT4);
	constBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dResult = this->d3dDevice_->CreateBuffer(&constBufferDesc, 0, &this->colorCB_);
	if (FAILED(d3dResult))
	{
		return false;
	}
	return true;
}

void KeyboardDemo::UnloadContent()
{
	if (customColorVS_) customColorVS_->Release();
	if (customColorPS_) customColorPS_->Release();
	if (inputLayout_) inputLayout_->Release();
	if (vertexBuffer_) vertexBuffer_->Release();
	if (colorCB_) colorCB_->Release();
	customColorVS_ = 0;
	customColorPS_ = 0;
	inputLayout_ = 0;
	vertexBuffer_ = 0;
	colorCB_ = 0;
	
	DX11_BASE::UnloadContent();
}

void KeyboardDemo::Update(float dt)
{

}

void KeyboardDemo::Render()
{
	if (d3dContext_ == 0)
	{
		return;
	}
	float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
	this->d3dContext_->ClearRenderTargetView(this->backBufferTarget_, clearColor);
	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;
	this->d3dContext_->IASetInputLayout(this->inputLayout_);
	this->d3dContext_->IASetVertexBuffers(0, 1, &this->vertexBuffer_, &stride, &offset);
	this->d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	XMFLOAT4 redColor(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 greenColor(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blueColor(0.0f, 0.0f, 1.0f, 1.0f);

	if (selectedColor_ == 0)
	{
		this->d3dContext_->UpdateSubresource(colorCB_, 0, 0, &redColor, 0, 0);
	}
	else if (selectedColor_ == 1)
	{
		this->d3dContext_->UpdateSubresource(colorCB_, 0, 0, &greenColor, 0, 0);
	}
	else
	{
		this->d3dContext_->UpdateSubresource(colorCB_, 0, 0, &blueColor, 0, 0);
	}
	d3dContext_->VSSetShader(customColorVS_, 0, 0);
	d3dContext_->PSSetShader(customColorPS_, 0, 0);
	d3dContext_->PSSetConstantBuffers(0, 1, &colorCB_);
	d3dContext_->Draw(3, 0);
	swapChain_->Present(0, 0);
}
