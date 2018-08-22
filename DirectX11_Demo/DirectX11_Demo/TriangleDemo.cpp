#include "TriangleDemo.h"
#include <xnamath.h>

struct VertexPos
{
	XMFLOAT3 pos;
};

TriangleDemo::TriangleDemo()
	:solidColorVS_(0), solidColorPS_(0), inputLayout_(0), vertexBuffer_(0)
{
}


TriangleDemo::~TriangleDemo()
{
}

bool TriangleDemo::LoadContent()
{
	ID3DBlob * vsBuffer = NULL;
	bool CompileResult = CompileD3DShader("SolidGreenColor.fx", "VS_Main", "vs_4_0", &vsBuffer);

	if (!CompileResult)
	{
		MessageBox(0, "Error loading vertex shader.", "Compile Error", MB_OK);
		return false;
	}
	HRESULT d3dResult = 
		this->d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, &solidColorVS_);
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
	d3dResult = this->d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), 0, &this->solidColorPS_);
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
	return true;
}

void TriangleDemo::UnloadContent()
{
	if (this->solidColorPS_)
	{
		this->solidColorPS_->Release();
	}
	if (this->solidColorVS_)
	{
		this->solidColorVS_->Release();
	}
	if (this->inputLayout_)
	{
		this->inputLayout_->Release();
	}
	if (this->vertexBuffer_)
	{
		this->vertexBuffer_->Release();
	}
	DX11_BASE::UnloadContent();
}

void TriangleDemo::Update(float dt)
{
}

void TriangleDemo::Render()
{
	if (!this->d3dContext_)
	{
		return;
	}
	float clearColor[] = { 0.0f, 0.0f, 0.25f, 1.0f };
	d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);
	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;

	d3dContext_->IASetInputLayout(inputLayout_);
	d3dContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dContext_->VSSetShader(solidColorVS_, 0, 0);
	d3dContext_->PSSetShader(solidColorPS_, 0, 0);
	d3dContext_->Draw(3, 0);
	swapChain_->Present(0, 0);
}
