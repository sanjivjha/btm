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

// void test_main() {
//   int K = 20;
//   double alpha = 0;
//   double beta = 0.01;
//   string dfile = "./test/doc_biterms_w30.txt";
//   string dir = "./test/";
//   int n_iter = 10;
//   Model model(K, alpha, beta, dfile, dir, n_iter);
//   model.run();
//   exit(0);
// }

int main(int argc, char* argv[]) {
  //test_main();
  // load parameters from std input
  if (argc < 3) {
    usage();
    return 1;
  }

  if (strcmp(argv[1], "est")==0) {
    int K = atoi(argv[2]);                  // topic num
    double alpha = atof(argv[3]);    // hyperparameters of p(z)
    double beta = atof(argv[4]);     // hyperparameters of p(w|z)
	double lam = atof(argv[5]);
    int n_iter = atoi(argv[6]);
	int save_step = atoi(argv[7]);
    string dfile(argv[8]);
    string dir(argv[9]);

    cout << "==== BTM: Gibbs, K=" << K << ", alpha=" << alpha
		 << ", beta=" << beta << ", lambda=" << lam
		 << ", n_iter=" << n_iter << ", save_step=" << save_step << " ====" << endl;
	
    // load training data from file
    Model model(K, alpha, beta, lam, n_iter, save_step, dfile, dir);
    model.run();
  } else if (strcmp(argv[1], "inf")==0) {
    int K = atoi(argv[2]);                  // topic num
    string dfile(argv[3]);
    string dir(argv[4]);
    cout << "==== Infence:K=" << K << " ====" << endl;
    Infer inf(K, dfile, dir);
    inf.run();
  }
}
