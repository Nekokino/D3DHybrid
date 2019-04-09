#pragma once

#ifdef Engine_Export
#define Engine_DLL __declspec(dllexport)
#else
#define Engine_DLL __declspec(dllimport)
#endif

#define tassert(VALUE) assert(!VALUE)
#define tassertmsg(VALUE, MSG) assert((MSG, !VALUE))
#define DXNamespace using namespace DirectX