#include "Camera.h"

constexpr float eyeMoveSpeed = 0.03f;
constexpr float destTargetToView = -5.0f;

void Camera::Initialize(int wi, int hi)
{
	position = DirectX::XMFLOAT3(0.0f, 0.5f, destTargetToView);
	target = DirectX::XMFLOAT3(0.0f, 0.0f, 1.5f);
	up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

	projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, static_cast<float>(wi) / static_cast<float>(hi), 0.1f, 100.0f);
}

void Camera::Update()
{
	static float angle = 0.0f;
	if (GetAsyncKeyState('A'))
	{
		angle += eyeMoveSpeed;
		position.x = destTargetToView * std::sinf(angle);
		position.z = destTargetToView * std::cosf(angle);
	}
	if(GetAsyncKeyState('D'))
	{
		angle -= eyeMoveSpeed;
		position.x = destTargetToView * std::sinf(angle);
		position.z = destTargetToView * std::cosf(angle);
	}
	
	view = DirectX::XMMatrixLookAtLH
	(
		DirectX::XMLoadFloat3(&position),
		DirectX::XMLoadFloat3(&target),
		DirectX::XMLoadFloat3(&up)
	);
}