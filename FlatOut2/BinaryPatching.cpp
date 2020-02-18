#include "stdafx.h"
#include "BinaryPatching.h"

bool PatchCallInstruction(PBYTE codeAddr, const BYTE expected[BINPATCH_CALL_INSTRUCTION_SIZE], PVOID* newAddress)
{
	if (expected[0] != 0xFFU || expected[1] != 0x15U) {
		throw std::exception("Cannot patch invalid call opcode.");
	}

	if (strncmp((char*)(codeAddr), (char*)expected, BINPATCH_CALL_INSTRUCTION_SIZE) != 0) {
		return false;
	}

	DWORD old = 0;
	if (!VirtualProtect(codeAddr, BINPATCH_CALL_INSTRUCTION_SIZE, PAGE_EXECUTE_READWRITE, &old)) {
		throw std::exception("Failed to change page access permission for binary patching.");
	}

	PINT32 addressLocation = (PINT32)(codeAddr + 2);
	*addressLocation = (INT32)newAddress;
	VirtualProtect(codeAddr, BINPATCH_CALL_INSTRUCTION_SIZE, old, &old);
	return true;
}

bool PatchPushInstruction(PBYTE codeAddr, const BYTE expected, BINPATCH_REGISTER_OFFSETS registerIndex)
{
	if (static_cast<BYTE>(registerIndex) < 0 || registerIndex > BINPATCH_REGISTER_OFFSETS::max) {
		throw std::exception("Register index out of Range");
	}

	if (*codeAddr != expected) {
		return false;
	}

	DWORD old = 0;
	if (!VirtualProtect(codeAddr, 1, PAGE_EXECUTE_READWRITE, &old)) {
		throw std::exception("Failed to change page access permission for binary patching.");
	}
	*codeAddr = 0x50 + static_cast<BYTE>(registerIndex);
	VirtualProtect(codeAddr, 1, old, &old);
	return true;
}
