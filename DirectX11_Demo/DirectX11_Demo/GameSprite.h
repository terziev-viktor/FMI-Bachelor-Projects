#pragma once
#ifndef GAME_SPRITE_H_
#define GAME_SPRITE_H_
#include <Windows.h>
#include <xnamath.h>

class GameSprite
{
public:
	GameSprite();

	virtual ~GameSprite();

	XMMATRIX GetWorldMatrix();

	/*Setters*/
	void SetPosition(const XMFLOAT2 & position);

	void SetRotation(float r);

	void SetScale(const XMFLOAT2 & scale);

	/*Getters*/
	XMFLOAT2 GetPosition() const;

	float GetRotation() const;

	XMFLOAT2 GetScale() const;

private:
	XMFLOAT2 position_;
	float rotation_;
	XMFLOAT2 scale_;
};
#endif // !GAME_SPRITE_H_

