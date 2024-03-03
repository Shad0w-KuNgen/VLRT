#pragma once
#include "types.h"

namespace hook
{
	void vmt(void* addr, void* pDes, int index, void** ret, uint64_t vmt_dst)
	{
		auto vtable = *(uint64_t**)addr;
		int methods = 0;
		do {
			++methods;
		} while (*(uint64_t*)((uint64_t)vtable + (methods * 0x8)));
		auto vtable_buf = (uint64_t*)VirtualAlloc((void*)vmt_dst, methods* 0x8, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);// new uint64_t[methods];
		for (auto count = 0; count < methods; ++count) {
			vtable_buf[count] = *(uint64_t*)((uint64_t)vtable + (count * 0x8));

			*ret = (void*)vtable[index];

			vtable_buf[index] = (uint64_t)(pDes);
			*(uint64_t**)addr = vtable_buf;
		}
	}
}