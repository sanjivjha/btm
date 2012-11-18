#ifndef _BITERM_H
#define _BITERM_H

#include <cmath>
using std::min;
using std::max;

class Biterm {
public: 
  Biterm(){}
  Biterm(int w1, int w2, int freq):freq(freq) {
	wi = min(w1, w2);
	wj = max(w1, w2);
  }

  void incr() {freq += 1;}
  int count() const {return freq;}
  int first() const {return wi;}
  int second() const {return wj;}
  
private:
  int wi;
  int wj;
  int freq;
};

#endif
