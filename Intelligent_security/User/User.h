#ifndef __USER_H
#define __USER_H
 
#include "GSM.h"
#include "DHT11.h"
#include "encoder.h"


  void UserInitCallBack(void);
    void UserServiceCallBack(void);
      void BeepCall(uint8_t keeds);
      void UserTIM3ITCallBack(void);
      

#endif
