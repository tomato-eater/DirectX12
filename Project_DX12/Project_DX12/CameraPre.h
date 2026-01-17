#pragma once

#include "Camera.h"
#include "ConBuffer.h"

#include "Device.h"
#include "Heap.h"

#include "ComLis.h"

class CameraPre
{
private:
	Camera camera{};	//カメラ本体
	ConBuffer camBuff{};//カメラバッファ

public:
	bool Create(int wi, int hi, Device& devi, Heap& cons, UINT idx);
	void Change(ComLis& list);
	void Update() { camera.Update(); }
};

