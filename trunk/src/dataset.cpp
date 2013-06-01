#include <iostream>
#include <fstream>
#include <cstdlib>

#include "dataset.h"
// input, each line is a doc
// format: wid  wid  wid ...
// return: max wid
void Dataset::load_docs(const string& pt) {
  cout << "load docs:" << pt << endl;
  ifstream rf( pt.c_str() );
  if (!rf) {
	cout << "file not find:" << pt << endl;
	exit(-1);
  }

  string line;
  while(getline(rf, line)) {
	Doc d(line);
	ds.push_back(d);
  }
}

// return the maximum wid in docs
int Dataset::n_w() const {
  int M = 0;
  
  for (int i = 0; i < ds.size(); ++i) {
	const vector<int>& ws = ds[i].get_ws();
	
	for (int j = 0; j < ws.size(); ++j) 
	  if (M < ws[j]) M = ws[j];
  }

  return M+1;
}

void Dataset::extr_biterms() {
  for (int i = 0; i < ds.size(); ++i) {
	vector<Biterm> d_bs;
	ds[i].extr_biterms(d_bs);

	bs.insert(bs.end(), d_bs.begin(), d_bs.end());
  }
}
