#pragma once
#include <Windows.h>

extern "C" int _fltused = 0;
#pragma data_seg()
#pragma comment(linker, "/merge:.CRT=.rdata")

#define static_var(type, name, function) static type name{};\
if (!name) name = function;

extern "C" void __chkstk() { }
extern "C" void __CxxFrameHandler4() { }

#pragma intrinsic(memset) // weird fix to compile bug in release
extern void* __cdecl memset(void*, int, size_t);

#pragma function(memset)
void* memset(void* dest, int c, size_t count) {
	char* b = (char*)dest;
	while (count--)
		*b++ = (char)c;

	return dest;
}

int m_wtolower(int c)
{
	if (c >= L'A' && c <= L'Z') return c - L'A' + L'a';
	if (c >= L'¬¡' && c <= L'¬Á') return c - L'¬¡' + L'¬Ñ';
	if (c == L'¬§') return L'¬×';
	return c;
}
unsigned m_wstrlen(const wchar_t* str)
{
	int cnt = 0;
	if (!str)
		return 0;
	for (; *str != '\0'; ++str)
		++cnt;
	return cnt;
}
int m_wstrcmp(const wchar_t* cs, const wchar_t* ct)
{
	if (cs && ct)
	{
		while (*cs == *ct)
		{
			if (*cs == 0 && *ct == 0) return 0;
			if (*cs == 0 || *ct == 0) break;
			cs++;
			ct++;
		}
		return *cs - *ct;
	}
	return -1;
}
int m_wstricmp(const wchar_t* cs, const wchar_t* ct)
{
	if (cs && ct)
	{
		while (m_wtolower(*cs) == m_wtolower(*ct))
		{
			if (*cs == 0 && *ct == 0) return 0;
			if (*cs == 0 || *ct == 0) break;
			cs++;
			ct++;
		}
		return m_wtolower(*cs) - m_wtolower(*ct);
	}
	return -1;
}
void* m_memcpy(void* dest, void* src, unsigned int len)
{
	unsigned int i;
	char* char_src = (char*)src;
	char* char_dest = (char*)dest;
	for (i = 0; i < len; i++) {
		char_dest[i] = char_src[i];
	}
	return dest;
}
void* m_memset(void* dest, char c, unsigned int len)
{
	unsigned int i;
	unsigned int fill;
	unsigned int chunks = len / sizeof(fill);
	char* char_dest = (char*)dest;
	unsigned int* uint_dest = (unsigned int*)dest;
	fill = (c << 24) + (c << 16) + (c << 8) + c;

	for (i = len; i > chunks * sizeof(fill); i--) {
		char_dest[i - 1] = c;
	}

	for (i = chunks; i > 0; i--) {
		uint_dest[i - 1] = fill;
	}

	return dest;
}
int _stdcall m_is_bad_read_ptr(const void* lp, unsigned int ucb)
{
	return (ucb && (!lp || (char*)lp + ucb - 1 < lp));
}
int _stdcall m_is_bad_code_ptr(FARPROC lpfn)
{
	return m_is_bad_read_ptr(lpfn, 1u);
}
char* __cdecl m_strstr(const char* str1, const char* str2)
{
	char* cp = (char*)str1;
	char* s1, * s2;

	if (!*str2)
		return((char*)str1);

	while (*cp)
	{
		s1 = cp;
		s2 = (char*)str2;

		while (*s1 && *s2 && !(*s1 - *s2))
			s1++, s2++;

		if (!*s2)
			return(cp);

		cp++;
	}

	return(0);
}
char* __cdecl m_strchr(const char* string, int ch)
{
	while (*string && *string != (char)ch)
		string++;
	if (*string == (char)ch)
		return((char*)string);
	return(NULL);
}
int m_strcmp(const char* src, const char* dst)
{
	int ret = 0;
	unsigned char* p1 = (unsigned char*)src;
	unsigned char* p2 = (unsigned char*)dst;
	while (!(ret = *p1 - *p2) && *p2)
		++p1, ++p2;

	if (ret < 0)
		ret = -1;
	else if (ret > 0)
		ret = 1;

	return ret;
}
char* m_strcpy(char* dest, const char* src)
{
	char* ret = dest;
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
	return ret;
}
unsigned m_strlen(const char* str)
{
	int cnt = 0;
	if (!str)
		return 0;
	for (; *str != '\0'; ++str)
		++cnt;
	return cnt;
}

void sprintf(wchar_t* buffer, const wchar_t* format, ...)
{
	va_list v1;
	va_start(v1, format);
	wvsprintf(buffer, format, v1);
	va_end(v1);
}

void* __cdecl operator new(size_t size)
{
	void* res = HeapAlloc(GetProcessHeap(), 0, size);
	if (res)
		return res;
	return NULL;
}

void operator delete(void* p)
{
	if (p != NULL)
		HeapFree(GetProcessHeap(), 0, p);
}
void* __cdecl operator new[](size_t n)
{
	return operator new(n);
}

void  __cdecl operator delete[](void* p)
{
	operator delete(p);
}

void  __cdecl operator delete[](void* p, size_t)
{
	operator delete[](p);
}

void  __cdecl operator delete(void* p, size_t)
{
	operator delete(p);
}