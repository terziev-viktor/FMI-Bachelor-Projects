#pragma once
#include "DX11_BASE.h"
#include "GameSprite.h"

class GameSpriteDemo : public DX11_BASE
{
public:
	GameSpriteDemo();
	virtual ~GameSpriteDemo();

	static const UINT GAME_SPRITES_COUNT = 2;

	bool LoadContent() override;

	void UnloadContent() override;

	// Inherited via DX11_BASE
	virtual void Update(float dt) override;

	virtual void Render() override;
private:
	ID3D11VertexShader * solidColorVS_;
	ID3D11PixelShader* solidColorPS_;
	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;
	ID3D11ShaderResourceView* colorMap_;
	ID3D11SamplerState* colorMapSampler_;
	ID3D11BlendState* alphaBlendState_;
	GameSprite sprites_[GAME_SPRITES_COUNT];
	ID3D11Buffer* mvpCB_;
	XMMATRIX vpMatrix_;
};

