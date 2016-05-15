#pragma once
#ifndef _PEMACRO_H_
#define _PEMACRO_H_

#if defined(DEBUG) || defined(_DEBUG)
#define PE_DEBUG_MODE
#endif

#define PE_BEGIN namespace pe {
#define PE_END }

#define PE_DIRECTX

#define MAX_NAME_CHAR_COUNT 1024

#endif