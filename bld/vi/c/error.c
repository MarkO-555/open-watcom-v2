/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2015-2016 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Error processing, fatal and non-fatal.
*
****************************************************************************/


#include "vi.h"
#include "win.h"
#include "myprtf.h"

static char     strBuff[25];
static bool     errorMessagesLoaded = false;
static int      errCnt;
static char     *errorList;

static char     *errorTokens = NULL;
static int      *errorValues = NULL;
static bool     errorTokensLoaded = false;

/*
 * StartupError - process fatal startup error
 */
void StartupError( vi_rc err )
{
    const char  *str;

    if( err == ERR_NO_MEMORY ) {
        str = "Out of memory";
    } else {
        str = GetErrorMsg( err );
    }
    MyPrintf( "%s (fatal)\n", str );

    FiniMem();

    /* Do not call ExitEditor() because almost nothing is initialized yet. */
    exit( -1 );
    // never return
} /* StartupError */

/*
 * FatalError - process fatal error
 */
void FatalError( vi_rc err )
{
    const char  *str;

    SetPosToMessageLine();
    if( err == ERR_NO_MEMORY ) {
        str = "Out of memory";
    } else {
        str = GetErrorMsg( err );
    }
    MyPrintf( "%s (fatal)\n", str );
    ExitEditor( -1 );

} /* FatalError */

/*
 * Die - unusual termination
 */
void Die( const char *str, ... )
{
    va_list     al;

    SetPosToMessageLine();
    MyPrintf( "Failure: " );
    va_start( al, str );
    MyVPrintf( str, al );
    va_end( al );
    MyPrintf( "\n" );
    ExitEditor( -1 );

} /* Die */

static bool errmsg_alloc( int cnt )
{
    errCnt = cnt;
    return( false );
}

static bool errmsg_save( int i, const char *buff )
{
    /* unused parameters */ (void)i; (void)buff;

    return( true );
}

/*
 * readErrorMsgData - do just that
 */
static void readErrorMsgData( void )
{
    vi_rc       rc;

    rc = ReadDataFile( "errmsg.dat", &errorList, errmsg_alloc, errmsg_save, true );
    if( rc != ERR_NO_ERR ) {
        return;
    }
    errorMessagesLoaded = true;

} /* readErrorMsgData */

/*
 * GetErrorMsg - return pointer to message
 */
const char *GetErrorMsg( vi_rc err )
{
    const char  *msg;

    if( !errorMessagesLoaded ) {
        readErrorMsgData();
    }
    LastError = err;
    if( EditFlags.InputKeyMapMode ) {
        DoneInputKeyMap();
        EditFlags.NoInputWindow = false;
        EditFlags.Dotable = false;
    }
    if( err < 0 || err > errCnt ) {
        MySprintf( strBuff, "Err no. %d (no msg)", err );
        return( strBuff );
    }
    msg = GetTokenString( errorList, (int)err );
    if( msg == NULL ) {
        MySprintf( strBuff, "Err no. %d (no msg)", err );
        return( strBuff );
    }
    return( msg );

} /* GetErrorMsg */

/*
 * Error - print an error message in the message window
 */
void Error( const char *str, ... )
{
    va_list     al;
    char        tmp[MAX_STR];

    if( !BAD_ID( message_window_id ) ) {
        WindowAuxUpdate( message_window_id, WIND_INFO_TEXT_COLOR,
                            messagew_info.hilight_style.foreground );
        WindowAuxUpdate( message_window_id, WIND_INFO_BACKGROUND_COLOR,
                            messagew_info.hilight_style.background );
        va_start( al, str );
        MyVSprintf( tmp, str, al );
        va_end( al );

        SourceError( tmp );
        Message1( "%s", tmp );

        WindowAuxUpdate( message_window_id, WIND_INFO_TEXT_COLOR,
                            messagew_info.text_style.foreground );
        WindowAuxUpdate( message_window_id, WIND_INFO_BACKGROUND_COLOR,
                            messagew_info.text_style.background );
        MyBeep();
    } else {
        va_start( al, str );
#ifndef __WIN__
        MyVPrintf( str, al );
        MyPrintf( "\n" );
#endif
        va_end( al );
    }

} /* Error */

/*
 * ErrorBox - show an error message in a dialog box
 */
void ErrorBox( const char *str, ... )
{
    va_list     al;
    char        tmp[MAX_STR];

    if( !BAD_ID( message_window_id ) ) {
        WindowAuxUpdate( message_window_id, WIND_INFO_TEXT_COLOR,
                            messagew_info.hilight_style.foreground );
        WindowAuxUpdate( message_window_id, WIND_INFO_BACKGROUND_COLOR,
                            messagew_info.hilight_style.background );
        va_start( al, str );
        MyVSprintf( tmp, str, al );
        va_end( al );

        SourceError( tmp );
        Message1Box( "%s", tmp );

        WindowAuxUpdate( message_window_id, WIND_INFO_TEXT_COLOR,
                            messagew_info.text_style.foreground );
        WindowAuxUpdate( message_window_id, WIND_INFO_BACKGROUND_COLOR,
                            messagew_info.text_style.background );
        MyBeep();
    } else {
        va_start( al, str );
#ifndef __WIN__
        MyVPrintf( str, al );
        MyPrintf( "\n" );
#endif
        va_end( al );
    }

} /* Error */


static bool err_alloc( int cnt )
{
    errorValues = _MemAllocArray( int, cnt );
    return( true );
}

static bool err_save( int i, const char *buff )
{
    errorValues[i] = atoi( buff );
    return( true );
}

/*
 * ReadErrorTokens - do just that
 */
vi_rc ReadErrorTokens( void )
{
    vi_rc       rc;

    if( errorTokensLoaded ) {
        return( ERR_NO_ERR );
    }

    rc = ReadDataFile( "error.dat", &errorTokens, err_alloc, err_save, true );
    if( rc != ERR_NO_ERR ) {
        if( rc == ERR_FILE_NOT_FOUND ) {
            return( ERR_SRC_NO_ERROR_DATA );
        }
        return( rc );
    }
    errorTokensLoaded = true;

    return( ERR_NO_ERR );

} /* ReadErrorTokens */

/*
 * GetErrorTokenValue
 */
vi_rc GetErrorTokenValue( int *value, const char *str )
{
    int     i;
    vi_rc   rc;

    rc = ReadErrorTokens();
    if( rc == ERR_NO_ERR ) {
        i = Tokenize( errorTokens, str, true );
        if( i != TOK_INVALID ) {
            *value = errorValues[i];
        } else {
            rc = NO_NUMBER;
        }
    }
    return( rc );

} /* GetErrorTokenValue */

void ErrorFini( void )
{
    MemFree( errorList );
    errorMessagesLoaded = false;

    MemFree( errorTokens );
    MemFree( errorValues );
    errorTokensLoaded = false;

} /* ErrorFini */
