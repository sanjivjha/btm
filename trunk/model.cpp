#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <limits>
#include <ctime>
#include <algorithm>

#include <algorithm>
#include "sampler.h"
#include "model.h"
#include "pmat.h"

void Model::run() {
  read_train_data();
  
  cout << "init" << endl;
  init();
  
  cout << "begin estimate" << endl;
  estimate();
  
  cout << "store parameters" << endl;
  store_pz();
  store_pw_z();
}

// input format:did   w1    w2    freq
void Model::read_train_data() {
  printf("train docs:%s\n", dfile.c_str());

  ifstream rf( dfile.c_str() );
  if (!rf) 	EXIT_ERR("file not find:", dfile.c_str());
  
  string line;
  int max_wid = -1;
  while ( getline(rf, line) ) {
	int did, w1, w2, freq;
	istringstream iss(line);
	iss >> did >> w1 >> w2 >> freq;
	
	max_wid = max(max(w1, w2), max_wid);
	biterms.push_back(Biterm(w1, w2, freq));
  }

  rf.close();
  M = max_wid + 1;
  N = biterms.size();
  printf("n(w):%d, n(b):%d\n", M, N);
}

// n_status: number of latent status for each biterm
void Model::init() {
  srand(time(NULL));
  
  _nw_k.resize(K);
  _nwk.resize(M);
  for (int m = 0; m < M; ++m)
	_nwk[m].resize(K);

  _bz.resize(N);
  _nb_k.resize(K);
  
  for (int n=0; n < N; ++n) {
	int k = Sampler::uni_sample(K);
	assign_biterm_topic(n, k);
  }
}

void Model::estimate() {
  for (int iter=1; iter < n_iter+1; ++iter) {
	//lambda = 1/(1+exp(double(iter)/20));
	//cout << "iter " << iter << ", lambda ="<< lambda << endl;
	cout << "iter " << iter << endl;
	for (int b=0; b < N; ++b)
	  update_biterm(b);

	// test
	Pvec<double> pz(K);	          // p(z) = theta
	for (int k = 0; k < K; k++) {
	  pz[k] = (_nb_k[k] + alpha);
	}
	pz.normalize();
  	cout << pz.str(false) << endl;
	
	if (iter % save_step ==0) {
	  cout << "store parameters" << endl;
	  store_pz();
	  store_pw_z();
	}
	  
  }
  cout << "Gibbs completed!" << endl;
}

// sample procedure for biterm b
void Model::update_biterm(int b) {
  reset_biterm_topic(b);
  
  // compute p(z|b)
  Pvec<double> p;
  compute_pz_b(biterms[b], p);

  // sample topic for biterm b
  int k = Sampler::mult_sample(p.to_vector());
  assign_biterm_topic(b, k);
}

// reset topic assignment of biterm i
void Model::reset_biterm_topic(int i) {
  int w1 = biterms[i].first();
  int w2 = biterms[i].second();
  
  int k = _bz[i];
  int t = biterms[i].count();
  
  _nb_k[k] -= t;				// update proportion of biterms in topic K
  _nw_k[k] -= 2*t;				// update proportion of words in topic k
  _nwk[w1][k] -= t;			// update proportion w1's occurrence times in topic K
  _nwk[w2][k] -= t;

  assert(_nb_k[k]>=0 && _nw_k[k]>=0 && _nwk[w1][k]>=0 && _nwk[w2][k]>=0);
  _bz[i] = -1;
}

// compute p(z|w_i, w_j)
void Model::compute_pz_b(const Biterm& bi, Pvec<double>& p) {
  p.resize(K);
  
  int w1 = bi.first();
  int w2 = bi.second();
  double pw1k, pw2k;

  for (int k = 0; k < K; ++k) {
	double deno = _nw_k[k] + M * beta;
	
	pw1k = (_nwk[w1][k] + beta) / deno;
	pw2k = (_nwk[w2][k] + beta) / deno;
	p[k] = (_nb_k[k] + alpha) * pw1k * pw2k;
  }
  p.normalize();
  for (int k = 0; k < K; ++k)
	p[k] = (1-lambda)*p[k] + lambda/K;
}

// assign topic k to biterm i
void Model::assign_biterm_topic(int i, int k) {
  int w1 = biterms[i].first();
  int w2 = biterms[i].second();
  
  int t = biterms[i].count();
	
  _bz[i] = k;
  _nb_k[k] += t;
  _nw_k[k] += 2*t;
  _nwk[w1][k] += t;
  _nwk[w2][k] += t;
}

// p(z) is determinated by the overall proportions
// of biterms in it
void Model::store_pz() {
  Pvec<double> pz(K);	          // p(z) = theta
  for (int k = 0; k < K; k++) {
	pz[k] = (_nb_k[k] + alpha);
  }
  pz.normalize();
  
  // write
  ostringstream ss;
  ss << dir << "z.k" << K;
  string pt = ss.str();
  
  ofstream wf(pt.c_str());
  if (!wf) wf.open("z.txt");

  cout << "write p(z): " << pt << endl;
  wf << pz.str();
}

void Model::store_pw_z() {
  Pmat<double> pw_z(K, M);   // p(w|z) = phi, size K * M
  for (int k = 0; k < K; k++) {
	double deno = _nw_k[k] + M * beta;
	
	for (int m = 0; m < M; m++) 
	  pw_z[k][m] = (_nwk[m][k] + beta) / deno;
  }

  //void Model::write_pw_z()
  ostringstream ss;
  ss << dir << "zw.k" << K;
  string pt = ss.str();
  
  ofstream wf(pt.c_str());
  if (!wf) wf.open("zw.txt");

  cout << "write p(w|z): " << pt << endl;
  wf << pw_z.str();
}
