#pragma once
#ifndef BLANK_DEMO_H_
#define BLANK_DEMO_H_

#include "DX11_BASE.h"

class BlankDemo : public DX11_BASE
{
public:
	BlankDemo();

	virtual ~BlankDemo();

	void Update(float dt) override;

	void Render() override;

private:
	struct RGBA
	{
		float R;
		float G;
		float B;
		float A;
		float step;
	} rgba_;
};

#endif // !BLANK_DEMO_H_
