/**
* \file
* \brief Field Monitoring Handling
* Copyright (C) 2020, 2021, Ing.-Buero Dr. Michael Lehning, Hildesheim
* Copyright (C) 2020, 2021, SICK AG, Waldkirch
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Osnabrück University nor the names of its
*       contributors may be used to endorse or promote products derived from
*       this software without specific prior written permission.
*     * Neither the name of SICK AG nor the names of its
*       contributors may be used to endorse or promote products derived from
*       this software without specific prior written permission
*     * Neither the name of Ing.-Buero Dr. Michael Lehning nor the names of its
*       contributors may be used to endorse or promote products derived from
*       this software without specific prior written permission
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*  Last modified: 29th May 2018
*
*      Authors:
*         Michael Lehning <michael.lehning@lehning.de>
*
*/

/*
#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#pragma warning(disable: 4996)
#pragma warning(disable: 4267)
#endif

#ifndef _MSC_VER


#endif

#include <sick_scan/sick_scan_common_nw.h>
#include <sick_scan/RadarScan.h> // generated by msg-generator

#ifndef _MSC_VER

#include <dynamic_reconfigure/server.h>
#include <sick_scan/SickScanConfig.h>

#endif

#include "sick_scan/sick_generic_parser.h"
#include "sick_scan/sick_scan_common_nw.h"

#include <sick_scan/sick_scan_common_tcp.h>
#include <sick_scan/sick_generic_parser.h>
#include <sick_scan/sick_generic_field_mon.h>
#ifdef _MSC_VER
#include "sick_scan/rosconsole_simu.hpp"
#endif
#define _USE_MATH_DEFINES

#include <math.h>
#include "string"
#include <stdio.h>
#include <stdlib.h>
*/
#include <sick_scan/sick_scan_common.h>
#include <sick_scan/sick_generic_field_mon.h>

namespace sick_scan
{


/* Null, because instance will be initialized on demand. */
  SickScanFieldMonSingleton *SickScanFieldMonSingleton::instance = 0;

  SickScanFieldMonSingleton *SickScanFieldMonSingleton::getInstance()
  {
    if (instance == 0)
    {
      instance = new SickScanFieldMonSingleton();
    }

    return instance;
  }

  SickScanFieldMonSingleton::SickScanFieldMonSingleton()
  {
    this->monFields.resize(48);
    // just for debugging, but very helpful for the start
    this->active_mon_fieldset = 0;
  }

   /*!
  \brief Parse binary LIDinputstate message and set active field set
  \param datagramm: Pointer to datagram data
  \param datagram_length: Number of bytes in datagram
  */
  int SickScanFieldMonSingleton::parseAsciiLIDinputstateMsg(unsigned char* datagram, int datagram_length)
  {
    ROS_ERROR("SickScanFieldMonSingleton::parseAsciiLIDinputstateMsg not implemented.");
    int exitCode=ExitSuccess;
    return (exitCode);
  }

  /*!
  \brief Parse binary LIDinputstate message and set active field set
  \param datagramm: Pointer to datagram data
  \param datagram_length: Number of bytes in datagram
  */
  int SickScanFieldMonSingleton::parseBinaryLIDinputstateMsg(unsigned char* datagramm, int datagram_length)
  {
    int exitCode=ExitSuccess;
    if(datagram_length > 36)
    {
      int fieldset = 0;
      for(int offset = 35; offset >= 32; offset--) // datagramm[32]=INT1, datagramm[33]=INT2, datagramm[34]=INT3, datagramm[35]=INT4 
      {
        fieldset = (fieldset << 1);
        fieldset |= ((datagramm[offset] != 0) ? 1 : 0);
      }
      setActiveFieldset(fieldset);
    }
    else
    {
      exitCode = ExitError;
    }
    return exitCode;
  }

  /*!
  \brief Parsing Ascii datagram
  \param datagram: Pointer to datagram data
  \param datagram_length: Number of bytes in datagram
  */
  int SickScanFieldMonSingleton::parseAsciiDatagram(std::vector<unsigned char> datagramm)
  {
    ROS_ERROR("SickScanFieldMonSingleton::parseAsciiDatagram not implemented.");
    int exitCode=ExitSuccess;
    return (exitCode);
  }

  int SickScanFieldMonSingleton::parseBinaryDatagram(std::vector<unsigned char> datagram)
  {
    int exitCode = ExitSuccess;
    int fieldNumberFromCMD=0;
    std::string sDatagramm( datagram.begin()+8, datagram.end() );
    sscanf(sDatagramm.c_str(), "sRA field%d", &fieldNumberFromCMD);
    float distScaleFactor;
    float distScaleFactorOffset;
    uint32_t angScaleFactor;
    int32_t angScaleFactorOffset;
    uint8_t fieldType;
    uint8_t fieldNumber;
    uint16_t segmentedFieldCOnfigured;

    unsigned char *dataPtr= &(datagram[0]);
    memcpy(&distScaleFactor, dataPtr  + 21, 4);
    memcpy(&distScaleFactorOffset, dataPtr  + 25, 4);
    memcpy(&angScaleFactor, dataPtr  + 29, 4);
    memcpy(&angScaleFactorOffset, dataPtr  + 33, 4);
    memcpy(&fieldType, dataPtr  + 37, 1);
    memcpy(&fieldNumber, dataPtr  + 38, 1);
    memcpy(&segmentedFieldCOnfigured, dataPtr  + 39, 2);
    swap_endian((unsigned char *) &distScaleFactor, 4);
    swap_endian((unsigned char *) &distScaleFactorOffset, 4);
    swap_endian((unsigned char *) &angScaleFactor, 4);
    swap_endian((unsigned char *) &angScaleFactorOffset, 4);
    swap_endian((unsigned char *) &fieldType, 1);
    swap_endian((unsigned char *) &fieldNumber, 1);
    swap_endian((unsigned char *) &segmentedFieldCOnfigured, 2);
    if(segmentedFieldCOnfigured==1)//only segmented fields are supported at the moment
    {
      uint16_t numOfFieldPoints;
      uint16_t angIDX;
      uint16_t startDist,stopDist;
      memcpy(&numOfFieldPoints, dataPtr  + 41, 2);
      swap_endian((unsigned char *) &numOfFieldPoints, 2);
      for(uint16_t point=0;point<numOfFieldPoints;point++)
      {
        memcpy(&angIDX, dataPtr  + 43+point*6, 2);
        memcpy(&startDist, dataPtr  + 45+point*6, 2);
        memcpy(&stopDist, dataPtr  + 47+point*6, 2);
        swap_endian((unsigned char *) &angIDX, 2);
        swap_endian((unsigned char *) &startDist, 2);
        swap_endian((unsigned char *) &stopDist, 2);
        float angRad=(angIDX*angScaleFactor/1e4+angScaleFactorOffset/1e4)*deg2rad;
        float distMeter=(stopDist*distScaleFactor+distScaleFactorOffset)/1000;//TODO check 1000
        monFields[fieldNumberFromCMD].pushPoint(distMeter, angRad);
      }

    }

    return (exitCode);
  }

}
