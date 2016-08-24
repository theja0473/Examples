//****************************************************************************//
// File:          app_blinky.c                                                //
// Description:   This file contains sources for blinky example               //
// Author:        Johann Tiderko                                              //
// e-mail:        johann.tiderko@microcontrol.net                             //
//                                                                            //
// Copyright (C) MicroControl GmbH & Co. KG                                   //
// Junkersring 23                                                             //
// 53844 Troisdorf                                                            //
// Germany                                                                    //
// Tel: +49-2241-25659-0                                                      //
// Fax: +49-2241-25659-11                                                     //
//                                                                            //
// The copyright to the computer program(s) herein is the property of         //
// MicroControl GmbH & Co. KG, Germany. The program(s) may be used            //
// and/or copied only with the written permission of MicroControl GmbH &      //
// Co. KG or in accordance with the terms and conditions stipulated in        //
// the agreement/contract under which the program(s) have been supplied.      //
//----------------------------------------------------------------------------//
//                                                                            //
// Date        History                                                        //
// ----------  -------------------------------------------------------------- //
// 02.02.2016  Initial version                                                //
//                                                                            //
//****************************************************************************//


/*----------------------------------------------------------------------------*\
** Include files                                                              **
**                                                                            **
\*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "umic_led.h"

/*----------------------------------------------------------------------------*\
** local variables and implementation                                         **
**                                                                            **
\*----------------------------------------------------------------------------*/



//----------------------------------------------------------------------------//
// main()                                                                     //
//                                                                            //
//----------------------------------------------------------------------------//
int main()
{
   unsigned char ubLedCntrT;
   unsigned char ubRunTimeT;
   uint8_t aubTriggerValueT[8];

   //----------------------------------------------------------------
   // RUN LED Blinky sequence
   //
   printf("\n\n");
   printf("###############################################################################\n");
   printf("# uMIC.200 Blinky Example                                                     #\n");
   printf("###############################################################################\n");
   printf("\n\n");
   printf("Switch ON sequentially green LED2 to LED4,\n");
   printf("then switch ON sequentially red LED2 to LED4.\n");
   ubRunTimeT = 5;


   printf("This sequence will run %i times\n",ubRunTimeT);
   //----------------------------------------------------------------
   // Safe Current LED Trigger Mode
   //
   for (ubLedCntrT = eUMIC_LED1_GREEN; ubLedCntrT <= eUMIC_LED4_GREEN; ubLedCntrT++)
   {
      umic_led_trigger_get(ubLedCntrT, &aubTriggerValueT[ubLedCntrT]);
      printf("Current Mode of LED_GREEN%i = %i\n",ubLedCntrT+1,aubTriggerValueT[ubLedCntrT]);
      fflush(stdout);
   }

   for (ubLedCntrT = eUMIC_LED1_RED; ubLedCntrT <= eUMIC_LED4_RED; ubLedCntrT++)
   {
      umic_led_trigger_get(ubLedCntrT, &aubTriggerValueT[ubLedCntrT]);
      printf("Current Mode of LED_RED%i = %i\n",ubLedCntrT+1,aubTriggerValueT[ubLedCntrT]);
      fflush(stdout);
   }

   while (ubRunTimeT)
   {
      ubRunTimeT--;
      printf(".");
      fflush(stdout);

      for (ubLedCntrT = eUMIC_LED1_GREEN; ubLedCntrT <= eUMIC_LED4_GREEN; ubLedCntrT++)
      {
         umic_led_set(ubLedCntrT);
      }
      for (ubLedCntrT = eUMIC_LED1_RED; ubLedCntrT <= eUMIC_LED4_RED; ubLedCntrT++)
      {
         umic_led_clr(ubLedCntrT);
      }

      sleep(1);
      for (ubLedCntrT = eUMIC_LED1_GREEN; ubLedCntrT <= eUMIC_LED4_GREEN; ubLedCntrT++)
      {
         umic_led_clr(ubLedCntrT);
      }
      for (ubLedCntrT = eUMIC_LED1_RED; ubLedCntrT <= eUMIC_LED4_RED; ubLedCntrT++)
      {
         umic_led_set(ubLedCntrT);
      }

      sleep(1);
   }

   for (ubLedCntrT = eUMIC_LED1_RED; ubLedCntrT <= eUMIC_LED4_RED; ubLedCntrT++)
   {
      umic_led_clr(ubLedCntrT);
   }

   //----------------------------------------------------------------
   // Write Old LED Trigger Mode
   //
   for (ubLedCntrT = eUMIC_LED1_GREEN; ubLedCntrT <= eUMIC_LED4_GREEN; ubLedCntrT++)
   {
      umic_led_trigger_set(ubLedCntrT, aubTriggerValueT[ubLedCntrT]);
      fflush(stdout);
   }

   for (ubLedCntrT = eUMIC_LED1_RED; ubLedCntrT <= eUMIC_LED4_RED; ubLedCntrT++)
   {
      umic_led_trigger_set(ubLedCntrT, aubTriggerValueT[ubLedCntrT]);
      fflush(stdout);
   }

   printf("\n");

   return 0;
}
