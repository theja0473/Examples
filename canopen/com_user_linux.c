//****************************************************************************//
// File:          com_user_linux.c                                            //
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
** IMPORTANT NOTE                                                             **
** This file provides a template for all functions that need to be adopted to **
** YOUR hardware. Copy this file to <myfile.c> and make your changes in the   **
** new file.                                                                  **
\*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*\
** Include files                                                              **
**                                                                            **
\*----------------------------------------------------------------------------*/


#include "canopen_master.h"

#include <stdio.h>


/*----------------------------------------------------------------------------*\
** Variables                                                                  **
**                                                                            **
\*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*\
** Functions                                                                  **
**                                                                            **
\*----------------------------------------------------------------------------*/

//----------------------------------------------------------------------------//
// ComEmcyConsEventReceive()                                                  //
//                                                                            //
//----------------------------------------------------------------------------//
void ComEmcyConsEventReceive(uint8_t ubNetV, uint8_t ubNodeIdV)
{
   uint8_t  aubDataT[8];
   uint16_t uwEmcyCodeT;

   ComEmcyConsGetData(ubNetV,ubNodeIdV,&aubDataT[0]);
   uwEmcyCodeT = aubDataT[2];
   uwEmcyCodeT = uwEmcyCodeT << 8;
   uwEmcyCodeT = uwEmcyCodeT | aubDataT[1];

   printf("can%d: NID %03d - EMCY code %04X, error register value %d\n",
           ubNetV, ubNodeIdV, uwEmcyCodeT, aubDataT[0]);
}


//----------------------------------------------------------------------------//
// ComLssEventReceive()                                                       //
// Function handler for LSS reception                                         //
//----------------------------------------------------------------------------//
void ComLssEventReceive(uint8_t __attribute__((unused)) ubNetV,
                        uint8_t __attribute__((unused)) ubLssProtocolV)
{

}



//----------------------------------------------------------------------------//
// ComMgrEventBus()                                                           //
// Handler for Bus events                                                     //
//----------------------------------------------------------------------------//
void ComMgrEventBus(uint8_t ubNetV, CpState_ts * ptsBusStateV)
{
   //----------------------------------------------------------------
   // Initialise user parameter
   //
   switch (ubNetV)
   {
      case eCOM_NET_1 :
         if(ptsBusStateV->ubCanErrState == CP_STATE_BUS_OFF)
         {
            //---------------------------------------------
            // handle bus-off condition
            //
         }
         break;


      default:

         break;
   }

}


//----------------------------------------------------------------------------//
// ComUserInit()                                                              //
//                                                                            //
//----------------------------------------------------------------------------//
ComStatus_tv ComMgrMasterInit(uint8_t __attribute__((unused)) ubNetV)
{

   return(eCOM_ERR_OK);
}


//----------------------------------------------------------------------------//
// ComNmtEventActiveMaster()                                                  //
// Handler for Active Master detection                                        //
//----------------------------------------------------------------------------//
void ComNmtEventActiveMaster( uint8_t __attribute__((unused)) ubNetV,
                              uint8_t __attribute__((unused)) ubPriorityV,
                              uint8_t __attribute__((unused)) ubNodeIdV)
{

}


//----------------------------------------------------------------------------//
// ComNmtEventHeartbeat()                                                     //
// Handler for NMT events                                                     //
//----------------------------------------------------------------------------//
void ComNmtEventHeartbeat(uint8_t ubNetV, uint8_t ubNodeIdV)
{
   printf("can%d: NID %03d - No heartbeat received\n", ubNetV, ubNodeIdV);
}


//----------------------------------------------------------------------------//
// ComNmtEventIdCollision()                                                   //
// Handler for NMT events                                                     //
//----------------------------------------------------------------------------//
void ComNmtEventIdCollision(uint8_t ubNetV)
{
   printf("can%d: COB-ID collision.\n", ubNetV);
}


//----------------------------------------------------------------------------//
// ComNmtEventMasterDetection()                                               //
// Handler for NMT master detection events                                    //
//----------------------------------------------------------------------------//
void ComNmtEventMasterDetection(uint8_t ubNetV, uint8_t ubResultV)
{
   //----------------------------------------------------------------
   // In case of timeout: we are the active CANopen master
   //
   if(ubResultV == eCOM_NMT_DETECT_TIMEOUT)
   {
      //----------------------------------------------------------------
      // reset all nodes
      //
      ComNmtSetNodeState(ubNetV, 0, eCOM_NMT_STATE_RESET_COM);

      //----------------------------------------------------------------
      // set the cycle time to 500 ms
      //
      ComSyncSetCycleTime(ubNetV, 500000);
      ComSyncEnable(ubNetV, 1);
   }
}


//----------------------------------------------------------------------------//
// ComNmtEventResetCommunication()                                            //
// Handler for NMT events                                                     //
//----------------------------------------------------------------------------//
void ComNmtEventResetCommunication(uint8_t __attribute__((unused)) ubNetV)
{

}


//----------------------------------------------------------------------------//
// ComNmtEventResetNode()                                                     //
// Handler for NMT events                                                     //
//----------------------------------------------------------------------------//
void ComNmtEventResetNode(uint8_t __attribute__((unused)) ubNetV)
{

}


//----------------------------------------------------------------------------//
// ComNmtEventStateChange()                                                   //
// Handler for NMT events                                                     //
//----------------------------------------------------------------------------//
void ComNmtEventStateChange(uint8_t ubNetV, uint8_t ubNodeIdV,
                            uint8_t ubNmtEventV)
{
   switch(ubNmtEventV)
   {
      case eCOM_NMT_STATE_BOOTUP:
    	  printf("can%d: NID %03d - received boot-up message\n",
    			  ubNetV, ubNodeIdV);
    	  break;

      case eCOM_NMT_STATE_PRE_OPERATIONAL:
    	  printf("can%d: NID %03d - switched to pre-operational state\n",
    			  ubNetV, ubNodeIdV);
    	  break;

      case eCOM_NMT_STATE_OPERATIONAL:
    	  printf("can%d: NID %03d - switched to operational state\n",
    			  ubNetV, ubNodeIdV);
    	  break;

      default:

    	  break;
   }
}


//----------------------------------------------------------------------------//
// ComPdoEventReceive()                                                       //
// Function handler for PDO Receive                                           //
//----------------------------------------------------------------------------//
void ComPdoEventReceive(uint8_t ubNetV, uint16_t uwPdoV)
{
   uint8_t  aubPdoDataT[8];

   if(ubNetV == eCOM_NET_1)
   {
      ComPdoGetData(ubNetV, uwPdoV, &aubPdoDataT[0]);
   }
}





//----------------------------------------------------------------------------//
// ComSdoSrvBlkUpObjectSize()                                                 //
// Function handler for SDO server block transfer                             //
//----------------------------------------------------------------------------//
uint32_t ComSdoSrvBlkUpObjectSize(uint8_t __attribute__((unused)) ubNetV,
                                  uint16_t __attribute__((unused)) uwIndexV,
                                  uint8_t __attribute__((unused)) ubSubIndexV)
{
   uint32_t ulObjSizeT = 0;


   return ulObjSizeT;
}


