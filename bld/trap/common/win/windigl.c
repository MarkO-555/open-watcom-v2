/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2023 The Open Watcom Contributors. All Rights Reserved.
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include <stdlib.h>
#include <string.h>
#include "digld.h"
#include "winpath.h"
#include "servio.h"


size_t DIGLoader( Find )( dig_filetype ftype, const char *base_name, size_t base_name_len,
                                const char *defext, char *filename, size_t filename_len )
/****************************************************************************************/
{
    bool        has_ext;
    bool        has_path;
    char        *p;
    char        fname[256];
    char        c;
    size_t      len;

    /* unused parameters */ (void)ftype;

    has_ext = false;
    has_path = false;
    p = fname;
    while( base_name_len-- > 0 ) {
        c = *base_name++;
        *p++ = c;
        switch( c ) {
        case '.':
            has_ext = true;
            break;
        case '/':
        case '\\':
            has_ext = false;
                /* fall through */
        case ':':
            has_path = true;
            break;
        }
    }
    if( !has_ext && *defext != '\0' ) {
        *p++ = '.';
        p = StrCopyDst( defext, p );
    }
    *p = '\0';
    p = fname;
    if( !has_path ) {
        _searchenv( fname, "PATH", RWBuff );
        p = RWBuff;
    }
    len = strlen( p );
    if( filename_len > 0 ) {
        filename_len--;
        if( filename_len > len )
            filename_len = len;
        if( filename_len > 0 )
            strncpy( filename, p, filename_len );
        filename[filename_len] = '\0';
    }
    return( len );
}
