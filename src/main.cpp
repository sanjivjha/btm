#include <cstdlib>
#include <string.h>
#include <string>
#include <cstdlib>
#include <string.h>
#include <string>
#include <iostream>
#include "model.h"
#include "infer.h"

using namespace std;

void usage() {
  cout << "Usage:\n"
       << "btm est K alpha beta pt_input pt_outdir n_iter\n"
       << "\tK\tint, number of topics, like 20\n"
       << "\talpha\tdouble, Pymmetric Dirichlet prior of P(z), like 1.0\n"
       << "\tbeta\tdouble, Pymmetric Dirichlet prior of P(w|z), like 0.01\n"
       << "\tpt_input\ttraining biterms file\n"
       << "\tpt_outdir\toutput directory\n"
       << "btm inf K pt_input pt_outdir\n"
       << "\tpt_input\tinference biterms file\n"
       << "\tpt_outdir\tparameters/output directory\n";
}

void test_main() {
  int K = 2;
  int save_step = 1000;
  double alpha = 0.1;
  double beta = 0.01;
  string dfile = "./test/doc_wids.txt";
  string dir = "./test/";
  int n_iter = 10;
  Model model(K, alpha, beta, n_iter, save_step, dfile, dir);
  model.run();
  cout << "==== Infence:K=" << K << ", n_iter=" << n_iter << " ====" << endl;
  Infer inf(K, n_iter, dfile, dir);
  inf.run();
  exit(0);
}

int main(int argc, char* argv[]) {
  // test_main();
  //// load parameters from std input
  if (argc < 3) {
    usage();
    return 1;
  }

  if (strcmp(argv[1], "est")==0) {
    int K = atoi(argv[2]);                  // topic num
    double alpha = atof(argv[3]);    // hyperparameters of p(z)
    double beta = atof(argv[4]);     // hyperparameters of p(w|z)
    int n_iter = atoi(argv[5]);
	int save_step = atoi(argv[6]);
    string dfile(argv[7]);
    string dir(argv[8]);

    cout << "==== BTM: Gibbs, K=" << K << ", alpha=" << alpha << ", beta=" << beta 
		 << ", n_iter=" << n_iter << ", save_step=" << save_step << " ====" << endl;
	
    // load training data from file
    Model model(K, alpha, beta, n_iter, save_step, dfile, dir);
    model.run();
  } else if (strcmp(argv[1], "inf")==0) {
    int K = atoi(argv[2]);                  // topic num
	int n_iter = atoi(argv[3]);
    string dfile(argv[4]);
    string dir(argv[5]);
    cout << "==== Infence:K=" << K << ", n_iter=" << n_iter << " ====" << endl;
    Infer inf(K, n_iter, dfile, dir);
    inf.run();
  }
}
