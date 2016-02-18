<font color='red'>
<ul><li>The latest code has been moved into Gibhub, and this site will no longer be updated.<br>
<ul><li>BTM: <a href='https://github.com/xiaohuiyan/BTM'>https://github.com/xiaohuiyan/BTM</a>
</li><li>online BTM: <a href='https://github.com/xiaohuiyan/OnlineBTM'>https://github.com/xiaohuiyan/OnlineBTM</a>
</li><li>bursty BTM: <a href='https://github.com/xiaohuiyan/BurstyBTM'>https://github.com/xiaohuiyan/BurstyBTM</a>
</li></ul></li><li>The papers can be found on my homepage: <a href='http://www.shortext.org'>http://www.shortext.org</a>
</font></li></ul>

Biterm Topic Model (BTM) is a topic model developed for short text
(need to set a window length when generating biterms for nomral text),
like microblogs and webpage titles. It learns topics by modeling the
generation process of word co-occurrences (referred as biterms), rather
than word-document co-occurrences.

### 1. Model Description ###
In BTM, the distribution of a biterm b=(w1,w2) is

> P(w1,w2) = \sum\_k{P(w1|k)P(w2|k)P(k)}.

Steps of Gibbs algorithm for BTM:
  1. Randomly assign topic uniformly to each biterm b
  1. for each biterm b
    1. reset topic assignment of b
    1. sample topic k according to P(k|B-b)
    1. re-assign topic k to biterm b
  1. looper step 2) until converge
  1. inference the parameters {P(k), P(w|k)}

More detail is refered to the following papers:

Xiaohui Yan, Jiafeng Guo, Yanyan Lan, Xueqi Cheng.[A Biterm Topic Model For Short Text](http://shortext.org/paper/BTM-WWW13.pdf). WWW2013.

Xueqi Cheng, Xiaohui Yan, Yanyan Lan, and Jiafeng Guo.[BTM: Topic Modeling over Short Texts](http://shortext.org/paper/BTM-TKDE.pdf). TKDE 2014

### 2. Usage ###
Before running the code, just type "make" in "src/batch" directory
to generate the executable file "btm".



Your can run a toy exapmle by the script:

# cd script

# ./bat.sh

it includes learning, inference, and inspection as follows.

#### 1) Usage for estimation: ####

# ./btm est K W alpha beta n\_iter save\_step pt\_input pt\_outdir

> K	int, number of topics, like 20

> W	int, size of vocabulary

> alpha	double, Symmetric Dirichlet prior of P(z), like 1

> beta	double, Symmetric Dirichlet prior of P(w|z), like 0.01

> n\_iter	int, number of iterations of Gibbs sampling

> save\_step	int, steps to save the results

> pt\_input	string, path of training docs

> pt\_outdir	string, output directory

#### 2) Usage for inference: ####

# ./btm inf type K pt\_input pt\_outdir

> K	int, number of topics, like 20

> type	 string, 4 choices:sum\_w, sum\_b, lda, mix. sum\_b is used in our paper.

> pt\_input	string, path of training docs

> pt\_outdir	string, output directory

#### 3) Result inspection ####

# python script/tran.py

Output the topics with top 10 words of the topics in the example.


### 3. Input & Output ###
1) Input
The input file contains all the training documents. Each line records
a short text doucment, and word indexes (starts from 0) seperated by
space. See the toy example in data/doc\_wids.txt

2) Output
The estimation program will output into the directory "pt\_outdir":
  * pw\_z.k20  a K\*W matrix for P(w|z), if K=20
  * pz.k20   a K\*1 matrix for P(z), if K=20
The inference program will produce:
  * pz\_d.k20 a D\*K matrix for P(z|d), if K=20

### History ###
2013-8-28     Add online BTM.

2013-6-1      Add the process of single word document Inference.

2013-5-6      add a doc\_infer\_sum\_w inference procedure.

2013-5-5      v0.2, add Doc and Dataset class. We change the input from biterms to word sequences. Example is the test/doc\_wids.txt.

2012-09-25    v0.1

Feel free to contact: Xiaohui Yan(xhcloud@gmail.com)