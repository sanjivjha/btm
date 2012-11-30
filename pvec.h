/*
 * Positive vector
 */
#ifndef _PVEC_H
#define _PVEC_H
#include <vector>
#include <string>
#include <cassert>
#include <cmath>
#include <ctime>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

#define EXIT_ERR( s1, s2 ) {printf("[Error] %s%s\n", s1, s2);	\
	exit(EXIT_FAILURE);}

using namespace std;

template<class T>
class Pvec
{
private:
  vector<T> p;
    
public:
  Pvec(){}
  Pvec(size_t n): p(n) {}
  Pvec(const vector<T>& v): p(v) {}
  Pvec(const Pvec<T>& v): p(v.p) {}
  Pvec(const string& line);
  
  void resize(size_t n) {p.resize(n);}
  void assign(size_t n, const T& v) {p.assign(n, v);}
  
  void rand_init();
  void uniform_init();
  void bias_init(double v);		// fix p[0] = v
  
  void push_back(T v) {p.push_back(v);}
  
  void load_data(const string& inf);
  T sum() const;
  T norm() const;
  void normalize(double smoother=0);
  void exp_normalize();   // normalize to all exponents of entries sum to 1

  Pvec<T> &operator= (const vector<T>& v) {p=v; return *this;}
  
  T &operator[] ( size_t i ) {
	assert(i<p.size());
	return p[i];
  }

  const T &operator[] ( size_t i ) const{
	assert(i<p.size());
	return p[i];
  }

  Pvec<T> operator+(const T & v){
	vector<T> tp(p.size());
	for (int i=0; i<p.size(); ++i)
	  tp[i] = p[i] + v;
	return tp;
  }

  Pvec<T>& operator+=(const T & v){
	vector<T> tp(p.size());
	for (int i=0; i<p.size(); ++i)
	  p[i] += v;
	return *this;
  }

  Pvec<T> operator+(const Pvec& v){
	vector<T> tp(p.size());
	for (int i=0; i<p.size(); ++i)
	  tp[i] = p[i] + v[i];
	return tp;
  }

  Pvec<T> &operator+=(const Pvec& v){
	for (int i=0; i<p.size(); ++i)
	  p[i] += v[i];
	return *this;
  }

  Pvec<T> operator-(const T & v){
	vector<T> tp(p.size());
	for (int i=0; i<p.size(); ++i)
	  tp[i] = p[i] - v;
	return tp;
  }

  Pvec<T>& operator-=(const T & v){
	vector<T> tp(p.size());
	for (int i=0; i<p.size(); ++i)
	  p[i] -= v;
	return *this;
  }

  Pvec<T> operator-(const Pvec& v){
	vector<T> tp(p.size());
	for (int i=0; i<p.size(); ++i)
	  tp[i] = p[i] - v[i];
	return tp;
  }

  Pvec<T> &operator-=(const Pvec& v){
	for (int i=0; i<p.size(); ++i)
	  p[i] -= v[i];
	return *this;
  }

  Pvec<T> operator*(const T & v){
	vector<T> tp(p.size());
	for (int i=0; i<p.size(); ++i)
	  tp[i] = p[i] * v;
	return tp;
  }

  size_t size() const {return p.size();}
  string str(bool col=true) const;
  vector<T> to_vector() {return p;}
};

template<class T>
Pvec<T>::Pvec(const string& line) {
  p.clear();
  istringstream iss(line);
  T v;
  while (iss >> v) {
	p.push_back(v);
  }
}

template<class T>
void Pvec<T>::rand_init() {
  for (size_t i = 0; i < p.size(); ++i) 
	p[i] = rand() % 100 + 1;

  normalize();
}

template<class T>
void Pvec<T>::uniform_init() {
  for( size_t i = 0 ; i < p.size() ; ++i ) 
	p[i] = (double)(1 / p.size());
}

template<class T>
void Pvec<T>::bias_init(double v) {
  assert( v < 1);
  p[0] = v;
  for( size_t i = 1 ; i < p.size() ; ++i ) 
	p[i] = (double)((1-v) / (p.size()-1));
}

// load a varible an line, make sure no empty lines
// the number of rows determinates the dimension of vector
template<class T>
void Pvec<T>::load_data(const string& inf) {
  ifstream rf(inf.c_str());
  if (!rf) 
	EXIT_ERR("file not find:", inf.c_str());

  p.clear();
  string line;
  while (getline(rf, line)) {
	istringstream iss(line);
	T v;
	iss >> v;
	p.push_back(v);
  }

  normalize();
}

template<class T>
T Pvec<T>::sum() const{
  T s = 0;
  for( size_t i = 0 ; i < p.size() ; ++i ) 
	s += p[i];
  return s;
}

template<class T>
T Pvec<T>::norm() const{
  T s = 0;
  for( size_t i = 0 ; i < p.size() ; ++i ) 
	s += p[i]*p[i];
  return sqrt(s);
}

template<class T>
void Pvec<T>::normalize(double smoother) {
  T s = sum();
  int K = p.size();
  //assert(s>0);
  // avoid numerical problem
  for( size_t i = 0 ; i < K ; ++i ) 
	p[i] = (p[i] + smoother)/(s + K*smoother);
}

template<class T>
void Pvec<T>::exp_normalize() {
  vector<T> tmp(p);
  for (size_t i = 0; i < p.size(); ++i ) {
    double s = 0.0;
	for (size_t j = 0; j < p.size(); ++j ) {
	  s += exp( tmp[j] - tmp[i] );
	}
	
	assert(s>=1);
    p[i] = 1/s;
  }
}


template<class T>
string Pvec<T>::str(bool col) const{
  char delim;
  if (col) delim = '\n';		// write in a column
  else delim = '\t';			// write in a row
  
  ostringstream os;
  size_t i;
  for( i = 0 ; i < p.size()-1; ++i ) 
	os << p[i] << delim;
  
  if (i < p.size())
	os << p[i];
  return os.str();
}

#endif
