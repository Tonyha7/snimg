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

// extract from AOSP/CAF: system/tools/mkbootimg/include/bootimg/bootimg.h
/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef H_CAF
#define H_CAF 

///////////////////////////////////////////////////////////////////////////////////////////////////////
// purpose:  SN.IMG uses version 0 of the boot header
#define BOOT_MAGIC "ANDROID!"
#define BOOT_MAGIC_SIZE 8
#define BOOT_NAME_SIZE 16
#define BOOT_ARGS_SIZE 512
#define BOOT_EXTRA_ARGS_SIZE 1024

typedef struct boot_img_hdr_v0 {
    uint8_t magic[BOOT_MAGIC_SIZE];
    uint32_t kernel_size; 
    uint32_t kernel_addr; 
    uint32_t ramdisk_size;
    uint32_t ramdisk_addr;
    uint32_t second_size; 
    uint32_t second_addr; 
    uint32_t tags_addr;
    uint32_t page_size;
    uint32_t header_version;
    uint32_t os_version;
    uint8_t name[BOOT_NAME_SIZE];
    uint8_t cmdline[BOOT_ARGS_SIZE];
    uint32_t id[8];                     // hash
    uint8_t extra_cmdline[BOOT_EXTRA_ARGS_SIZE];
} BOOTIMG_HDR;

#endif