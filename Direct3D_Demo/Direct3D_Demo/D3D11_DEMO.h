#pragma once
#ifndef _BLANK_DEMO_H_
#define _BLANK_DEMO_H_

#include "D3D11_BASE.h"

class D3D11_DEMO : public D3D11_BASE
{
public:
	
	void Update(float dt) override;
	void Render() override;
};

#endif // !_BlankDemo_H