#include "stdafx.h"
#include "CDIDevice8Hook.h"

BOOL CDIDevice8Hook::IsButtonPress(DWORD button)
{
	if (button < 128)
	{
		return m_currentJoyState.rgbButtons[button] && !m_lastJoyState.rgbButtons[button];
	}
	else
	{
		return false;
	}
}

void CDIDevice8Hook::GetStickDirection(LPINT upDown, LPINT rightLeft)
{
	signed short joystickXPos = (signed short)m_currentJoyState.lX;
	signed short joystickYPos = (signed short)m_currentJoyState.lY;
	if (joystickXPos > XBOX_JOY_THRESHOLD)
	{
		*rightLeft = 1;
	}
	else if (joystickXPos < -XBOX_JOY_THRESHOLD)
	{
		*rightLeft = -1;
	}
	else if (abs(joystickXPos) < XBOX_JOY_R_THRESHOLD)
	{
		*rightLeft = 0;
	}
	else
	{
		*rightLeft = 2;
	}

	if (joystickYPos > XBOX_JOY_THRESHOLD)
	{
		*upDown = 1;
	}
	else if (joystickYPos < -XBOX_JOY_THRESHOLD)
	{
		*upDown = -1;
	}
	else if (abs(joystickYPos) < XBOX_JOY_R_THRESHOLD)
	{
		*upDown = 0;
	}
	else
	{
		*upDown = 2;
	}
}

HRESULT CDIDevice8Hook::SetCooperativeLevel(HWND hwnd, DWORD dwFlags)
{
	HRESULT res = m_ptr->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	return res;
}

HRESULT CDIDevice8Hook::GetDeviceState(DWORD cbData, LPVOID lpvData)
{
	HRESULT res = m_ptr->GetDeviceState(cbData, lpvData);
	memcpy_s(&m_lastJoyState, sizeof(m_lastJoyState), &m_currentJoyState, sizeof(m_currentJoyState));
	memcpy_s(&m_currentJoyState, sizeof(m_currentJoyState), lpvData, sizeof(DIJOYSTATE2));
	return res;
}
