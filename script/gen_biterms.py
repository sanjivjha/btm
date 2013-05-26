#!/usr/bin/env python
#coding=utf-8
# extract biterms from docs
# input:  doc_wids.txt, each line is a short text document
#    w1  w2  w3 ...
# output: doc_biterms.txt
#    did   wi   wj    freq
from collections import defaultdict

# input [w1, w2, ...]
# return [('w1\tw2', dist(w1,w2)), ...]
def extract_doc(ws, win_len=0):
    assert(len(ws)>0)
    if len(ws)==1: ws.append(ws[0])
    if win_len==0: win_len = len(ws)

    biterms = defaultdict(int)
    for i in range(len(ws) - 1):
        end = min(len(ws), i+win_len)
        for j in range(i+1, end):
            w1 = min(ws[i], ws[j])
            w2 = max(ws[i], ws[j])
            biterm = '%s\t%s' % (w1, w2)
            biterms[biterm] += 1
    return biterms

# return {did:[zid, tag, v], ...}
def proc_docs(pt, res_pt, win_len):
    rf = open(pt)
    wf = open(res_pt, 'w')
    
    did = 0
    for l in rf:
        ws = [int(w) for w in l.strip().split()]
        biterms = extract_doc(ws, win_len)
        assert(len(biterms)>0)
        # write
        for b, freq in biterms.iteritems():
            wf.write('%d\t%s\t%d\n' % (did, b, freq))
            
        did += 1

if __name__ == '__main__':
    data = 'baiduQA'
    data_dir = '/home/xh/mp/sparseNMF/data/%s/mid/' % data
    
    doc_info_pt = data_dir + 'doc_wids.txt'

    win_len = 0                # context windows, 0 to all
    if win_len == 0:
        biterm_pt = data_dir + 'doc_biterms_all.txt'
    else:
        biterm_pt = data_dir + 'doc_biterms_w%d.txt' % win_len
        
    print "data:%s, win_len:%d" % (data, win_len)
    proc_docs(doc_info_pt, biterm_pt, win_len)
    print 'write:', biterm_pt
