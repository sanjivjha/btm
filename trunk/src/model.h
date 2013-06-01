/**
 * Biterm topic model(BTM) with Gbbis sampling implementation
 * Author: Xiaohui Yan(xhcloud@gmail.com)
 * 2012-9-25
 */
#ifndef _MODEL_H
#define _MODEL_H

#include <vector>

#include "biterm.h"
#include "dataset.h"
#include "pvec.h"

using namespace std;

class Model {
protected:
  Dataset data;

  int M;						// n(w)
  int N;						// n(d)
  int K;				// topic number
  int n_iter;
  int save_step;

  double alpha;			// hyperparameters of p(z)
  double beta;			// hyperparameters of p(w|z)
  string dir;			// result dir
  string dfile;			// training file
  
  // sample recorders
  vector<int> _bz;	    // topic assignment of a biterm, size N*1
  vector<int> _nb_z;	// n(b|z), size K*1
  
  vector< vector<int> > _nwz;	  // n(w,z), size M*K
  vector<int> _nw_z;			// n(w|z)=\sum_w{n(w,z)}, size K*1

 public:
  Model(int K, double a, double b, int n_iter, int save_step, const string& dfile, const string& dir):
	K(K), alpha(a), beta(b), dfile(dfile), dir(dir), n_iter(n_iter), save_step(save_step) {}
  
  // run estimate and inference procedures
  void run();
  
 protected:
  // intialize memeber varibles
  void init();
  void estimate();

  // update estimate of a biterm
  void update_biterm(int b);
  
  // reset topic proportions for biterm b
  void reset_biterm_topic(int b);
  
  // assign topic proportions for biterm b
  void assign_biterm_topic(int b, int k);
  
  // compute condition distribution p(z|b)
  void compute_pz_b(int b, Pvec<double>& p); 

  void store_pz();
  void store_pw_z();
};

#endif
