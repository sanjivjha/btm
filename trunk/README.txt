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


References
[1] 
[2]


TODO:

History：
