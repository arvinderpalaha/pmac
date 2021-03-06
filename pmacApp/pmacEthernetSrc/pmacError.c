#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static const char * pmacErrors[] = {
    /* ERR000 */ "No Error",
    /* ERR001 */ "Command not allowed during program execution. (Should halt program execution before issuing command)",
    /* ERR002 */ "Password error. (Should enter the proper password)",
    /* ERR003 */ "Data error or unrecognized command. (Should correct syntax of command)",
    /* ERR004 */ "Illegal character: bad value (>127 ASCII) or serial parity/framing error. (Should correct the character and or check for noise on the serial cable)",
    /* ERR005 */ "Command not allowed unless buffer is open. (Should open a buffer first)",
    /* ERR006 */ "No room in buffer for command. (Should allow more room for buffer -- DELETE or CLEAR other buffers)",
    /* ERR007 */ "Buffer already in use. (Should CLOSE currently open buffer first)",
    /* ERR008 */ "MACRO auxiliary communications error. (Should check MACRO ring hardware and software setup)",
    /* ERR009 */ "Program structural error (e.g. ENDIF without IF). (Should correct structure of program)",
    /* ERR010 */ "Both overtravel limits set for a motor in the C. S. (Should correct or disable limits)",
    /* ERR011 */ "Previous move not completed. (Should Abort it or allow it to complete)",
    /* ERR012 */ "A motor in the coordinate system is open-loop. (Should close the loop on the motor)",
    /* ERR013 */ "A motor in the coordinate system is not activated. (Should set Ix00 to 1 or remove motor from C.S.)",
    /* ERR014 */ "No motors in the coordinate system. (Should define at least one motor in C.S.)",
    /* ERR015 */ "Not pointing to valid program buffer. (Should use B command first, or clear out scrambled buffers)",
    /* ERR016 */ "Running improperly structured program (e.g.missing ENDWHILE). (Should correct structure of program)",
    /* ERR017 */ "Trying to resume after H or Q with motors out of stopped position. (Should use J= to return motor[s] to stopped position)",
    /* ERR018 */ "Attempt to perform phase reference during move, move during phase reference., or enabling with phase clock error. (Should finish move before phase reference, finish phase reference before move, or fix phase clock source problem)",
    /* ERR019 */ "Illegal position-change command while moves stored in CCBUFFER. (Should pass through section of Program requiring storage of moves in CCBUFFER, or abort)"
};

static const char * badErrString = "Not a valid PMAC error string";
static const char * badErrNum = "Not a valid PMAC error number";

const char * pmacError( const char * errStr )
{
    const char * result;

    /* Strip any leading BELL */
    if (*errStr == '\a') errStr++;

    /* Check for correct format */
    if (strncmp(errStr, "ERR", 3 ) == 0 &&
	isdigit((int) errStr[3]) && isdigit((int) errStr[4]) && isdigit((int) errStr[5]) )
    {
        int errnum = (errStr[3]-'0')*100 + (errStr[4]-'0')*10 + errStr[5]-'0';

        if ( errnum <=0 || errnum > sizeof( pmacErrors ) ) result = badErrNum;
        else result = pmacErrors[errnum];
    }
    else result = badErrString;

    return result;
}
