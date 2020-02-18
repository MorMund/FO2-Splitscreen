#pragma once
#include "stdafx.h"
#define BINPATCH_CALL_INSTRUCTION_SIZE 6
enum class BINPATCH_REGISTER_OFFSETS: BYTE
{
	eax=0,
	ecx=1,
	edx=2,
	ebx=3,
	esp=4,
	ebp=5,
	esi=6,
	edi=7,
	max=edi
};
bool PatchCallInstruction(PBYTE codeAddr, const BYTE expected[BINPATCH_CALL_INSTRUCTION_SIZE], PVOID* newAddress);
bool PatchPushInstruction(PBYTE codeAddr, const BYTE expected, BINPATCH_REGISTER_OFFSETS registerIndex);
