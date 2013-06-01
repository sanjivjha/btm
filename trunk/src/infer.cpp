#include <iostream>
#include <sstream>
#include "infer.h"

Infer::Infer(int K, int n_iter, const string& dfile, const string& dir):
  K(K), n_iter(n_iter), dfile(dfile), dir(dir){
  ostringstream ss;
  ss << "zd.k" << K;
  string suf = ss.str();
  string pt = dir + suf;

  cout << "write p(z|d):" << pt << endl;
  wf.open(pt.c_str());
  if (!wf) {
	cout << "[Error]Wirte:" << pt.c_str() << endl;
	exit(-1);
  }
}

void Infer::run() {
  data.load_docs(dfile);
  
  load_para();
  
  cout << "Infer topic proportions for docs." << endl;
  const vector<Doc>& docs = data.get_ds();
  for (int d = 0; d < docs.size(); ++d) {
	if (d % 10000 == 0)
	  cout << "infer progress: " << d << '/' << docs.size() << endl;
	
	Pvec<double> pz_d(K);
	doc_infer_sum_b(data.get_d(d), pz_d);
	// doc_infer_sum_w(d, pz_d);

	// write p(z|d) for d, a doc a time
	wf << pz_d.str() << endl;
  }  
}

void Infer::load_para() {
  ostringstream ss;
  string pt;
  
  ss << dir << "z.k" << K;
  pt = ss.str();
  cout << "load p(z):" << pt <<endl;
  _pz.load_data(pt);

  ostringstream s2;
  s2 << dir << "zw.k" << K;
  pt = s2.str();
  cout << "load p(w|z):" << pt <<endl;
  _pw_z.load_data(pt);
}

// compute p(z|d, w) \proto p(w|z)p(z|d)
void Infer::compute_pz_dw(int w, const Pvec<double>& pz_d, Pvec<double>& p) {
  p.resize(K);
  
  for (int k = 0; k < K; ++k) {
	p[k] = _pw_z[k][w] * pz_d[k];
  }
  p.normalize();
}


// p(z|d) = \sum_b{ p(z|b)p(b|d) }
void Infer::doc_infer_sum_b(const Doc& doc, Pvec<double>& pz_d) {
  pz_d.assign(K, 0);
  
  if (doc.size() == 1) {
	// doc is a single word, p(z|d) = p(z|w) \propo p(z)p(w|z)
	for (int k = 0; k < K; ++k) 
	  pz_d[k] = _pz[k] * _pw_z[k][doc.get_w(0)];
  }
  else {
	// more than one words
	vector<Biterm> bis;
	doc.extr_biterms(bis);
  
	for (vector<Biterm>::const_iterator it = bis.begin();
		 it != bis.end();
		 ++it) {
	  int w1 = it->first();
	  int w2 = it->second();
	
	  // compute p(z|b) \propo p(w1|z)p(w2|z)p(z)
	  Pvec<double> pz_b(K);
	  for (int k = 0; k < K; ++k) {
		assert(_pw_z[k][w1]>0 && _pw_z[k][w2]>0);
		pz_b[k] = _pz[k] * _pw_z[k][w1] * _pw_z[k][w2];
	  }
	  pz_b.normalize();
	
	  // sum for b, p(b|d) is unifrom
	  for (int k = 0; k < K; ++k) 
		pz_d[k] += pz_b[k];
	}
  }
  pz_d.normalize();
}

// p(z|d) = \sum_w{ p(z|w)p(w|d) }
void Infer::doc_infer_sum_w(const Doc& doc, Pvec<double>& pz_d) {
  pz_d.assign(K, 0);

  const vector<int>& ws = doc.get_ws();
  for (vector<int>::const_iterator it = ws.begin();
	   it != ws.end();
	   ++it) {
	int w = *it;
	
	// compute p(z|w) \propo p(w|z)p(z)
	Pvec<double> pz_w(K);
	for (int k = 0; k < K; ++k) 
	  pz_w[k] = _pz[k] * _pw_z[k][w];
	
	pz_w.normalize();
	
	// sum for b, p(b|d) is unifrom
	for (int k = 0; k < K; ++k) 
	  pz_d[k] += pz_w[k];
  }
  pz_d.normalize();
}
