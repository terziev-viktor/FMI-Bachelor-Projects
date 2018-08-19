#include "BlankDemo.h"

BlankDemo::BlankDemo()
{
	this->rgba_.R = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	this->rgba_.G = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	this->rgba_.B = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	this->rgba_.A = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	this->rgba_.step = 0.01f;
}


BlankDemo::~BlankDemo()
{
}

void BlankDemo::Update(float dt)
{

}

void BlankDemo::Render()
{
	Sleep(50); // hard work simulation
	if (d3dContext_ == 0)
	{
		return;
	}
	this->rgba_.R = (this->rgba_.R + this->rgba_.step) > 1.0f ? 0.0f : this->rgba_.R + this->rgba_.step;
	this->rgba_.G = (this->rgba_.G + this->rgba_.step) > 1.0f ? 0.0f : this->rgba_.G + this->rgba_.step;
	this->rgba_.B = (this->rgba_.B + this->rgba_.step) > 1.0f ? 0.0f : this->rgba_.B + this->rgba_.step;
	this->rgba_.A = (this->rgba_.A + this->rgba_.step) > 1.0f ? 0.0f : this->rgba_.A + this->rgba_.step;

	float clearColor[] = { this->rgba_.R, this->rgba_.G, this->rgba_.B, this->rgba_.A };
	d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);
	swapChain_->Present(0, 0);

}
