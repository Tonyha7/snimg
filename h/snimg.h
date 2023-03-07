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
#ifndef H_IMAGE
#define H_IMAGE

#include "system.h"

/////////////////////////////////////////////////////////////////////////////// 
//
// Some OEMs (like Lenovo) use a small file called "sn.img" to limit a user's
// ability to enable bootloader unlocking.
// 
// To enable unlocking, the command is usually:  fastboot flash unlock sn.img
// 
// The bootloader takes the sn.img file and passes it to VBVerifyImage() for authentication.
// It fibs to VBVerifyImage() and tells it that this is a 0x1000-byte boot image.
// 
// If successful, this authorises the command "fastboot oem unlock-go" to unlock the bootloader.
// 
// NOTE: not all Android bootloaders (LinuxLoader aka abl.elf) issued by these OEMs
//       contain the "fastboot flash unlock" or "fastboot oem unlock-go" commands.
// 
/////////////////////////////////////////////////////////////////////////////// 
// 
// Every sn.img is a signed Android Boot Image.
// 
// It consists of the following components starting at address:
//   0x0000    boot_header
//   0x0240    boot_header.id (where the hash is stored)
//   0x0800    device serial number
//   0x1000    X.509 cert chain and signature
//
// The only differences between sn.img files are at 0x240 (hash) and 0x800 (serialno).
// Everything else is the same - including the Lenovo certificate/signature.
// See InsertHash() in snimg.cpp for information about how the hash is calculated.
//

#define OFFSET_HASH     0x0240
#define OFFSET_SERIALNO 0x0800
#define OFFSET_SIGN     0x1000

#define IMAGE_SIZE_UNSIGNED   0x1000	// size of the sn.img boot image
#define SIZEOF_SIGNATURE      0x050a    // size of X509 cert & signature
#define IMAGE_SIZE_SIGNED     (IMAGE_SIZE_UNSIGNED + SIZEOF_SIGNATURE)	// size of the sn.img file with certs
#define SIZEOF_IMAGE  IMAGE_SIZE_SIGNED

// function declarations
extern int WriteHeader(void);
extern int InsertHash(void);
//extern int InsertSerialNo(void);
//extern int SignImage(void);

// var declarations (defined in snimg.cpp)
// "the blob" is the x509 cert and signature at the end of the sn.img file from 0x1000 to the end (0x1509)
extern BYTE blob[SIZEOF_SIGNATURE];

// declare globals (defined in main.cpp)
extern char     g_serialno[9];  // null-terminated 8-char serial number
extern PBYTE    g_sn_img;       // pointer to sn.img in memory 

#endif
