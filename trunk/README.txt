BRIFE ILLUSTRATION OF THE IMPLEMENTATIONS OF BTM AND BBTM MODEL
More detail is refered to the paper[1].

Author: Xiaohui Yan(xhcloud@gmail.com)
Date: 2012-09-25
Version: 0.1


1. Biterm Topic Model(BTM)
In BTM, the distribution of a biterm b=(w1,w2) is
   
       P(w1,w2) = \sum_k{P(w1|k)*P(w2|k)*P(k)}.

It can be viewed as a special case of mixture of unigrams with only two words.

Steps of Gibbs algorithm for BTM:
1) Randomly assign topic uniformly to each biterm b
2) for each biterm b
   i) reset topic assignment of b
   ii) sample topic k according to P(k|B-b)
   iii) re-assign topic k to biterm b
3) looper step 2) until converge
4) inference the parameters {P(k), P(w|k)}


2. Run
1) make
2) estimation
   k=20                         # topic number
   alpha=`echo "scale=3;50/$k"|bc`
   beta=0.01
   niter=1000                   # maximum iteration
   savestep=100                 # save parameters every 100 iteration
   doc_pt=                      # training file
   dir=							# output dir
   ./btm est $k ${alpha} ${beta} $niter $savestep $doc_pt $dir
3) inference
   ./src/btm inf $k $doc_infer_pt 
   
3. Input & Output
   Both $doc_pt and $doc_infer_pt have the following format:
   doc_id    wi_id      wj_id     count
   for example:0    0     1     1
   means words 0 and 1 co-occurrs once in document 0
   
References
[1] Xiaohui Yan, Jiafeng Guo, Yanyan Lan, Xueqi Cheng. A Biterm Topic Model For Short Text. Submitted to WWW2013.

TODO:
add a test case

History：
