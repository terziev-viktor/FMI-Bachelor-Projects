#pragma once
#ifndef TRIANGLE_DEMO_H_
#define TRIANGLE_DEMO_H_

#include "DX11_BASE.h"

class TriangleDemo : public DX11_BASE
{
public:
	TriangleDemo();
	virtual ~TriangleDemo();

	bool LoadContent() override;

	void UnloadContent() override;

	// Inherited via DX11_BASE
	virtual void Update(float dt) override;

	virtual void Render() override;
private:
	ID3D11VertexShader * solidColorVS_;
	ID3D11PixelShader * solidColorPS_;
	ID3D11InputLayout * inputLayout_;
	ID3D11Buffer * vertexBuffer_;
};

#endif // !TRIANGLE_DEMO_H_

