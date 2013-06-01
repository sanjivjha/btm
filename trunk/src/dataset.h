#ifndef _DATASET_H
#define _DATASET_H

#include <string>
#include <vector>

#include "doc.h"
#include "biterm.h"

using namespace std;

class Dataset {
private:
  vector<Doc> ds;
  vector<Biterm> bs;
  
public:
  Dataset() {}

  void load_docs(const string& pt);

  // dimension
  int n_d() const {return ds.size();}
  int n_w() const;
  int n_b() const {return bs.size();}

  // sub data
  const vector<Doc>& get_ds() const {return ds;}
  const Doc& get_d(int i) const {return ds[i];}
  
  const vector<Biterm>& get_bs() const {return bs;}
  const Biterm& get_b(int i) const {return bs[i];}
  
  // extract biterms from docs
  void extr_biterms();
};
#endif

