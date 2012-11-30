#ifndef _SAMPLER_H
#define _SAMPLER_H
#include <cstdlib>
#include <vector>
using namespace std;

class Sampler {
public:
  // uniform sample Mult(1/K)
  static int uni_sample(int K) {
	int k = rand() % K;
	return k;
  }
  
  // sample from Mult(p)
  static int mult_sample(vector<double> p) {
	int K = p.size();
	for (int i = 1; i < K; i++) 
	  p[i] += p[i-1];
  
	double u = (double)rand() / RAND_MAX;
	int k;  						// record sampled index
	for ( k = 0; k < K; k++) 
	  if (p[k] >= u * p[K-1]) break;
	
	if (k == K)
	  --k;

	return k;
  }
};

#endif
