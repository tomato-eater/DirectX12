#include "Obj.h"

void Obj::Initialize(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 col)
{
	world = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	color = col;
}

void Obj::Updata()
{
	/*
	move += 0.02f;
	world = DirectX::XMMatrixTranslation(0.0f, std::sinf(move) * 1.5f, 0.0f);
	color = DirectX::XMFLOAT4(0.1f, 1.0f, 1.0f, 1.0f);
	*/
	DirectX::XMFLOAT3 pos = {};

	if (GetAsyncKeyState('A'))
		pos.x -= 0.01f;
	if (GetAsyncKeyState('D'))
		pos.x += 0.01f;

	if (GetAsyncKeyState('Q'))
		pos.y -= 0.01f;
	if (GetAsyncKeyState('E'))
		pos.y += 0.01f;

	if (GetAsyncKeyState('S'))
		pos.z -= 0.01f;
	if (GetAsyncKeyState('W'))
		pos.z += 0.01f;
	
	{	//³‹K‰»
		float mag = 1 / sqrtf(powf(pos.x, 2) + powf(pos.y, 2) + powf(pos.z, 2));
		if (mag > 1.0f) mag = 1.0f;
		pos.x *= mag;
		pos.y *= mag;
		pos.z *= mag;
	}

	float x = DirectX::XMVectorGetX(world.r[3]) + pos.x;
	float y = DirectX::XMVectorGetY(world.r[3]) + pos.y;
	float z = DirectX::XMVectorGetZ(world.r[3]) + pos.z;

	world = DirectX::XMMatrixTranslation(x, y, z);
}

void Obj::SetRot(int i)
{
	//‰ñ“]ˆ—’Ç‰Á—\’è
	float rot = DirectX::XMConvertToRadians(45.0f);
	world = DirectX::XMMatrixRotationZ(rot) * world;

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