#ifndef __European__
#define __European__
#include "gbm.h"
#include <vector>
#include <string>

class European{
public:
   European(double delivery_time, double street_price);
   ~European(){};

   double GetPrice_Call();
   double GetPrice_Put();

   //warning: guardare l'implementazione!!!!
   void ToDatePrice(gbm Asset, double this_time);

   //questa sarebbe l'implementazione sensata
   void UpdatePrice(gbm Asset, double this_time);

private:
   double _EuropeanCall;
   double _EuropeanPut;
   double _T, _K;

};

#endif
