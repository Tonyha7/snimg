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

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "snimg.h"
#include "system.h"

// declare statics & globals
static const char VERSION[] = "1.0";   // program version
char        g_serialno[9] = { 0 };     // null-terminated 8-char serial number
PBYTE       g_sn_img = NULL;           // pointer to sn.img in memory

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// prints help info to stdio
static void usage(void)
{
    printf("\nsnimg  version: %s\n", VERSION);

    printf(  "               a utility to create an \"sn.img\" file to unlock an Android device.\n");
    printf(  "               The sn.img file only works with the \"fastboot flash unlock sn.img\" command.\n");
    printf("\n\nusage:\n");
    printf("     snimg <serial_no>\n\n");

    printf("                serial_no: serial number of device, eg HIHOHIHO.\n");
    printf("                           This is made up of 8 alphanumeric characters.\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// write the sn.img file to disk
// RETURN: zero on success
static int WriteToDisk(void)
{
    FILE* fp = NULL;
    int rc = NO_ERROR;
    char filename[32] = { 0 };

    sprintf(filename, "%s_sn.img", g_serialno);

#ifdef OS_WINDOWS
    errno_t erc = fopen_s(&fp, filename, "wb");
    if (erc || (fp == NULL)) {
        rc = erc;
#else // OS_LINUX
    fp = fopen(filename,"wb");
    if (fp == NULL) {
        rc = errno;
#endif
        printf("ERROR: fopen(%s) failed with rc=[%d] (%s)", filename, rc,strerror(rc));
        return rc;
    }

    size_t written = fwrite(g_sn_img, sizeof(BYTE), SIZEOF_IMAGE, fp);

    if (written != SIZEOF_IMAGE)
        printf("WARNING: only wrote [%zd] bytes of [%d] to file [%s]", written, SIZEOF_IMAGE, filename);

    fclose(fp);
    return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    int rc = NO_ERROR;

    // check args...
    if (argc != 2) {
        printf("ERROR: wrong number of arguments supplied\n");
        usage();
        return EINVAL;
    }

    // copy serial number
    strncpy(g_serialno, argv[1], 8);

    // to save time and effort, we create the sn.img file in memory and then dump it to disk
    g_sn_img = (PBYTE)malloc(SIZEOF_IMAGE);
    if (g_sn_img == NULL) {
        printf("ERROR: memory allocation failed.    rc=[%d]\n",errno);
        return ENOMEM;
    }
    memset(g_sn_img, 0, SIZEOF_IMAGE);
 
    // setup the boot header
    rc = WriteHeader();
    if (rc) return rc;

    // calculate and write the hash to 0x240
    rc = InsertHash();
    if (rc) return rc;

    // insert the serial number at 0x800
    memcpy(g_sn_img + OFFSET_SERIALNO, g_serialno, 8); // don't copy the null-terminator

    // copy the image signature and X509 cert (which are identical in all sn.img files)
    memcpy(g_sn_img + OFFSET_SIGN, blob, SIZEOF_SIGNATURE);

    // write the sn.img out to disk
    rc = WriteToDisk();
    if (rc) return rc;

    printf("\nFile [%s_sn.img] created in current directory\n",g_serialno);
    printf("\nUse \"fastboot flash unlock %s_sn.img\" to allow \"fastboot oem unlock-go\"\n\n",g_serialno);

    if (g_sn_img)
        free(g_sn_img);

    return NO_ERROR;
}