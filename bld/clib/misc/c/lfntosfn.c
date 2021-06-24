/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2021 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  A function to convert long filenames to short filenames (DOS)
*
****************************************************************************/


#include "variety.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include "_doslfn.h"


_WCRTLINK int _islfn( const char *path )
{
    const char *buff;

    buff = strrchr( path, '\\' );
    if( buff == NULL ) {
        buff = path;
        if( buff[0] != '\0' && buff[1] == ':' ) {
            buff += 2;
        }
    } else {
        ++buff;
    }
    if( strlen( buff ) > 12 || strchr( buff, ' ' ) != NULL )
        return( 1 );
    return( 0 );
}

#ifdef __WATCOM_LFN__
static lfn_ret_t _lfntosfn_lfn( const char *orgname, char *shortname )
{
  #ifdef _M_I86
    return( __lfntosfn_lfn( orgname, shortname ) );
  #else
    call_struct     dpmi_rm;

    strcpy( RM_TB_PARM1_LINEAR, orgname );
    memset( &dpmi_rm, 0, sizeof( dpmi_rm ) );
    dpmi_rm.ds  = RM_TB_PARM1_SEGM;
    dpmi_rm.esi = RM_TB_PARM1_OFFS;
    dpmi_rm.es  = RM_TB_PARM2_SEGM;
    dpmi_rm.edi = RM_TB_PARM2_OFFS;
    dpmi_rm.ecx = 1;
    dpmi_rm.eax = 0x7160;
    dpmi_rm.flags = 1;
    if( __dpmi_dos_call( &dpmi_rm ) ) {
        return( -1 );
    }
    if( LFN_DPMI_ERROR( dpmi_rm ) ) {
        return( LFN_RET_ERROR( dpmi_rm.ax ) );
    }
    strcpy( shortname, RM_TB_PARM2_LINEAR );
    return( 0 );
  #endif
}
#endif

_WCRTLINK char *_lfntosfn( const char *orgname, char *shortname )
{
#ifdef __WATCOM_LFN__
    lfn_ret_t   rc;

    if( _RWD_uselfn ) {
        rc = _lfntosfn_lfn( orgname, shortname );
        if( LFN_ERROR( rc ) ) {
            strcpy( shortname, orgname );
            return( NULL );
        }
        if( LFN_OK( rc ) ) {
            return( shortname );
        }
    }
#endif
    return( strcpy( shortname, orgname ) );
}
