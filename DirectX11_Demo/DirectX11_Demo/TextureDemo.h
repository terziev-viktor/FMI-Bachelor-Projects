#pragma once
#ifndef TEXTURE_DEMO_H_
#define TEXTURE_DEMO_H_

#include "DX11_BASE.h"

class TextureDemo : public DX11_BASE
{
public:
	TextureDemo();
	virtual ~TextureDemo();

	bool LoadContent() override;

	void UnloadContent() override;

	// Inherited via DX11_BASE
	virtual void Update(float dt) override;

	virtual void Render() override;
private:
	ID3D11VertexShader * solidColorVS_;
	ID3D11PixelShader* solidColorPS_;

	ID3D11InputLayout * inputLayout_;
	ID3D11Buffer* vertexBuffer_;

	ID3D11ShaderResourceView * colorMap_;
	ID3D11SamplerState * colorMapSampler_;
};

#endif // !TEXTURE_DEMO_H_
