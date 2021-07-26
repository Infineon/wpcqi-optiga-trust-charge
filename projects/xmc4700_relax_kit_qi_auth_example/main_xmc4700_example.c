/** \example main_xmc4500.c
* \copyright
* Copyright� 2018, Infineon Technologies AG
* All rights reserved.
*
* This software is provided with terms and conditions as specified in OPTIGA� Trust X Evaluation Kit License Agreement.
 * \endcopyright
 *
 * \author  Infineon AG
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

    // Initialisation of DAVE Apps
    if (DAVE_Init() == DAVE_STATUS_FAILURE)
    {
        /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
        while (1U) { }
    }

    pal_logger_init(&logger_console);

    pal_os_timer_delay_in_milliseconds(100);

    qi_auth_self_test();

    return ((status == 0)? 0:1);
}


