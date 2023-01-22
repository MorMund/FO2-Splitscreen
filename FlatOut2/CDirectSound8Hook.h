#pragma once
#include "stdafx.h"

class CDirectSound8Hook : public IDirectSound8
{
private:
    IDirectSound8* m_ptr;
public:
    CDirectSound8Hook(IDirectSound8* ptr);

    HRESULT _stdcall CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER* ppDSBuffer, LPUNKNOWN pUnkOuter);
    // IUnknown methods
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) { /*TODO*/ return m_ptr->QueryInterface(riid, ppvObj); }
    STDMETHOD_(ULONG, AddRef)(THIS) { /*TODO*/ return m_ptr->AddRef(); }
    STDMETHOD_(ULONG, Release)(THIS) { /*TODO*/ return m_ptr->Release(); }

    // IDirectSound methods
    STDMETHOD(GetCaps)              (THIS_ _Out_ LPDSCAPS pDSCaps) { /*TODO*/ return m_ptr->GetCaps(pDSCaps); };
    STDMETHOD(DuplicateSoundBuffer) (THIS_ _In_ LPDIRECTSOUNDBUFFER pDSBufferOriginal, _Out_ LPDIRECTSOUNDBUFFER* ppDSBufferDuplicate) { /*TODO*/ return m_ptr->DuplicateSoundBuffer(pDSBufferOriginal, ppDSBufferDuplicate); };
    STDMETHOD(SetCooperativeLevel)  (THIS_ HWND hwnd, DWORD dwLevel) { /*TODO*/ return m_ptr->SetCooperativeLevel(hwnd, dwLevel); };
    STDMETHOD(Compact)              (THIS) { /*TODO*/ return m_ptr->Compact(); };
    STDMETHOD(GetSpeakerConfig)     (THIS_ _Out_ LPDWORD pdwSpeakerConfig) { /*TODO*/ return m_ptr->GetSpeakerConfig(pdwSpeakerConfig); };
    STDMETHOD(SetSpeakerConfig)     (THIS_ DWORD dwSpeakerConfig) { /*TODO*/ return m_ptr->SetSpeakerConfig(dwSpeakerConfig); };
    STDMETHOD(Initialize)           (THIS_ _In_opt_ LPCGUID pcGuidDevice) { /*TODO*/ return m_ptr->Initialize(pcGuidDevice); };

    // IDirectSound8 methods
    STDMETHOD(VerifyCertification)  (THIS_ _Out_ LPDWORD pdwCertified) { /*TODO*/ return m_ptr->VerifyCertification(pdwCertified); };
};

