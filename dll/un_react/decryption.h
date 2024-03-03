#pragma once
#include "memory.h"
#include "class.h"

#pragma warning (push)
#pragma warning (disable : 4244)

namespace decrypt
{
	uint64_t uworld(uint32_t key, uint64_t* state)
	{
		uint64_t v19; // r11
		uint64_t v20; // r8
		uint64_t v21; // r9
		uint32_t v22; // er10
		uint64_t v23; // rcx
		uint64_t v24; // rdx
		uint64_t v25; // rcx
		int32_t v26; // ebx
		uint32_t v27; // ecx
		int64_t v28; // rax
		uint64_t v29; // r8
		uint64_t v30; // r8
		uint64_t v31; // rcx
		uint64_t v32; // rdx
		uint64_t v33; // rcx

		v19 = 2685821657736338717i64
			* ((unsigned int)key ^ (unsigned int)(key << 25) ^ (((unsigned int)key ^ ((unsigned __int64)(unsigned int)key >> 15)) >> 12))
			% 7;
		v20 = state[v19];
		v21 = (2685821657736338717i64
			* ((unsigned int)key ^ (unsigned int)(key << 25) ^ (((unsigned int)key ^ ((unsigned __int64)(unsigned int)key >> 15)) >> 12))) >> 32;
		v22 = (unsigned int)v19 % 7;
		if (!((unsigned int)v19 % 7))
		{
			v23 = (2 * (v20 - (unsigned int)(v21 - 1))) ^ ((2 * (v20 - (unsigned int)(v21 - 1))) ^ ((v20
				- (unsigned int)(v21 - 1)) >> 1)) & 0x5555555555555555i64;
			v24 = (4 * v23) ^ ((4 * v23) ^ (v23 >> 2)) & 0x3333333333333333i64;
			v25 = (16 * v24) ^ ((16 * v24) ^ (v24 >> 4)) & 0xF0F0F0F0F0F0F0Fi64;
			v20 = _rotl64((v25 << 8) ^ ((v25 << 8) ^ (v25 >> 8)) & 0xFF00FF00FF00FFi64, 32);
		LABEL_26:
			v26 = 2 * v19;
			goto LABEL_27;
		}
		if (v22 != 1)
			goto LABEL_26;
		v26 = 2 * v19;
		v20 = _rotl64(v20 - (unsigned int)(2 * v19 + v21), (unsigned __int8)(((int)v21 + (int)v19) % 0x3Fu) + 1);
	LABEL_27:
		v27 = v26 + v21;
		if (v22 == 2)
			v20 = ~(v20 - v27);
		switch (v22)
		{
		case 3u:
			v28 = 2 * ((2 * v20) ^ ((2 * v20) ^ (v20 >> 1)) & 0x5555555555555555i64);
			v20 = v28 ^ (v28 ^ (((2 * v20) ^ ((2 * v20) ^ (v20 >> 1)) & 0x5555555555555555i64) >> 1)) & 0x5555555555555555i64;
			break;
		case 4u:
			v29 = _rotr64(v20, (unsigned __int8)(v27 % 0x3F) + 1);
			v20 = (2 * v29) ^ ((2 * v29) ^ (v29 >> 1)) & 0x5555555555555555i64;
			break;
		case 5u:
			v30 = _rotr64(v20, (unsigned __int8)(v27 % 0x3F) + 1);
			v31 = (2 * v30) ^ ((2 * v30) ^ (v30 >> 1)) & 0x5555555555555555i64;
			v32 = (4 * v31) ^ ((4 * v31) ^ (v31 >> 2)) & 0x3333333333333333i64;
			v33 = (16 * v32) ^ ((16 * v32) ^ (v32 >> 4)) & 0xF0F0F0F0F0F0F0Fi64;
			v20 = _rotl64((v33 << 8) ^ ((v33 << 8) ^ (v33 >> 8)) & 0xFF00FF00FF00FFi64, 32);
			break;
		case 6u:
			v20 = ~v20 - (unsigned int)(v21 + v19);
			break;
		}
		return v20 ^ (unsigned int)key;
	}
}

#pragma warning (pop)