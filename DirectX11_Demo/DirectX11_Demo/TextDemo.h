#pragma once

#ifndef _TEXT_DEMO_
#define _TEXT_DEMO_

#include "DX11_BASE.h"

class TextDemo : public DX11_BASE
{
public:
	TextDemo();

	virtual ~TextDemo();

	bool LoadContent();

	void UnloadContent();

	// Inherited via DX11_BASE
	virtual void Update(float dt) override;

	virtual void Render() override;

private:
	bool DrawString(const char * message, float startX, float startY);
private:
	ID3D11VertexShader * solidColorVS_;
	ID3D11PixelShader * solidColorPS_;

	ID3D11InputLayout * inputLayout_;
	ID3D11Buffer * vertexBuffer_;

	ID3D11ShaderResourceView * colorMap_;
	ID3D11SamplerState * colorMapSampler_;
};



#endif // !_TEXT_DEMO_
