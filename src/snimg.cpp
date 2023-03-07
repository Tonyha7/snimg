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
  with snimg. If not, see < https://www.gnu.org/licenses/>.
********************************************************************************/
#include <string.h>
#include <stdio.h>

#include <openssl/sha.h>    // OpenSSL for hashing

#include "snimg.h"
#include "caf.h"        // AOSP/CAF definitions
#include "system.h"     // OS-specific calls

// var declarations
// "the blob" that contains the x509 certificate and signature at the end of all sn.img images
BYTE blob[SIZEOF_SIGNATURE] = {
        0x30, 0x82, 0x05, 0x06, 0x02, 0x01, 0x01, 0x30, 0x82, 0x03, 0xe1, 0x30, 0x82, 0x02, 0xc9, 0xa0,
        0x03, 0x02, 0x01, 0x02, 0x02, 0x09, 0x00, 0x80, 0x49, 0x40, 0xde, 0x79, 0x16, 0x4b, 0x1a, 0x30,
        0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x30, 0x81,
        0x86, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x43, 0x4e, 0x31, 0x10,
        0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x07, 0x42, 0x45, 0x49, 0x4a, 0x49, 0x4e, 0x47,
        0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x06, 0x4c, 0x65, 0x6e, 0x6f, 0x76,
        0x6f, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x07, 0x41, 0x6e, 0x64, 0x72,
        0x6f, 0x69, 0x64, 0x31, 0x0c, 0x30, 0x0a, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x03, 0x50, 0x41,
        0x44, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x07, 0x41, 0x6e, 0x64, 0x72,
        0x6f, 0x69, 0x64, 0x31, 0x22, 0x30, 0x20, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01,
        0x09, 0x01, 0x16, 0x13, 0x63, 0x68, 0x65, 0x6e, 0x67, 0x78, 0x74, 0x61, 0x40, 0x6c, 0x65, 0x6e,
        0x6f, 0x76, 0x6f, 0x2e, 0x63, 0x6f, 0x6d, 0x30, 0x1e, 0x17, 0x0d, 0x32, 0x30, 0x30, 0x36, 0x32,
        0x39, 0x30, 0x37, 0x34, 0x38, 0x31, 0x34, 0x5a, 0x17, 0x0d, 0x34, 0x37, 0x31, 0x31, 0x31, 0x35,
        0x30, 0x37, 0x34, 0x38, 0x31, 0x34, 0x5a, 0x30, 0x81, 0x86, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03,
        0x55, 0x04, 0x06, 0x13, 0x02, 0x43, 0x4e, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x08,
        0x0c, 0x07, 0x42, 0x45, 0x49, 0x4a, 0x49, 0x4e, 0x47, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03, 0x55,
        0x04, 0x07, 0x0c, 0x06, 0x4c, 0x65, 0x6e, 0x6f, 0x76, 0x6f, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03,
        0x55, 0x04, 0x0a, 0x0c, 0x07, 0x41, 0x6e, 0x64, 0x72, 0x6f, 0x69, 0x64, 0x31, 0x0c, 0x30, 0x0a,
        0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x03, 0x50, 0x41, 0x44, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03,
        0x55, 0x04, 0x03, 0x0c, 0x07, 0x41, 0x6e, 0x64, 0x72, 0x6f, 0x69, 0x64, 0x31, 0x22, 0x30, 0x20,
        0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x01, 0x16, 0x13, 0x63, 0x68, 0x65,
        0x6e, 0x67, 0x78, 0x74, 0x61, 0x40, 0x6c, 0x65, 0x6e, 0x6f, 0x76, 0x6f, 0x2e, 0x63, 0x6f, 0x6d,
        0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01,
        0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0f, 0x00, 0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01, 0x01,
        0x00, 0xb7, 0x44, 0x8b, 0xed, 0x70, 0x73, 0x0f, 0xed, 0x28, 0x40, 0x22, 0x0e, 0x2b, 0xf7, 0x8c,
        0x00, 0x7d, 0x67, 0xfe, 0xe8, 0xb2, 0x8d, 0xf8, 0xa6, 0xc0, 0x9c, 0x8c, 0xd0, 0x38, 0x75, 0xaf,
        0xcd, 0xfe, 0xdf, 0xb0, 0xc8, 0xfa, 0x68, 0x69, 0xe2, 0x96, 0x49, 0x62, 0xf9, 0x09, 0xb0, 0xcc,
        0x01, 0x9b, 0x52, 0xd0, 0x55, 0x4d, 0x9a, 0x20, 0xef, 0x34, 0x4f, 0x2b, 0xa4, 0x5d, 0x93, 0xf6,
        0x82, 0xc9, 0xa0, 0xd5, 0x56, 0x08, 0x44, 0x38, 0xfc, 0x38, 0x32, 0xd0, 0xe4, 0x48, 0x6c, 0xdc,
        0x28, 0x02, 0x38, 0x94, 0xe3, 0x9a, 0x00, 0x15, 0x44, 0x8f, 0xe2, 0xde, 0x52, 0x6d, 0x3d, 0xe1,
        0x93, 0x58, 0x77, 0xcb, 0x9d, 0xd9, 0xcf, 0xd1, 0x92, 0x49, 0xef, 0xfe, 0x91, 0xfc, 0x1f, 0x28,
        0x0c, 0x34, 0x0b, 0x61, 0x02, 0x9d, 0x43, 0xb2, 0x98, 0xc2, 0xac, 0xfd, 0x90, 0x9c, 0x88, 0x8d,
        0xb3, 0xfd, 0xed, 0x47, 0x08, 0x58, 0xea, 0xd4, 0xec, 0xc0, 0xf0, 0x5f, 0x19, 0x5a, 0x79, 0x02,
        0xda, 0xea, 0x05, 0x21, 0x86, 0xc9, 0xd0, 0xab, 0xc2, 0xb8, 0x99, 0x45, 0x48, 0xac, 0xe8, 0xda,
        0x1e, 0xac, 0xfc, 0x0e, 0x70, 0xaf, 0xa2, 0xf2, 0x2b, 0x75, 0x52, 0xaf, 0x81, 0x58, 0x8d, 0x8e,
        0xa1, 0x0a, 0x34, 0x2c, 0x37, 0x2c, 0xbd, 0x34, 0x7f, 0x3b, 0x13, 0xbe, 0xd7, 0x25, 0xdd, 0x7b,
        0xdf, 0xc1, 0x23, 0x83, 0x7e, 0x35, 0x99, 0x48, 0xd9, 0x60, 0x84, 0xde, 0x1a, 0x95, 0x46, 0x39,
        0x41, 0x74, 0x1a, 0xc5, 0x29, 0xfd, 0x21, 0xe3, 0x01, 0xcd, 0xff, 0xe0, 0x28, 0xb8, 0xec, 0x1a,
        0xe6, 0x18, 0xb6, 0x3d, 0x11, 0x4c, 0x49, 0x54, 0x91, 0x51, 0xb2, 0xb3, 0xac, 0x5e, 0x8b, 0x5c,
        0x51, 0x42, 0x9b, 0x50, 0x89, 0x0e, 0xfd, 0x08, 0xbe, 0x78, 0xcd, 0x4e, 0x85, 0x13, 0x05, 0x6f,
        0x29, 0x02, 0x03, 0x01, 0x00, 0x01, 0xa3, 0x50, 0x30, 0x4e, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d,
        0x0e, 0x04, 0x16, 0x04, 0x14, 0x1b, 0x3b, 0x83, 0x08, 0x2f, 0x5d, 0x29, 0x08, 0xc3, 0x2b, 0xf8,
        0x0e, 0xa9, 0x07, 0x9e, 0x34, 0x92, 0xda, 0xee, 0x62, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23,
        0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x1b, 0x3b, 0x83, 0x08, 0x2f, 0x5d, 0x29, 0x08, 0xc3, 0x2b,
        0xf8, 0x0e, 0xa9, 0x07, 0x9e, 0x34, 0x92, 0xda, 0xee, 0x62, 0x30, 0x0c, 0x06, 0x03, 0x55, 0x1d,
        0x13, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xff, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
        0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x07, 0xa1, 0x75, 0xd5,
        0xe6, 0xf6, 0xe1, 0xf6, 0x7c, 0x4b, 0x1b, 0x7c, 0xc2, 0xa1, 0x40, 0x0e, 0x27, 0x17, 0x6e, 0xa7,
        0x96, 0x7f, 0x93, 0x3e, 0x40, 0xb1, 0x11, 0xa0, 0x12, 0x83, 0x5b, 0x25, 0x20, 0x7d, 0xad, 0x9e,
        0xdf, 0x6a, 0x3f, 0x22, 0x3f, 0xad, 0xf9, 0xb3, 0x02, 0x8d, 0xb2, 0x8c, 0x35, 0x28, 0xad, 0xc3,
        0x6d, 0x24, 0xa2, 0x12, 0x74, 0xa0, 0xa8, 0xa5, 0xe1, 0xbe, 0x48, 0xf5, 0x2a, 0xbf, 0xc6, 0xd6,
        0xb0, 0xcb, 0x77, 0x20, 0xb4, 0x0b, 0x07, 0x93, 0xad, 0x7c, 0xf6, 0x51, 0x05, 0x31, 0x26, 0x5f,
        0x12, 0x70, 0x1d, 0xba, 0xab, 0x23, 0x1c, 0x5c, 0xb4, 0xb5, 0x51, 0x44, 0xca, 0x02, 0xae, 0x03,
        0xb4, 0x89, 0xad, 0xd0, 0xad, 0xa2, 0xd5, 0x4d, 0x88, 0x9c, 0x9d, 0x12, 0x39, 0x79, 0xb4, 0x89,
        0x05, 0x8d, 0x34, 0xf5, 0x52, 0x10, 0x79, 0xd3, 0x4c, 0x33, 0xa0, 0x44, 0x5a, 0x2d, 0x6e, 0x27,
        0x81, 0xbf, 0x68, 0xd9, 0x96, 0xa4, 0x8d, 0xf2, 0xd2, 0x82, 0x22, 0xae, 0x9a, 0x04, 0x1e, 0x00,
        0xa0, 0x9c, 0x3a, 0x45, 0xe6, 0x3a, 0x25, 0xb3, 0x00, 0x51, 0x27, 0x66, 0xd2, 0x98, 0x16, 0xfa,
        0x70, 0xde, 0x8b, 0x85, 0xf7, 0x2b, 0x45, 0x9c, 0xd1, 0x3f, 0x80, 0x64, 0x0f, 0xc8, 0x7e, 0xd5,
        0x69, 0x01, 0xfb, 0x48, 0x74, 0x9f, 0x7e, 0xd3, 0xa9, 0xf5, 0x86, 0x95, 0xae, 0xc5, 0xa4, 0xe7,
        0xa3, 0x6e, 0x1a, 0x51, 0xf4, 0x67, 0x98, 0x2e, 0x38, 0xa4, 0xd8, 0xc2, 0x6f, 0x1f, 0xaf, 0xc1,
        0x85, 0x7b, 0xc1, 0x6a, 0x73, 0xfa, 0xfb, 0x59, 0x8e, 0xa2, 0xfb, 0x23, 0xaf, 0x2f, 0x19, 0xb2,
        0xe2, 0x40, 0xc7, 0xc2, 0x84, 0x79, 0xec, 0xa1, 0xb6, 0x3c, 0x55, 0xe9, 0x1f, 0xe7, 0x4f, 0xf6,
        0x4b, 0x3a, 0xe6, 0x37, 0xec, 0x08, 0x03, 0xa5, 0xe2, 0x2a, 0x50, 0x0c, 0x30, 0x0b, 0x06, 0x09,
        0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x30, 0x0b, 0x13, 0x05, 0x2f, 0x62, 0x6f,
        0x6f, 0x74, 0x02, 0x02, 0x10, 0x00, 0x04, 0x82, 0x01, 0x00, 0xb2, 0x30, 0x8a, 0x23, 0x76, 0x92,
        0xba, 0xeb, 0xa9, 0x76, 0x64, 0x4b, 0x47, 0xd5, 0xa5, 0xaf, 0x96, 0x84, 0xf2, 0x29, 0x24, 0x8e,
        0x42, 0x73, 0x77, 0x06, 0x89, 0xbf, 0x63, 0x3a, 0x03, 0x11, 0x2c, 0x68, 0xe8, 0x3e, 0x91, 0x3e,
        0xfd, 0xc5, 0x1c, 0xc4, 0xee, 0x83, 0x07, 0x5e, 0x25, 0xa0, 0xdb, 0x86, 0x8e, 0xd1, 0x35, 0x1d,
        0x28, 0xa5, 0x61, 0x9b, 0x97, 0xa3, 0x94, 0xc0, 0x66, 0x61, 0x60, 0xc1, 0x8e, 0xe6, 0x07, 0x60,
        0xa3, 0x6c, 0xe3, 0x9e, 0x58, 0xb1, 0x1f, 0xfb, 0x83, 0xbb, 0x0c, 0x1c, 0x16, 0xa4, 0x55, 0xb5,
        0xc1, 0x65, 0x90, 0x15, 0x9e, 0x15, 0xb8, 0xf0, 0x7e, 0x3d, 0x33, 0x4e, 0x80, 0xbd, 0x6e, 0xd4,
        0x9b, 0xb1, 0x13, 0x4c, 0xa1, 0x1f, 0x8b, 0xc3, 0x7b, 0x52, 0x96, 0x05, 0xe0, 0x33, 0x20, 0x98,
        0xe3, 0xb2, 0xea, 0x7f, 0x76, 0xc0, 0x03, 0x85, 0xc4, 0xbd, 0x9f, 0x3b, 0x5c, 0x1c, 0x57, 0x81,
        0x5b, 0xb3, 0x04, 0xc6, 0xd3, 0xea, 0x20, 0x72, 0x83, 0x3f, 0x65, 0x92, 0x10, 0xd1, 0x0f, 0xec,
        0x06, 0x58, 0xde, 0x3b, 0xe1, 0x79, 0x76, 0x6a, 0x50, 0x9e, 0x3d, 0x18, 0x69, 0xf3, 0x4d, 0x93,
        0x72, 0x84, 0xc8, 0xd9, 0xe8, 0xe5, 0xec, 0xa4, 0x17, 0xf8, 0xa4, 0x17, 0x56, 0x1a, 0x8c, 0x1d,
        0xa9, 0x81, 0xf9, 0x1b, 0x8c, 0xe1, 0x11, 0x80, 0x6a, 0x73, 0xd7, 0x63, 0x08, 0x79, 0x64, 0x71,
        0xf0, 0x4f, 0x64, 0x5d, 0x26, 0x07, 0xb0, 0x34, 0x44, 0xa9, 0x8b, 0xd0, 0x1f, 0xb7, 0xe1, 0xd8,
        0x2a, 0xb8, 0x7c, 0xc2, 0xb5, 0x37, 0x41, 0xed, 0x17, 0x77, 0x84, 0xd8, 0x96, 0xe6, 0xf7, 0xd1,
        0xc8, 0xc2, 0xdd, 0x24, 0x2c, 0x70, 0x69, 0x0e, 0xed, 0xbc, 0x6a, 0xb9, 0x38, 0x56, 0x1a, 0x65,
        0xb5, 0x2a, 0xb4, 0x96, 0xac, 0x5f, 0x5e, 0xf1, 0x04, 0x0a
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "convert" hex numbers to their ASCII representation
//
// IN: buff  - buffer of hex numbers
// IN: bytes - number of bytes in "buff" to convert to ASCII representation
// OUT: ascii - buffer to take ASCII string representing "bytes" count of hex numbers from "buff"
typedef unsigned long long int QWORD;
static void hex_to_ascii(const PBYTE buff, const int bytes, char* ascii)
{
    const char charset[] = "0123456789abcdef";
    QWORD ascii_charset = (QWORD)charset;

    int i = 0x0;
    while (i < bytes) {

        *(PBYTE)ascii = *(PBYTE)(ascii_charset + (*(PBYTE)(buff + i) >> 0x4));
        *(PBYTE)(ascii + 0x01) = *(PBYTE)(ascii_charset + (*(PBYTE)(buff + i) & 0xf));

        i += 0x1; // go to next byte in buffIn
        ascii += 0x2; // move to next pair of chars in pOutput
    }

    return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// inserts corrects values in the boot image header
// 
// RETURNS zero on success
int WriteHeader(void) 
{
    BOOTIMG_HDR* hdr = (BOOTIMG_HDR*)g_sn_img;

    memset(hdr,0,sizeof(BOOTIMG_HDR));

    memcpy(hdr->magic,BOOT_MAGIC,BOOT_MAGIC_SIZE);
    hdr->kernel_size   = 0x00000008;    // these values make no sense
    hdr->kernel_addr   = 0x10008000;
    hdr->ramdisk_addr  = 0x11000000;
    hdr->second_addr   = 0x10f00000;
    hdr->tags_addr     = 0x10000100;
    hdr->page_size     = 0x00000800;
    hdr->header_version= 0x00000000;    // boot_img_hdr_v0
    //hdr->id   is the hash

    return NO_ERROR; 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// calculates the correct 20-bit SHA1 hash and inserts in at 0x240  (i.e. boot_img_hdr_v0.id)
// 
// RETURNS zero on success
int InsertHash(void)
{ 
    BOOTIMG_HDR* hdr = (BOOTIMG_HDR*)g_sn_img;

    // inspired by JohnWu's magiskboot source (bootimg.cpp)
    SHA_CTX ctx;
    SHA1_Init(&ctx);

    uint32_t size;
    int64_t zero64 = 0;

    // After much trial and error, this sequence works:
    //  SERIALNUMBER
    //  sizeof(SERIALNUMBER)
    //  (int64_t)0                         <== no idea why a zeroed 64-bit number is being used...but it works
    size = 0x8;
    SHA1_Update(&ctx, g_serialno, size);                // SERIALNUMBER
    SHA1_Update(&ctx, &size,      sizeof(size));        // sizeof(SERIALNUMBER)
    SHA1_Update(&ctx, &zero64,    sizeof(zero64));      // (int64_t)0

    SHA1_Final((unsigned char*)hdr->id,&ctx);   // hash it into boot_header.id

    // output hash to stdout
    char hash[128];
    memset(hash, 0, sizeof(hash));
    hex_to_ascii((PBYTE)hdr->id, SHA_DIGEST_LENGTH, hash);

    printf("SERIALNO: [%s]\n", g_serialno);
    printf("    HASH: [%s]\n", hash);

	return NO_ERROR;
}