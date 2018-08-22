#include "GameSprite.h"



GameSprite::GameSprite() : rotation_(0)
{
	scale_.x = scale_.y = 1.0f;
}


GameSprite::~GameSprite()
{

}

XMMATRIX GameSprite::GetWorldMatrix()
{
	XMMATRIX translation = XMMatrixTranslation(position_.x, position_.y, 0.0f);
	XMMATRIX rotationZ = XMMatrixRotationZ(rotation_);
	XMMATRIX scale = XMMatrixScaling(scale_.x, scale_.y, 1.0f);
	return translation * rotationZ * scale;
}

void GameSprite::SetPosition(const XMFLOAT2 & position)
{
	this->position_ = position;
}

void GameSprite::SetRotation(float r)
{
	this->rotation_ = r;
}

void GameSprite::SetScale(const XMFLOAT2 & scale)
{
	this->scale_ = scale;
}

XMFLOAT2 GameSprite::GetPosition() const
{
	return this->position_;
}

float GameSprite::GetRotation() const
{
	return this->rotation_;
}

XMFLOAT2 GameSprite::GetScale() const
{
	return this->scale_;
}
