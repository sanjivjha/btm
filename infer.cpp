#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include "infer.h"

// load docs to be inferred
// input format:did   w1    w2    freq
void Infer::load_docs(){
  printf("infer docs:%s\n", dfile.c_str());
  ifstream rf( dfile.c_str() );
  if (!rf) 	EXIT_ERR("file not find:", dfile.c_str());
  
  string line;
  while ( getline(rf, line) ) {
    if (line.empty()) continue;
	int did, w1, w2, freq;
	istringstream iss(line);
	iss >> did >> w1 >> w2 >> freq;

	// insert a biterm for doc did
	Biterm b(w1, w2, freq);
	while (docs.size() < did + 1) {
	  vector<Biterm> doc;
	  docs.push_back(doc);
	}
	docs[did].push_back(b);
  }

  printf("n(d):%d\n", docs.size());
}

void Infer::run() {
  load_docs();
  
  load_para();

  ostringstream ss;
  ss << dir << "zd.k" << K;
  string pt = ss.str();
  cout << "write p(z|d): " << pt << endl;
  ofstream wf(pt.c_str());
  if (!wf) wf.open("zd.txt");

  cout << "Infer topic proportions for docs." << endl;
  for (int d=0; d<docs.size(); ++d) {
    	if (d % 100000 == 0)
	  cout << "progress: " << d << '/' << docs.size() << endl;
	
	Pvec<double> d_pz;
	//doc_infer_max(docs[d], d_pz);
	doc_infer_sum(docs[d], d_pz);
	//doc_infer_prod(docs[d], d_pz);

	wf << d_pz.str(false) << endl;
	//_pz_d.add_row(d_pz);
  }
  
  //  write_pz_d();
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

// p(z|d) = \sum_b{ p(z|b)p(b|d) }
void Infer::doc_infer_sum(const vector<Biterm>& bis, Pvec<double>& d_pz) {
  d_pz.assign(K, 0);
  // more than one words
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
	  d_pz[k] += pz_b[k];
  }
  d_pz.normalize();
}

// p(z|d) = \sum_b_\in_D{ p(b,z)=p(w1|z)p(w2|z)p(z) }
void Infer::doc_infer_sum2(const vector<Biterm>& bis, Pvec<double>& d_pz) {
  d_pz.assign(K, 0);
  // more than one words
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
	//pz_b.normalize();
	
	// sum for b, p(b|d) is unifrom
	for (int k = 0; k < K; ++k) 
	  d_pz[k] += pz_b[k];
  }
  d_pz.normalize();
}

// p(z|d)~ max_b(p(z|b))
void Infer::doc_infer_max(const vector<Biterm>& bis, Pvec<double>& d_pz) {
  d_pz.assign(K, 0);
  // more than one words
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
	  if (d_pz[k] < pz_b[k])
		d_pz[k] = pz_b[k];
  }
  d_pz.normalize();
}

// p(z|d) = \prod_b{ p(z|b)p(b|d) }
void Infer::doc_infer_prod(const vector<Biterm>& bis, Pvec<double>& d_pz) {
  d_pz.assign(K, 0);
  // more than one words
  for (vector<Biterm>::const_iterator it = bis.begin();
	   it != bis.end();
	   ++it) {
	int w1 = it->first();
	int w2 = it->second();
	
	// compute p(z|b) \propo p(w1|z)p(w2|z)p(z)
	for (int k = 0; k < K; ++k) 
	  d_pz[k] += log(_pw_z[k][w1]) + log(_pw_z[k][w2]);	
  }
  
	// sum for b, p(b|d) is unifrom
  for (int k = 0; k < K; ++k) 
	d_pz[k] = 0.5 * d_pz[k] + log(_pz[k]);
  d_pz.exp_normalize();
}

// // pz_d size D, write D*K
// void Infer::write_pz_d() {
//   ostringstream ss;
//   ss << dir << "zd.k" << K;
//   string pt = ss.str();
  
//   cout << "write p(z|d): " << pt << endl;
//   ofstream wf(pt.c_str());
//   if (!wf) wf.open("zd.txt");

//   for (int i=0; i < _pz_d.rows(); ++i) 
//     for (int c=0; c < _pz_d.cols(); ++c) {
// 	  wf << _pz_d[i][c] << " ";
// 	wf << endl;
//   }
// }
