#pragma once
#ifndef _VECTORS_VERTICES_H_
#define _VECTORS_VERTICES_H_
#include <DirectXMath.h>

namespace demo
{
	struct XMFLOAT3 {
		float x;
		float y;
		float z;
	};

	struct Vector2D
	{
		float x;
		float y;
	};

	struct Vector3D
	{
		float x;
		float y;
		float z;
	};
	
	struct Vertex3D
	{
		Vector3D pos;
		Vector3D normal;
		Vector2D textureCoordinate;
	};

	struct VertexPos
	{
		XMFLOAT3 pos;
	};
}

#endif // !_VECTORS_VERTICES_H_