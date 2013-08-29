/**
 * Base model of online BTM
 * Author: Xiaohui Yan(l0he1g@gmail.com)
 * 2013-6-6
 */
#ifndef _BTMUPTO_H
#define _BTMUPTO_H

#include <string>
#include "base.h"

using namespace std;

class BtmUpto : public Base {
private:
    int n_iter;
  
public:
  BtmUpto(int K, int W, double a, double b, int n_iter);

  void proc_dfile(string pt);
  
private:
  void proc_biterms();
};


#endif
