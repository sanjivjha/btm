**1. High frequency words issue**

As we known, LDA is sensitive to high frequency words as it models word occurrences. Therefore, we often remove stop words/high frequency words before training LDA.

Actually, BTM is more sensitive to high frequency words than LDA, since it models word co-occurrences, which are quadratic of word occurrences. In practice, we address this issue in two ways:

  * Reduce high frequency words
  * Add a weight 1/[n(d,w)-1] for each biterm b in document d, n(d,w) is the number of words in document d.