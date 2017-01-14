#pragma once
#include "stdafx.h"

#define D3D_DEBUGFONTNAME  L"Consolas"

class CDirect3D9DevHook : public IDirect3DDevice9
{
private:
	IDirect3DDevice9* m_ptr;
	LPD3DXFONT m_dbgFont;
	WCHAR* m_dbgText;
public:
	CDirect3D9DevHook(IDirect3DDevice9* ptr) : m_ptr(ptr)
	{
		HRESULT fr = D3DXCreateFont(m_ptr, 12, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, D3D_DEBUGFONTNAME, &m_dbgFont);
	}

public:
	void _stdcall SetDebugText(LPWSTR dbgText);
	HRESULT _stdcall EndScene();
#pragma region stubs
	COM_METHOD(HRESULT, QueryInterface)(THIS_ REFIID riid, void** ppvObj) { return  m_ptr->QueryInterface(riid, ppvObj); }
	COM_METHOD(ULONG, AddRef)(THIS) { return  m_ptr->AddRef(); }
	COM_METHOD(ULONG, Release)(THIS) { return  m_ptr->Release(); }
	COM_METHOD(HRESULT, TestCooperativeLevel)(THIS) { return  m_ptr->TestCooperativeLevel(); }
	COM_METHOD(UINT, GetAvailableTextureMem)(THIS) { return  m_ptr->GetAvailableTextureMem(); }
	COM_METHOD(HRESULT, EvictManagedResources)(THIS) { return  m_ptr->EvictManagedResources(); }
	COM_METHOD(HRESULT, GetDirect3D)(THIS_ IDirect3D9** ppD3D9) { return  m_ptr->GetDirect3D(ppD3D9); }
	COM_METHOD(HRESULT, GetDeviceCaps)(THIS_ D3DCAPS9* pCaps) { return  m_ptr->GetDeviceCaps(pCaps); }
	COM_METHOD(HRESULT, GetDisplayMode)(THIS_ UINT iSwapChain, D3DDISPLAYMODE* pMode) { return  m_ptr->GetDisplayMode(iSwapChain, pMode); }
	COM_METHOD(HRESULT, GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS *pParameters) { return  m_ptr->GetCreationParameters(pParameters); }
	COM_METHOD(HRESULT, SetCursorProperties)(THIS_ UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap) { return  m_ptr->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap); }
	COM_METHOD(void, SetCursorPosition)(THIS_ int X, int Y, DWORD Flags) { return  m_ptr->SetCursorPosition(X, Y, Flags); }
	COM_METHOD(BOOL, ShowCursor)(THIS_ BOOL bShow) { return  m_ptr->ShowCursor(bShow); }
	COM_METHOD(HRESULT, CreateAdditionalSwapChain)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain) { return  m_ptr->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain); }
	COM_METHOD(HRESULT, GetSwapChain)(THIS_ UINT iSwapChain, IDirect3DSwapChain9** pSwapChain) { return  m_ptr->GetSwapChain(iSwapChain, pSwapChain); }
	COM_METHOD(UINT, GetNumberOfSwapChains)(THIS) { return  m_ptr->GetNumberOfSwapChains(); }
	COM_METHOD(HRESULT, Reset)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters) { return  m_ptr->Reset(pPresentationParameters); }
	COM_METHOD(HRESULT, Present)(THIS_ CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) { return  m_ptr->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion); }
	COM_METHOD(HRESULT, GetBackBuffer)(THIS_ UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) { return  m_ptr->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer); }
	COM_METHOD(HRESULT, GetRasterStatus)(THIS_ UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus) { return  m_ptr->GetRasterStatus(iSwapChain, pRasterStatus); }
	COM_METHOD(HRESULT, SetDialogBoxMode)(THIS_ BOOL bEnableDialogs) { return  m_ptr->SetDialogBoxMode(bEnableDialogs); }
	COM_METHOD(void, SetGammaRamp)(THIS_ UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp) { return  m_ptr->SetGammaRamp(iSwapChain, Flags, pRamp); }
	COM_METHOD(void, GetGammaRamp)(THIS_ UINT iSwapChain, D3DGAMMARAMP* pRamp) { return  m_ptr->GetGammaRamp(iSwapChain, pRamp); }
	COM_METHOD(HRESULT, CreateTexture)(THIS_ UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) { return  m_ptr->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle); }
	COM_METHOD(HRESULT, CreateVolumeTexture)(THIS_ UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle) { return  m_ptr->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle); }
	COM_METHOD(HRESULT, CreateCubeTexture)(THIS_ UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle) { return  m_ptr->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle); }
	COM_METHOD(HRESULT, CreateVertexBuffer)(THIS_ UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle) { return  m_ptr->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle); }
	COM_METHOD(HRESULT, CreateIndexBuffer)(THIS_ UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle) { return  m_ptr->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle); }
	COM_METHOD(HRESULT, CreateRenderTarget)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) { return  m_ptr->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle); }
	COM_METHOD(HRESULT, CreateDepthStencilSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) { return  m_ptr->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle); }
	COM_METHOD(HRESULT, UpdateSurface)(THIS_ IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint) { return  m_ptr->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint); }
	COM_METHOD(HRESULT, UpdateTexture)(THIS_ IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture) { return  m_ptr->UpdateTexture(pSourceTexture, pDestinationTexture); }
	COM_METHOD(HRESULT, GetRenderTargetData)(THIS_ IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface) { return  m_ptr->GetRenderTargetData(pRenderTarget, pDestSurface); }
	COM_METHOD(HRESULT, GetFrontBufferData)(THIS_ UINT iSwapChain, IDirect3DSurface9* pDestSurface) { return  m_ptr->GetFrontBufferData(iSwapChain, pDestSurface); }
	COM_METHOD(HRESULT, StretchRect)(THIS_ IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter) { return  m_ptr->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter); }
	COM_METHOD(HRESULT, ColorFill)(THIS_ IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color) { return  m_ptr->ColorFill(pSurface, pRect, color); }
	COM_METHOD(HRESULT, CreateOffscreenPlainSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) { return  m_ptr->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle); }
	COM_METHOD(HRESULT, SetRenderTarget)(THIS_ DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget) { return  m_ptr->SetRenderTarget(RenderTargetIndex, pRenderTarget); }
	COM_METHOD(HRESULT, GetRenderTarget)(THIS_ DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget) { return  m_ptr->GetRenderTarget(RenderTargetIndex, ppRenderTarget); }
	COM_METHOD(HRESULT, SetDepthStencilSurface)(THIS_ IDirect3DSurface9* pNewZStencil) { return  m_ptr->SetDepthStencilSurface(pNewZStencil); }
	COM_METHOD(HRESULT, GetDepthStencilSurface)(THIS_ IDirect3DSurface9** ppZStencilSurface) { return  m_ptr->GetDepthStencilSurface(ppZStencilSurface); }
	COM_METHOD(HRESULT, BeginScene)(THIS) { return  m_ptr->BeginScene(); }
	COM_METHOD(HRESULT, Clear)(THIS_ DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) { return  m_ptr->Clear(Count, pRects, Flags, Color, Z, Stencil); }
	COM_METHOD(HRESULT, SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) { return  m_ptr->SetTransform(State, pMatrix); }
	COM_METHOD(HRESULT, GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) { return  m_ptr->GetTransform(State, pMatrix); }
	COM_METHOD(HRESULT, MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE a, CONST D3DMATRIX* b) { return  m_ptr->MultiplyTransform(a, b); }
	COM_METHOD(HRESULT, SetViewport)(THIS_ CONST D3DVIEWPORT9* pViewport) { return  m_ptr->SetViewport(pViewport); }
	COM_METHOD(HRESULT, GetViewport)(THIS_ D3DVIEWPORT9* pViewport) { return  m_ptr->GetViewport(pViewport); }
	COM_METHOD(HRESULT, SetMaterial)(THIS_ CONST D3DMATERIAL9* pMaterial) { return  m_ptr->SetMaterial(pMaterial); }
	COM_METHOD(HRESULT, GetMaterial)(THIS_ D3DMATERIAL9* pMaterial) { return  m_ptr->GetMaterial(pMaterial); }
	COM_METHOD(HRESULT, SetLight)(THIS_ DWORD Index, CONST D3DLIGHT9* a) { return  m_ptr->SetLight(Index, a); }
	COM_METHOD(HRESULT, GetLight)(THIS_ DWORD Index, D3DLIGHT9* a) { return  m_ptr->GetLight(Index, a); }
	COM_METHOD(HRESULT, LightEnable)(THIS_ DWORD Index, BOOL Enable) { return  m_ptr->LightEnable(Index, Enable); }
	COM_METHOD(HRESULT, GetLightEnable)(THIS_ DWORD Index, BOOL* pEnable) { return  m_ptr->GetLightEnable(Index, pEnable); }
	COM_METHOD(HRESULT, SetClipPlane)(THIS_ DWORD Index, CONST float* pPlane) { return  m_ptr->SetClipPlane(Index, pPlane); }
	COM_METHOD(HRESULT, GetClipPlane)(THIS_ DWORD Index, float* pPlane) { return  m_ptr->GetClipPlane(Index, pPlane); }
	COM_METHOD(HRESULT, SetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD Value) { return  m_ptr->SetRenderState(State, Value); }
	COM_METHOD(HRESULT, GetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD* pValue) { return  m_ptr->GetRenderState(State, pValue); }
	COM_METHOD(HRESULT, CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB) { return  m_ptr->CreateStateBlock(Type, ppSB); }
	COM_METHOD(HRESULT, BeginStateBlock)(THIS) { return  m_ptr->BeginStateBlock(); }
	COM_METHOD(HRESULT, EndStateBlock)(THIS_ IDirect3DStateBlock9** ppSB) { return  m_ptr->EndStateBlock(ppSB); }
	COM_METHOD(HRESULT, SetClipStatus)(THIS_ CONST D3DCLIPSTATUS9* pClipStatus) { return  m_ptr->SetClipStatus(pClipStatus); }
	COM_METHOD(HRESULT, GetClipStatus)(THIS_ D3DCLIPSTATUS9* pClipStatus) { return  m_ptr->GetClipStatus(pClipStatus); }
	COM_METHOD(HRESULT, GetTexture)(THIS_ DWORD Stage, IDirect3DBaseTexture9** ppTexture) { return  m_ptr->GetTexture(Stage, ppTexture); }
	COM_METHOD(HRESULT, SetTexture)(THIS_ DWORD Stage, IDirect3DBaseTexture9* pTexture) { return  m_ptr->SetTexture(Stage, pTexture); }
	COM_METHOD(HRESULT, GetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) { return  m_ptr->GetTextureStageState(Stage, Type, pValue); }
	COM_METHOD(HRESULT, SetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) { return  m_ptr->SetTextureStageState(Stage, Type, Value); }
	COM_METHOD(HRESULT, GetSamplerState)(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue) { return  m_ptr->GetSamplerState(Sampler, Type, pValue); }
	COM_METHOD(HRESULT, SetSamplerState)(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) { return  m_ptr->SetSamplerState(Sampler, Type, Value); }
	COM_METHOD(HRESULT, ValidateDevice)(THIS_ DWORD* pNumPasses) { return  m_ptr->ValidateDevice(pNumPasses); }
	COM_METHOD(HRESULT, SetPaletteEntries)(THIS_ UINT PaletteNumber, CONST PALETTEENTRY* pEntries) { return  m_ptr->SetPaletteEntries(PaletteNumber, pEntries); }
	COM_METHOD(HRESULT, GetPaletteEntries)(THIS_ UINT PaletteNumber, PALETTEENTRY* pEntries) { return  m_ptr->GetPaletteEntries(PaletteNumber, pEntries); }
	COM_METHOD(HRESULT, SetCurrentTexturePalette)(THIS_ UINT PaletteNumber) { return  m_ptr->SetCurrentTexturePalette(PaletteNumber); }
	COM_METHOD(HRESULT, GetCurrentTexturePalette)(THIS_ UINT *PaletteNumber) { return  m_ptr->GetCurrentTexturePalette(PaletteNumber); }
	COM_METHOD(HRESULT, SetScissorRect)(THIS_ CONST RECT* pRect) { return  m_ptr->SetScissorRect(pRect); }
	COM_METHOD(HRESULT, GetScissorRect)(THIS_ RECT* pRect) { return  m_ptr->GetScissorRect(pRect); }
	COM_METHOD(HRESULT, SetSoftwareVertexProcessing)(THIS_ BOOL bSoftware) { return  m_ptr->SetSoftwareVertexProcessing(bSoftware); }
	COM_METHOD(BOOL, GetSoftwareVertexProcessing)(THIS) { return  m_ptr->GetSoftwareVertexProcessing(); }
	COM_METHOD(HRESULT, SetNPatchMode)(THIS_ float nSegments) { return  m_ptr->SetNPatchMode(nSegments); }
	COM_METHOD(float, GetNPatchMode)(THIS) { return  m_ptr->GetNPatchMode(); }
	COM_METHOD(HRESULT, DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) { return  m_ptr->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount); }
	COM_METHOD(HRESULT, DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE a, INT BaseVertexIndex, UINT MinVertexIndex, UINT  NumVertices, UINT startIndex, UINT primCount) { return  m_ptr->DrawIndexedPrimitive(a, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount); }
	COM_METHOD(HRESULT, DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) { return  m_ptr->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride); }
	COM_METHOD(HRESULT, DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) { return  m_ptr->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride); }
	COM_METHOD(HRESULT, ProcessVertices)(THIS_ UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags) { return  m_ptr->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags); }
	COM_METHOD(HRESULT, CreateVertexDeclaration)(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl) { return  m_ptr->CreateVertexDeclaration(pVertexElements, ppDecl); }
	COM_METHOD(HRESULT, SetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9* pDecl) { return  m_ptr->SetVertexDeclaration(pDecl); }
	COM_METHOD(HRESULT, GetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9** ppDecl) { return  m_ptr->GetVertexDeclaration(ppDecl); }
	COM_METHOD(HRESULT, SetFVF)(THIS_ DWORD FVF) { return  m_ptr->SetFVF(FVF); }
	COM_METHOD(HRESULT, GetFVF)(THIS_ DWORD* pFVF) { return  m_ptr->GetFVF(pFVF); }
	COM_METHOD(HRESULT, CreateVertexShader)(THIS_ CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader) { return  m_ptr->CreateVertexShader(pFunction, ppShader); }
	COM_METHOD(HRESULT, SetVertexShader)(THIS_ IDirect3DVertexShader9* pShader) { return  m_ptr->SetVertexShader(pShader); }
	COM_METHOD(HRESULT, GetVertexShader)(THIS_ IDirect3DVertexShader9** ppShader) { return  m_ptr->GetVertexShader(ppShader); }
	COM_METHOD(HRESULT, SetVertexShaderConstantF)(THIS_ UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) { return  m_ptr->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
	COM_METHOD(HRESULT, GetVertexShaderConstantF)(THIS_ UINT StartRegister, float* pConstantData, UINT Vector4fCount) { return  m_ptr->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
	COM_METHOD(HRESULT, SetVertexShaderConstantI)(THIS_ UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) { return  m_ptr->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
	COM_METHOD(HRESULT, GetVertexShaderConstantI)(THIS_ UINT StartRegister, int* pConstantData, UINT Vector4iCount) { return  m_ptr->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
	COM_METHOD(HRESULT, SetVertexShaderConstantB)(THIS_ UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount) { return  m_ptr->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount); }
	COM_METHOD(HRESULT, GetVertexShaderConstantB)(THIS_ UINT StartRegister, BOOL* pConstantData, UINT BoolCount) { return  m_ptr->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount); }
	COM_METHOD(HRESULT, SetStreamSource)(THIS_ UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) { return  m_ptr->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride); }
	COM_METHOD(HRESULT, GetStreamSource)(THIS_ UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride) { return  m_ptr->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride); }
	COM_METHOD(HRESULT, SetStreamSourceFreq)(THIS_ UINT StreamNumber, UINT Setting) { return  m_ptr->SetStreamSourceFreq(StreamNumber, Setting); }
	COM_METHOD(HRESULT, GetStreamSourceFreq)(THIS_ UINT StreamNumber, UINT* pSetting) { return  m_ptr->GetStreamSourceFreq(StreamNumber, pSetting); }
	COM_METHOD(HRESULT, SetIndices)(THIS_ IDirect3DIndexBuffer9* pIndexData) { return  m_ptr->SetIndices(pIndexData); }
	COM_METHOD(HRESULT, GetIndices)(THIS_ IDirect3DIndexBuffer9** ppIndexData) { return  m_ptr->GetIndices(ppIndexData); }
	COM_METHOD(HRESULT, CreatePixelShader)(THIS_ CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader) { return  m_ptr->CreatePixelShader(pFunction, ppShader); }
	COM_METHOD(HRESULT, SetPixelShader)(THIS_ IDirect3DPixelShader9* pShader) { return  m_ptr->SetPixelShader(pShader); }
	COM_METHOD(HRESULT, GetPixelShader)(THIS_ IDirect3DPixelShader9** ppShader) { return  m_ptr->GetPixelShader(ppShader); }
	COM_METHOD(HRESULT, SetPixelShaderConstantF)(THIS_ UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) { return  m_ptr->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
	COM_METHOD(HRESULT, GetPixelShaderConstantF)(THIS_ UINT StartRegister, float* pConstantData, UINT Vector4fCount) { return  m_ptr->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
	COM_METHOD(HRESULT, SetPixelShaderConstantI)(THIS_ UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) { return  m_ptr->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
	COM_METHOD(HRESULT, GetPixelShaderConstantI)(THIS_ UINT StartRegister, int* pConstantData, UINT Vector4iCount) { return  m_ptr->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
	COM_METHOD(HRESULT, SetPixelShaderConstantB)(THIS_ UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount) { return  m_ptr->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount); }
	COM_METHOD(HRESULT, GetPixelShaderConstantB)(THIS_ UINT StartRegister, BOOL* pConstantData, UINT BoolCount) { return  m_ptr->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount); }
	COM_METHOD(HRESULT, DrawRectPatch)(THIS_ UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo) { return  m_ptr->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo); }
	COM_METHOD(HRESULT, DrawTriPatch)(THIS_ UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo) { return  m_ptr->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo); }
	COM_METHOD(HRESULT, DeletePatch)(THIS_ UINT Handle) { return  m_ptr->DeletePatch(Handle); }
	COM_METHOD(HRESULT, CreateQuery)(THIS_ D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery) { return  m_ptr->CreateQuery(Type, ppQuery); }
#pragma endregion

};