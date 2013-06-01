BRIFE ILLUSTRATION OF THE IMPLEMENTATIONS OF BTM AND BBTM MODEL
More detail is refered to the following paper:
[1] Xiaohui Yan, Jiafeng Guo, Yanyan Lan, Xueqi Cheng. A Biterm Topic Model For Short Text. WWW2013.

Author: Xiaohui Yan(xhcloud@gmail.com)
Date: 2012-09-25
Version: 0.2


1. Biterm Topic Model(BTM)
In BTM, the distribution of a biterm b=(w1,w2) is
   
       P(w1,w2) = \sum_k{P(w1|k)*P(w2|k)*P(k)}.

Steps of Gibbs algorithm for BTM:
1) Randomly assign topic uniformly to each biterm b
2) for each biterm b
   i) reset topic assignment of b
   ii) sample topic k according to P(k|B-b)
   iii) re-assign topic k to biterm b
3) looper step 2) until converge
4) inference the parameters {P(k), P(w|k)}


[History]
2013-6-1      Add the process of single word document Inference.

2013-5-6      add a doc_infer_sum_w inference procedure.

2013-5-5      v0.2, add Doc and Dataset class. We change the input
			  from biterms to word sequences. Example is the test/doc_wids.txt.

2012-09-25    v0.1
