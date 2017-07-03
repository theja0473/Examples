//============================================================================//
// File:          umic_watchdog.c                                             //
// Description:   Example for watchdog                                        //
//                                                                            //
// Copyright (C) MicroControl GmbH & Co. KG                                   //
// 53844 Troisdorf - Germany                                                  //
// www.microcontrol.net                                                       //
//                                                                            //
//----------------------------------------------------------------------------//
// Redistribution and use in source and binary forms, with or without         //
// modification, are permitted provided that the following conditions         //
// are met:                                                                   //
// 1. Redistributions of source code must retain the above copyright          //
//    notice, this list of conditions, the following disclaimer and           //
//    the referenced file 'LICENSE'.                                          //
// 2. Redistributions in binary form must reproduce the above copyright       //
//    notice, this list of conditions and the following disclaimer in the     //
//    documentation and/or other materials provided with the distribution.    //
// 3. Neither the name of MicroControl nor the names of its contributors      //
//    may be used to endorse or promote products derived from this software   //
//    without specific prior written permission.                              //
//                                                                            //
// Provided that this notice is retained in full, this software may be        //
// distributed under the terms of the GNU Lesser General Public License       //
// ("LGPL") version 3 as distributed in the 'LICENSE' file.                   //
//                                                                            //
//============================================================================//


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <assert.h>
#include <linux/watchdog.h>

int main(void)
{
    int32_t slFileDescT = open("/dev/watchdog", O_WRONLY);
    int32_t slTimeoutT = 45;  // a time in seconds

    // Send time request to the driver.
    ioctl(slFileDescT, WDIOC_SETTIMEOUT, &slTimeoutT);
    printf("The timeout was set to %d seconds\n", slTimeoutT);

    if (slFileDescT == -1)
    {
      perror("watchdog");
      exit(EXIT_FAILURE);
    }
    close(slFileDescT);

    return 0;
}
