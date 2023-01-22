#include "stdafx.h"
#include "CDirectSound8Hook.h"

CDirectSound8Hook::CDirectSound8Hook(IDirectSound8* ptr)
{
	if (ptr == NULL) {
		throw std::exception("Error in CDirectSound8Hook::CDirectSound8Hook. DirectSound8 object pointer was null!");
	}

	m_ptr = ptr;
}

HRESULT _stdcall CDirectSound8Hook::CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER* ppDSBuffer, LPUNKNOWN pUnkOuter)
{
	// The sound buffer create with the `DSBCAPS_PRIMARYBUFFER` flag is only used to call QueryInterface on it @0x0062a10c.
	if (pcDSBufferDesc->dwFlags & DSBCAPS_PRIMARYBUFFER) {
		return m_ptr->CreateSoundBuffer(pcDSBufferDesc, ppDSBuffer, pUnkOuter);
	}

	(const_cast<DSBUFFERDESC*>(pcDSBufferDesc))->dwFlags |= DSBCAPS_GLOBALFOCUS;
	return m_ptr->CreateSoundBuffer(pcDSBufferDesc, ppDSBuffer, pUnkOuter);
}
