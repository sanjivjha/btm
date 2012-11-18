/**
 * Base model interface for biterm topic model(BTM) and
 * biter topic model with background(BBTM) with CVB0 solver
 * 2012-9-25
 */
#ifndef _MODEL_H
#define _MODEL_H

#include <vector>

#include "biterm.h"
#include <pvec.h>

using namespace std;

class Model {
protected:
  int N;				// biterms num
  int M;				// words num
  int K;
  int n_iter;
  int save_step;
  int n_thread;

  double lambda;
  double alpha;			// hyperparameters of p(z)
  double beta;			// hyperparameters of p(w|z)
  string dir;			// result dir
  string dfile;
  
  vector<Biterm> biterms;

  // sample recorders
  vector<int> _bz;	  // z|b, size N*1
  vector<int> _nw_k;			  // n(w)|k, size K*1
  vector< vector<int> > _nwk;	  // n(w,k), size M*K
  vector<int> _nb_k;				// n(b|k), size K*1

 public:
  Model(int K, double a, double b, double lam,  int n_iter, int save_step, const string& dfile, const string& dir):
	K(K), alpha(a), beta(b), lambda(lam), dfile(dfile), dir(dir), n_iter(n_iter), save_step(save_step) {}
  
  // run estimate and inference procedures
  void run();
  
 protected:
  void read_train_data();

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
  void compute_pz_b(const Biterm& bi, Pvec<double>& p); 

  void store_pz();
  void store_pw_z();
};

#endif
