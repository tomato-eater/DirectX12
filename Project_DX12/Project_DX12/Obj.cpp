#include "Obj.h"

void Obj::Initialize(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 col)
{
	x = pos.x;
	y = pos.y;
	z = pos.z;

	world = DirectX::XMMatrixTranslation(x, y, z);
	color = col;
}

void Obj::Updata()
{
	/*
	move += 0.02f;
	world = DirectX::XMMatrixTranslation(0.0f, std::sinf(move) * 1.5f, 0.0f);
	color = DirectX::XMFLOAT4(0.1f, 1.0f, 1.0f, 1.0f);
	*/
	if (GetAsyncKeyState('A'))
		x -= 0.01f;
	if (GetAsyncKeyState('D'))
		x += 0.01f;

	if (GetAsyncKeyState('Q'))
		y -= 0.01f;
	if (GetAsyncKeyState('E'))
		y += 0.01f;

	if (GetAsyncKeyState('S'))
		z -= 0.01f;
	if (GetAsyncKeyState('W'))
		z += 0.01f;
	
	world = DirectX::XMMatrixTranslation(x, y, z);
}

void Obj::Hit()
{
	if (color.z > 0.0f)
	{
		color.z -= 0.1f;
		return;
	}
	if (color.y > 0.0f)
	{
		color.y -= 0.1f;
		return;
	}
	if (color.x > 0.0f)
	{
		color.x -= 0.1f;
		return;
	}
}