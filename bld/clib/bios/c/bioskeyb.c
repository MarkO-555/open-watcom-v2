/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2022 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  BIOS keyboard access.
*
****************************************************************************/


#include "variety.h"
#include <stddef.h>
#include <bios.h>
#include "necibm.h"
#include "rtdata.h"


_WCRTLINK unsigned short _bios_keybrd( unsigned cmd )
{
#if defined( __WATCOM_PC98__ )
    if( _RWD_isPC98 ) { /* NEC PC-98 */
        unsigned short  necRc;
        unsigned short  ret;

        /*** Translate IBM commands to NEC98 commands ***/
        switch( cmd ) {
        case _KEYBRD_READ:
            ret = __nec98_bios_keybrd( cmd, NULL );
            break;
        case _KEYBRD_READY:
            ret = __nec98_bios_keybrd( cmd, NULL );
            break;
        case _KEYBRD_SHIFTSTATUS:
            necRc = __nec98_bios_keybrd( cmd, NULL );
            ret = 0;
            if( necRc & 0x0001 )
                ret |= 0x02;
            if( necRc & 0x0002 )
                ret |= 0x40;
            if( necRc & 0x0008 )
                ret |= 0x08;
            if( necRc & 0x0010 )
                ret |= 0x04;
            break;
        default:
            ret = 0;        // invalid command for NEC 98
            break;
        }
        return( ret );
    }
#endif
    /* IBM PC */
    return( __ibm_bios_keybrd( cmd ) );
}
