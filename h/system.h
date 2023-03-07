/********************************************************************************
  Copyright 2023 Yahoo Mike from XDA - developers.com
  This file is part of snimg.
 
  snimg is free software : you can redistribute it and /or modify it under the
  terms of the GNU General Public License as published by the Free Software
  Foundation, either version 3 of the License, or (at your option) any later
  version.
 
  snimg is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License
  for more details.
 
  You should have received a copy of the GNU General Public License along
  with snimg.If not, see < https://www.gnu.org/licenses/>.
********************************************************************************/
#ifndef H_SYSTEM
#define H_SYSTEM 

//#include <stdio.h>
//#include <wchar.h>

//
// which OS are we compiling for ?
//
// based on https://stackoverflow.com/a/8249232/13546494 
#if defined(__linux) || defined(__linux__) || defined(linux)
#define OS_LINUX
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64)
#define OS_WINDOWS
//#elif defined(__APPLE__) || (__MACH__)
//#define OS_MACOS
#endif

//////////////////////////////////////////////////////
// WINDOWS-specific directives
#ifdef OS_WINDOWS

#include <windows.h>

#endif

//////////////////////////////////////////////////////
// LINUX-specific directives
#ifdef OS_LINUX

#include <stdint.h>
typedef __uint8_t   BYTE;
typedef BYTE*       PBYTE;

#define NO_ERROR             0

//#include <linux/limits.h>
//#define MAX_PATH PATH_MAX

//typedef __uint16_t  WORD;
//typedef __uint32_t  DWORD;
//typedef __int32_t   LONG;
//typedef __uint64_t  ULONGLONG;

#endif

#endif