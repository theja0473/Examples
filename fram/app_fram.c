#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>



#include <umic_fram.h>

/*
** Code Beispiel
*/

int main()
{
   int32_t  slStatusT;
   uint16_t uwStartAdrT = 500;
   uint8_t  aubRcvBufT[32];
   uint16_t uwCountT;
   uint8_t  aubDataT[] = {0xAA,0xAB,0xCC,0x00,0xAA,0x11,0xAA,0xCC,0xAB,0x1C};

   //----------------------------------------------------------------
   // print prgram message
   //
   printf("\n\n");
   printf("###############################################################################\n");
   printf("# uMIC.200 FRAM Example                                                       #\n");
   printf("###############################################################################\n");
   printf("\n");

   //----------------------------------------------------------------
   // init FRAM, exit on error
   //
   slStatusT = umic_fram_init();
   printf("Initialize FRAM ............. : ");
   if (slStatusT != 0)
   {
      printf("failed with error code %i", slStatusT);
      return slStatusT;
   }
   printf("done \n");


   //----------------------------------------------------------------
   // FRAM enable write access
   //
   printf("Enable FRAM write access .... : ");
   if(umic_fram_write_enable() == 0)
   {
      printf("done \n");
   }
   else
   {
      printf("failed \n");
   }


   //----------------------------------------------------------------
   // Write data to FRAM
   // Data to write in aubDataT[]
   // Start Address = 500;
   //
   printf("Write sample data to FRAM ... : ");
   slStatusT = umic_fram_write(uwStartAdrT,
                               aubDataT,
                               (uint32_t)sizeof(aubDataT));
   if(slStatusT == 0)
   {
      printf("success \n");
      printf("Data written to FRAM ........ : ");
      for(uwCountT=0; uwCountT < (uint32_t)sizeof(aubDataT); uwCountT++)
      {
         printf("%02Xh ", aubDataT[uwCountT]);
      }
      printf("\n");
   }
   else
   {
      printf("failed. \n");
   }

   //----------------------------------------------------------------
   // FRAM disable write access
   //
   printf("Disable FRAM write access ... : ");
   if(umic_fram_write_disable() == 0)
   {
      printf("done \n");
   }
   else
   {
      printf("failed \n");
   }


   //----------------------------------------------------------------
   // Read data from FRAN
   //
   printf("Read sample data from FRAM .. : ");
   if(umic_fram_read(uwStartAdrT, aubRcvBufT, (uint32_t)sizeof(aubDataT)) == 0)
   {
      printf("success \n");
      printf("Data read from FRAM ......... : ");
      for(uwCountT=0; uwCountT < (uint32_t)sizeof(aubDataT); uwCountT++)
      {
         printf("%02Xh ", aubRcvBufT[uwCountT]);
      }
      printf("\n");
   }


   //----------------------------------------------------------------
   // FRAM release.
   //
   umic_fram_release();
   printf("Release FRAM ................ : ");
   umic_fram_release();
   printf("done \n\n");
   return 0;
}
