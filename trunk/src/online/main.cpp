#include <cstdlib>
#include <string.h>
#include <string>
#include <iostream>
#include <ctime>

#include "str_util.h"
#include "base.h"
#include "btmUpto.h"
#include "obtm.h"
#include "ibtm.h"

using namespace std;

int main(int argc, char* argv[]) {
  string method(argv[1]);
  // common parameters
  int K = atoi(argv[2]);                  // topic num
  int W = atoi(argv[3]);
  int end_day = atoi(argv[4]);
  double alpha = atof(argv[5]);    // hyperparameters of p(z)
  double beta = atof(argv[6]);     // hyperparameters of p(w|z)
  string pt_init(argv[7]);
  string doc_dir(argv[8]);
  string res_dir(argv[9]);
  printf("=== %s: end_day %d , K %d, W %d, alpha %f, beta %f ====\n",
		 method.c_str(), end_day, K, W, alpha, beta);
  
  Base* mod;
  // model-sepcific parameters
  if (method == "btmUpto") {
	int n_iter = atoi(argv[10]);
	mod = new BtmUpto(K, W, alpha, beta, n_iter);
	printf("Model=btmUpto, n_iter=%d\n", n_iter);
  }
  else if (method == "obtm") {
	int n_iter = atoi(argv[10]);
	mod = new Obtm(K, W, alpha, beta, n_iter);
	printf("Model=obtm, n_iter=%d\n", n_iter);
  }
  else if (method == "ibtm") {
	int win = atoi(argv[10]);
	int n_rej = atoi(argv[11]);
	mod = new Ibtm(K, W, alpha, beta, win, n_rej);
	printf("Model=ibtm, win=%d, n_rej=%d\n", win, n_rej);
  }
  else {
	cout << "Unrecognized method:" << argv[1] << endl;
	exit(-1);
  }
  
  // load model init file
  mod->load_init(pt_init);

  for (int day = 1; day <= end_day; ++day) {
	cout << "# day " << day << endl;
	string pt_doc = doc_dir + str_util::itos(day) + ".txt";
	
	clock_t start = clock();
	mod->proc_dfile(pt_doc);
	clock_t end = clock();
	printf("%s, cost %fs\n", method.c_str(), double(end - start)/CLOCKS_PER_SEC);

	// save results
	string res_pfx = res_dir + "d" + str_util::itos(day) + "_";
	mod->save_res(res_pfx);
	mod->save_bz(res_pfx);
  }

  delete mod;
}
