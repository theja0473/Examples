//****************************************************************************//
// File:          com_main_linux.c                                            //
// Description:   Demo for Linux with socketcan driver                        //
// Author:        Uwe Koppe                                                   //
// e-mail:        koppe@microcontrol.net                                      //
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
// 19.05.2014  Initial version                                                //
//                                                                            //
//****************************************************************************//





/*----------------------------------------------------------------------------*\
** Include files                                                              **
**                                                                            **
\*----------------------------------------------------------------------------*/



#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "canopen_master.h"


/*----------------------------------------------------------------------------*\
** Module variables                                                           **
**                                                                            **
\*----------------------------------------------------------------------------*/

static uint8_t	      ubProgRunS;
static uint8_t       ubSdoConfigS;
static uint32_t      ulCounterOneSecondS = 0;
static ComNode_ts    atsComNodeS[126];
static uint8_t       ubNodeIdDigitalModuleS = 0;

//-------------------------------------------------------------------
// configuration objects
//
static CoObject_ts   atsCoObjectS[10];
static uint32_t      ulObjCountS;
static uint32_t      ulIdx1016_S;
static uint8_t       ubIdx5FF5_S;

/*----------------------------------------------------------------------------*\
** Structures / Enumerations                                                  **
**                                                                            **
\*----------------------------------------------------------------------------*/

enum ComDemoNodeCmd_e {
   eDEMO_NODE_CMD_WRITE_CONFIG = 0x01

};

/*----------------------------------------------------------------------------*\
** Function                                                                   **
**                                                                            **
\*----------------------------------------------------------------------------*/



//----------------------------------------------------------------------------//
// ComDemoAppInit()                                                           //
//                                                                            //
//----------------------------------------------------------------------------//
void ComDemoAppInit(uint8_t ubNetV)
{
   uint8_t  ubCntT;

   //----------------------------------------------------------------
   // add structures for node-ID 1 to node-ID 126
   //
   for(ubCntT = 1; ubCntT < 127; ubCntT++)
   {
      ComNodeSetDefault(&atsComNodeS[ubCntT - 1]);
      ComMgrNodeAdd(ubNetV, ubCntT, &atsComNodeS[ubCntT - 1]);
   }

   //----------------------------------------------------------------
   // Setup the internal object table, which has the following
   // entries:
   // 1016h:01h - ulIdx1016_S  - write operation
   //

   //----------------------------------------------------------------
   // module heartbeat consumer time is 2500 ms (0x09C4), test
   // for node-ID 127 (0x7F)
   //
   ulIdx1016_S = 0x007F09C4;

   //----------------------------------------------------------------
   // object 1016h:01h
   //
   atsCoObjectS[0].uwIndex    = 0x1016;
   atsCoObjectS[0].ubSubIndex = 0x01;
   atsCoObjectS[0].ubMarker   = eDEMO_NODE_CMD_WRITE_CONFIG;
   atsCoObjectS[0].ulDataSize = 4;
   atsCoObjectS[0].pvdData    = &(ulIdx1016_S);

   //----------------------------------------------------------------
   // port direction: all outputs
   //
   ubIdx5FF5_S = 0xFF;

   //----------------------------------------------------------------
   // object 5FF5h:00h
   //
   atsCoObjectS[1].uwIndex    = 0x5FF5;
   atsCoObjectS[1].ubSubIndex = 0x00;
   atsCoObjectS[1].ubMarker   = eDEMO_NODE_CMD_WRITE_CONFIG;
   atsCoObjectS[1].ulDataSize = 1;
   atsCoObjectS[1].pvdData    = &(ubIdx5FF5_S);

}


//----------------------------------------------------------------------------//
// ComDemoAppProcess()                                                        //
//                                                                            //
//----------------------------------------------------------------------------//
void ComDemoAppProcess(uint8_t ubNetV)
{
   uint8_t        ubNodeIdT;
   uint8_t        ubNmtStateT;
   static uint8_t aubPdoDataS[8];

   //----------------------------------------------------------------
   // run configuration for new module
   //
   if(ubSdoConfigS == 0)
   {
      for(ubNodeIdT = 1; ubNodeIdT < 127; ubNodeIdT++)
      {
         ComNmtGetNodeState(ubNetV, ubNodeIdT, &ubNmtStateT);

         if(ubNmtStateT == eCOM_NMT_STATE_BOOTUP)
         {
            ComNodeGetInfo(ubNetV, ubNodeIdT);
            ubSdoConfigS = ubNodeIdT;
         }
      }
   }

   //----------------------------------------------------------------
   // send PDO to digital I/O module
   //
   if(ubNodeIdDigitalModuleS > 0)
   {
      if(aubPdoDataS[0] != 0x55)
      {
         aubPdoDataS[0] = 0x55;
      }
      else
      {
         aubPdoDataS[0] = 0xAA;
      }
      ComPdoSetData(ubNetV, (COM_PDO_TRM | ubNodeIdDigitalModuleS),
                    &(aubPdoDataS[0]));
   }
}


//----------------------------------------------------------------------------//
// ComDemoSetupPdoConfiguration()                                             //
//                                                                            //
//----------------------------------------------------------------------------//
ComStatus_tv ComDemoSetupPdoConfiguration(uint8_t ubNetV, uint8_t ubNodeIdV)
{
   printf("                ");
   printf("Setup PDO configuration - TPDO %03Xh - RPDO %03Xh \n",
         0x0180 + ubNodeIdV, 0x0200 + ubNodeIdV);

   ComPdoIdAdd( ubNetV, (COM_PDO_TRM | ubNodeIdV), 0x0200 + ubNodeIdV, 1, 1, 0);
   ComPdoEnable(ubNetV, (COM_PDO_TRM | ubNodeIdV), 1);

   ComPdoIdAdd( ubNetV, (COM_PDO_RCV | ubNodeIdV), 0x0180 + ubNodeIdV, 1, 1, 0);
   ComPdoEnable(ubNetV, (COM_PDO_RCV | ubNodeIdV), 1);

   return(eCOM_ERR_OK);
}


//----------------------------------------------------------------------------//
// ComDemoShowNodeInfo()                                                      //
// print some information about the CANopen device                            //
//----------------------------------------------------------------------------//
void ComDemoShowNodeInfo(uint8_t ubNetV, uint8_t ubNodeIdV)
{
   printf("can%d: NID %03d - Device profile CiA %d\n",
           ubNetV, ubNodeIdV,
           (atsComNodeS[ubNodeIdV - 1].ulIdx1000_DT) & 0x0000FFFF);

   printf("                ");
   printf("Device name: %s, Vendor-ID: %08X\n",
          atsComNodeS[ubNodeIdV - 1].aubIdx1008_DN,
          atsComNodeS[ubNodeIdV - 1].ulIdx1018_VI);
}


//----------------------------------------------------------------------------//
// CdcWriteModuleConfiguration()                                              //
// print some information about the CANopen device                            //
//----------------------------------------------------------------------------//
ComStatus_tv ComDemoWriteModuleConfiguration(uint8_t ubNetV, uint8_t ubNodeIdV)
{
   uint8_t        ubSdoT;     // SDO client

   printf("                ");
   printf("Write module configuration ...\n");

   //----------------------------------------------------------------
   // get and check SDO client index
   //
   ubSdoT = ComSdoGetClient(ubNetV);
   if(ubSdoT >= COM_SDO_CLIENT_MAX)
   {
      return(-eCOM_ERR_SDO_CLIENT_VALUE);
   }

   //----------------------------------------------------------------
   // perform the SDO write operation
   //
   ulObjCountS = 2;

   ComSdoWriteObject(ubNetV, ubSdoT, ubNodeIdV,
                     &atsCoObjectS[0],
                     &ulObjCountS);

   return(eCOM_ERR_OK);
}

//----------------------------------------------------------------------------//
// ComSdoEventObjectReady()                                                   //
// Function handler for SDO                                                   //
//----------------------------------------------------------------------------//
void ComSdoEventObjectReady(uint8_t ubNetV, uint8_t ubNodeIdV,
                            CoObject_ts * ptsCoObjV,
                            uint32_t * pulAbortV)
{
   if(ptsCoObjV != 0)
   {
      if(pulAbortV == 0L)
      {
         switch(ptsCoObjV->ubMarker)
         {
            case eCOM_NODE_CMD_GET_INFO:
               ComDemoShowNodeInfo(ubNetV, ubNodeIdV);
               ComNodeSetHbProdTime(ubNetV, ubNodeIdV, 500);
               break;
            case eCOM_NODE_CMD_SET_HEARTBEAT:
               ComNmtSetHbConsTime(ubNetV, ubNodeIdV, 1000);

               //------------------------------------------
               // test for mCAN.8.dio-BOX
               //
               if( (atsComNodeS[ubNodeIdV - 1].ulIdx1018_VI == 0x0000000E) &&
                   (atsComNodeS[ubNodeIdV - 1].ulIdx1018_PC == 0x00139F7E) )
               {
                  ComDemoSetupPdoConfiguration(ubNetV, ubNodeIdV);
                  ComDemoWriteModuleConfiguration(ubNetV, ubNodeIdV);
               }
               else
               {
                  ComNmtSetNodeState(ubNetV, ubNodeIdV,
                                     eCOM_NMT_STATE_OPERATIONAL);
                  ubSdoConfigS = 0;
               }
               break;

            case eDEMO_NODE_CMD_WRITE_CONFIG:
               ComNmtSetNodeState(ubNetV, ubNodeIdV,
                                  eCOM_NMT_STATE_OPERATIONAL);
               ubNodeIdDigitalModuleS = ubNodeIdV;
               ubSdoConfigS = 0;
               break;

            default:

               break;
         }
      }
      else
      {
         ubSdoConfigS = 0;
      }

   }
}

//----------------------------------------------------------------------------//
// sig_handler_time()                                                         //
// timer                                                                      //
//----------------------------------------------------------------------------//
void sig_handler_time(int slSignalV)
{
   static uint32_t ulCounterTenMillisecondsS = 0;


   if(slSignalV == SIGALRM)
   {
      ulCounterTenMillisecondsS++;
      if(ulCounterTenMillisecondsS == 100)
      {
         ulCounterOneSecondS++;
         ulCounterTenMillisecondsS = 0;
      }
      ComMgrTimerEvent();
   }
}

//----------------------------------------------------------------------------//
// sig_handler_quit()                                                         //
// quit program                                                               //
//----------------------------------------------------------------------------//
void sig_handler_quit(int slSignalV)
{
   if(slSignalV == SIGINT)
   {
      ubProgRunS = 0;
   }
}


//----------------------------------------------------------------------------//
// init_signal_handler()                                                      //
// install signal handler for CTRL-C (SIGINT) and alarm (SIGALRM)             //
//----------------------------------------------------------------------------//
void init_signal_handler(void)
{
   struct sigaction   tsSigActionT;
   struct itimerval   tsTimerValT;

   tsSigActionT.sa_handler = sig_handler_quit;
   sigemptyset(&tsSigActionT.sa_mask);
   tsSigActionT.sa_flags = 0;
   sigaction(SIGINT, &tsSigActionT, 0);

   tsSigActionT.sa_handler = sig_handler_time;
   sigemptyset(&tsSigActionT.sa_mask);
   tsSigActionT.sa_flags = 0;
   sigaction(SIGALRM, &tsSigActionT, 0);

   //----------------------------------------------------------------
   // setup a 10 ms timer interval
   //
   tsTimerValT.it_value.tv_sec  = 1;
   tsTimerValT.it_value.tv_usec = 0;
   tsTimerValT.it_interval.tv_sec  = 0;
   tsTimerValT.it_interval.tv_usec = 10000;
   setitimer(ITIMER_REAL, &tsTimerValT, NULL);

}


//----------------------------------------------------------------------------//
// main()                                                                     //
//                                                                            //
//----------------------------------------------------------------------------//
int main(int argc, char *argv[])
{
   uint32_t ulTickOneSecondT = 0;
   ubProgRunS = 1;
   ubSdoConfigS = 0;


   //----------------------------------------------------------------
   // Print application information
   //
   printf("\n\n");
   printf("###############################################################################\n");
   printf("# uMIC.200 CANopen Master Example                                             #\n");
   printf("###############################################################################\n");
   printf("\n");
   printf("Use CTRL-C to quit this demo.\n");
   printf("\n");


   //----------------------------------------------------------------
   // Initialise the signal handler for timer and CTRL-C
   //
   init_signal_handler();


   //----------------------------------------------------------------
   // Initialise the CANopen master stack
   // The bitrate value is a dummy here, since the bitrate is
   // set via the socketcan configuration file.
   //
   ComMgrInit( eCP_CHANNEL_1, eCOM_NET_1, eCP_BITRATE_500K,
	           127, eCOM_MODE_NMT_MASTER);


   //----------------------------------------------------------------
   // Initialise application variables
   //
   ComDemoAppInit(eCOM_NET_1);

   //----------------------------------------------------------------
   // start the CANopen master stack
   //
   ComMgrStart(eCOM_NET_1);


   //----------------------------------------------------------------
   // start master detection procedure
   //
   ComNmtMasterDetection(eCOM_NET_1, 1);


   //----------------------------------------------------------------
   // CANopen master is heartbeat producer, 2s
   //
   ComNmtSetHbProdTime(eCOM_NET_1, 2000);


   //----------------------------------------------------------------
   // this is the main loop of the application
   //
   while(ubProgRunS)
   {
      while(ulTickOneSecondT == ulCounterOneSecondS)
      {
         sleep(10);
      }
      ulTickOneSecondT = ulCounterOneSecondS;
      ComDemoAppProcess(eCOM_NET_1);
   }

   //----------------------------------------------------------------
   // reset all nodes when master quits
   //
   ComNmtSetNodeState(eCOM_NET_1, 0, eCOM_NMT_STATE_RESET_COM);

   printf("\n");
   printf("Quit CANopen Master demo.\n");
   return(0);
}


