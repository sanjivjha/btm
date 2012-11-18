#ifndef _INFER_H
#define _INFER_H

#include <string>
#include "pvec.h"
#include "pmat.h"
#include "biterm.h"

using namespace std;

class Infer {
public:
  Infer(int K, const string& dfile, const string& dir): K(K), dfile(dfile), dir(dir){}
  void run();
  
private:
  void load_docs();
  void load_para();
  //inference topic proportions of docs via max(p(z|b))
  void doc_infer_sum(const vector<Biterm>& biterms, Pvec<double>&);
  void doc_infer_sum2(const vector<Biterm>& biterms, Pvec<double>&);
  void doc_infer_max(const vector<Biterm>& biterms, Pvec<double>&);
  void doc_infer_prod(const vector<Biterm>& biterms, Pvec<double>&); // infer as mixture of Unigrams

  void write_pz_d();
  
private:
  int K;
  
  vector<vector<Biterm> > docs;
  string dir;			// parameters dir
  string dfile;			// inference docs
  Pvec<double> _pz;	    // p(z) = theta
  Pmat<double> _pw_z;   // p(w|z) = phi, size K * M

  Pmat<double> _pz_d;   // p(w|z) = phi, size N * K
};

#endif
