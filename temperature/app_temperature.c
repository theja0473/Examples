//============================================================================//
// File:          app_temperature.c                                           //
// Description:   Example for temperature readout                             //
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



/*----------------------------------------------------------------------------*\
** Include files                                                              **
**                                                                            **
\*----------------------------------------------------------------------------*/
#include "umic_temp.h"
#include <stdint.h>
#include <stdio.h>


//main

int main()
{
   int32_t slTempT;

   //----------------------------------------------------------------
   // The function umic_temp_read() returns an signed integer.
   // At 25°C the return value  is 25000.
   //
   slTempT = umic_temp_read();
   printf("Actual temperature inside controller: %i.%u°C \n",
		   (slTempT / 1000), ((slTempT / 100) % 10) );


   return 0;
}
