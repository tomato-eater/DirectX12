#include "PolyTemp.h"


//ƒ|ƒŠƒSƒ“‚Ìì¬
bool PolyTemp::Create()
{
	if (CreateVertex())	return true;
	if (CreateIndex())	return true;
	return false;
}

//ƒq[ƒv‚ÌÝ’è
D3D12_HEAP_PROPERTIES PolyTemp::SetHeap()
{
	D3D12_HEAP_PROPERTIES heap{};
	heap.Type = D3D12_HEAP_TYPE_UPLOAD;
	heap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap.CreationNodeMask = 0;
	heap.VisibleNodeMask = 0;
	return heap;
}

//ƒŠƒ\[ƒX‚ÌÝ’è
D3D12_RESOURCE_DESC PolyTemp::SetResource(const UINT size)
{
	D3D12_RESOURCE_DESC desc{};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = size;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc = { 1,0 };
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	return desc;
}

//ƒ|ƒŠƒSƒ“‚Ì•`‰æ
void PolyTemp::Drow(ID3D12GraphicsCommandList* list)
{
	list->IASetVertexBuffers(0, 1, &vertexView);
	list->IASetIndexBuffer(&indexView);
	list->IASetPrimitiveTopology(topolory);
	list->DrawIndexedInstanced(count, 1, 0, 0, 0);
}