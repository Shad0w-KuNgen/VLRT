#pragma once
#include <Windows.h>

#include "types.h"
#include "offset.h"

uint64_t base = 0;

template <typename T>
T stub(uint64_t Address)
{
	return reinterpret_cast<T(*)(uint64_t)>(base + offset::trigger_veh)(Address - 0x8);
}