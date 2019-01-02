#pragma once

#ifdef Engine_Export
#define Engine_DLL __declspec(dllexport)
#else
#define Engine_DLL __declspec(dllimport)
#endif