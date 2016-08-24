#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/types.h>


#include <umic_dio.h>
#include <umic_relay.h>

/*
** Code Beispiel
*/

int main()
{
   uint8_t  ubCntT;
   uint8_t  ubOutValueT = 0x55;

   //----------------------------------------------------------------
   // print program message
   //
   printf("\n\n");
   printf("###############################################################################\n");
   printf("# uMIC.200 Digital I/O Example                                                #\n");
   printf("###############################################################################\n");
   printf("\n");


   //----------------------------------------------------------------
   // init digital I/O, exit on error
   //
   printf("Initialize Digital I/O ...... : ");
   if (umic_dio_init() < 0)
   {
      printf("failed \n");
      _exit(0);
   }
   printf("done \n");

   //----------------------------------------------------------------
   // relay on
   //
   printf("Switch relay on ............  : ");
   if(umic_relay_on(0) < 0)
   {
      printf("failed \n");
   }
   else
   {
      printf("done \n");
   }

   //----------------------------------------------------------------
   // all pins are outputs
   //
   printf("Configure all pins as output  : ");
   if(umic_dio_set_direction(0, 0xFF) < 0)
   {
      printf("failed \n");
   }
   else
   {
      printf("done \n");
   }


   //----------------------------------------------------------------
   // set output pins
   //
   for(ubCntT = 0; ubCntT < 8; ubCntT ++)
   {
      ubOutValueT = ~ubOutValueT;
      printf("Write output pattern ........ : %02Xh\n", ubOutValueT);
      umic_dio_set_output(0, ubOutValueT);
      sleep(1);
      if(umic_dio_get_input(0) == ubOutValueT)
      {
         printf("Read input value ............ : %02Xh - match\n", ubOutValueT);
      }
   }
   printf("\n");

   //----------------------------------------------------------------
   // relay off
   //
   printf("Switch relay off ............ : ");
   if(umic_relay_off(0) < 0)
   {
      printf("failed \n");
   }
   else
   {
      printf("done \n");
   }

   //----------------------------------------------------------------
   // Digital I/O release.
   //
   printf("Release Digital I/O ......... : ");
   umic_dio_set_output(0, 0);
   umic_dio_release();
   printf("done \n\n");
   return 0;
}
