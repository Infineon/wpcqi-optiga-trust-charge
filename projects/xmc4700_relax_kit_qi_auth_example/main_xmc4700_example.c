/** \example main_xmc4500.c
* \copyright
* MIT License
*
* Copyright (c) 2020-2023 Infineon Technologies AG
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
* \endcopyright
*
* \author  Infineon Technologies AG
*
* \file
*
* \brief   This sample demonstrates OPTIGA use cases.
*
*/

/*************************************************************************
 *  required includes
*************************************************************************/

// Standard libs
#include "stdio.h"
#include "unistd.h"

// Platform dependent libs
#include <DAVE.h>            //Declarations from DAVE Code Generation (includes SFR declaration)
#include "optiga/common/optiga_lib_logger.h"
#include "optiga/pal/pal_os_timer.h"
#include "optiga/pal/pal_logger.h"

// QI Auth API
/*************************************************************************
 *  main.c
*************************************************************************/


/*************************************************************************
*  defines
*************************************************************************/

/*************************************************************************
*  function prototypes
*************************************************************************/
//extern void optiga_shell_wait_for_user(void);
extern int qi_auth_self_test(void);
/*************************************************************************
*  Global
*************************************************************************/
extern pal_logger_t logger_console;

/*************************************************************************
*  functions
*************************************************************************/


/**
 * This function is the entry point of sample.
 *
 * \retval
 *  0 on success
 *  1 on failure
 */
int main(Void)
{
    //Return value
    int32_t status = (int32_t) 0;

    // Initialization of DAVE Apps
    if (DAVE_Init() == DAVE_STATUS_FAILURE)
    {
        /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
        while (1U) { }
    }

    pal_logger_init(&logger_console);

    pal_os_timer_delay_in_milliseconds(100);

    status = qi_auth_self_test();

    return status;
}


