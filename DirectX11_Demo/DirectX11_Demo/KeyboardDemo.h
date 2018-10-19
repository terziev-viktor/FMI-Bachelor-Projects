#pragma once

#ifndef _KEYBOARD_DEMO_
#define _KEYBOARD_DEMO_

#include "DX11_BASE.h"

class KeyboardDemo : public DX11_BASE
{
public:
	KeyboardDemo();
	virtual ~KeyboardDemo();

	// Inherited via DX11_BASE
	bool LoadContent() override;

	void UnloadContent() override;
	
	virtual void Update(float dt) override;

	virtual void Render() override;

private:
	ID3D11VertexShader * customColorVS_;
	ID3D11PixelShader * customColorPS_;
	ID3D11InputLayout * inputLayout_;
	ID3D11Buffer * vertexBuffer_;
	ID3D11Buffer * colorCB_;
	int selectedColor_;
};

#endif // !_KEYBOARD_DEMO_
