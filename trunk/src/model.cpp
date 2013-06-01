#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <limits>
#include <ctime>
#include <algorithm>

#include <algorithm>
#include <sampler.h>
#include "model.h"
#include "pmat.h"

void Model::run() {
  data.load_docs(dfile);
  data.extr_biterms();
  N = data.n_b();
  M = data.n_w();
  cout << "n(d):" << data.n_d() 
	  << ", n(w):" << data.n_w()
	  << ", n(b):" << data.n_b() << endl;

  cout << "init" << endl;
  init();
  
  cout << "begin estimate" << endl;
  estimate();
  
  cout << "store parameters" << endl;
  store_pz();
  store_pw_z();
}

// n_status: number of latent status for each biterm
void Model::init() {
  srand(time(NULL));
  
  _nw_z.resize(K);
  _nwz.resize(M);
  for (int m = 0; m < M; ++m)
	_nwz[m].resize(K);

  _bz.resize(N);
  _nb_z.resize(K);
  
  for (int n=0; n < N; ++n) {
	int k = Sampler::uni_sample(K);
	assign_biterm_topic(n, k);
  }
}

void Model::estimate() {
  for (int iter=1; iter < n_iter+1; ++iter) {
	cout << "iter " << iter << endl;
	for (int b=0; b < N; ++b)
	  update_biterm(b);

	if (iter % save_step ==0) {
	  cout << "store parameters" << endl;
	  store_pz();
	  store_pw_z();
	}
	  
  }
  cout << "Gibbs completed!" << endl;
}

// sample procedure for ith biterm 
void Model::update_biterm(int i) {
  reset_biterm_topic(i);
  
  // compute p(z|b)
  Pvec<double> p;
  compute_pz_b(i, p);

  // sample topic for biterm b
  int k = Sampler::mult_sample(p.to_vector());
  assign_biterm_topic(i, k);
}

// reset topic assignment of biterm i
void Model::reset_biterm_topic(int i) {
  const Biterm& bi = data.get_b(i);
  int w1 = bi.first();
  int w2 = bi.second();
  
  int k = _bz[i];
  int t = bi.count();
  
  _nb_z[k] -= t;				// update proportion of biterms in topic K
  _nw_z[k] -= 2*t;				// update proportion of words in topic k
  _nwz[w1][k] -= t;			// update proportion w1's occurrence times in topic K
  _nwz[w2][k] -= t;

  assert(_nb_z[k]>=0 && _nw_z[k]>=0 && _nwz[w1][k]>=0 && _nwz[w2][k]>=0);
  _bz[i] = -1;
}

// compute p(z|w_i, w_j)
void Model::compute_pz_b(int i, Pvec<double>& p) {
  p.resize(K);
  
  const Biterm& bi = data.get_b(i);
  int w1 = bi.first();
  int w2 = bi.second();
  
  double pw1k, pw2k;

  for (int k = 0; k < K; ++k) {
	double deno = _nw_z[k] + M * beta;
	
	pw1k = (_nwz[w1][k] + beta) / deno;
	pw2k = (_nwz[w2][k] + beta) / deno;
	p[k] = (_nb_z[k] + alpha) * pw1k * pw2k;
  }
  p.normalize();
}

// assign topic k to biterm i
void Model::assign_biterm_topic(int i, int k) {
  const Biterm& bi = data.get_b(i);
  int w1 = bi.first();
  int w2 = bi.second();
  int t = bi.count();
	
  _bz[i] = k;
  _nb_z[k] += t;
  _nw_z[k] += 2*t;
  _nwz[w1][k] += t;
  _nwz[w2][k] += t;
}

// p(z) is determinated by the overall proportions
// of biterms in it
void Model::store_pz() {
  Pvec<double> pz(K);	          // p(z) = theta
  for (int k = 0; k < K; k++) {
	pz[k] = (_nb_z[k] + alpha);
  }
  pz.normalize();
  
  // write
  ostringstream ss;
  ss << dir << "z.k" << K;
  string pt = ss.str();
  
  cout << "write p(z): " << pt << endl;
  pz.write(pt);
}

void Model::store_pw_z() {
  Pmat<double> pw_z(K, M);   // p(w|z) = phi, size K * M
  for (int k = 0; k < K; k++) {
	double deno = _nw_z[k] + M * beta;
	
	for (int m = 0; m < M; m++) 
	  pw_z[k][m] = (_nwz[m][k] + beta) / deno;
  }

  //void Model::write_pw_z()
  ostringstream ss;
  ss << dir << "zw.k" << K;
  string pt = ss.str();
  
  cout << "write p(w|z): " << pt << endl;
  pw_z.write(pt);
}
