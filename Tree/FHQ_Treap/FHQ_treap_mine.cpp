#include <bits/stdc++.h>

using namespace std;

// 预定义代数算子（Tag）与信息载体（Info）
struct T {
    int x = 0;
    void ap(const T& t) { x += t.x; }
};

struct I {
    int val = 0;
    int size = 0;
    void ap(const T& t) { val += t.x; }
};

template <class I, class T>
struct FHQ {
    std::vector<T> t;
    std::vector<I> i;
    std::vector<int> ls;
    std::vector<int> rs;
    std::vector<uint64_t> pri;

    int rt = 0;
    int cnt = 0;
    std::mt19937_64 rng;

    void init(int n) {
        t.assign(n + 5, T());
        i.assign(n + 5, I());
        ls.assign(n + 5, 0);
        rs.assign(n + 5, 0);
        pri.assign(n + 5, 0);
        rt = 0;
        cnt = 0;
        rng.seed(114514);
    }

    int create(const I& info) {
        cnt++;
        if (cnt >= i.size()) {
            t.push_back(T());
            i.push_back(info);
            ls.push_back(0);
            rs.push_back(0);
            pri.push_back(rng());
        } else {
            i[cnt] = info;
            t[cnt] = T();
            ls[cnt] = rs[cnt] = 0;
            pri[cnt] = rng();
        }
        return cnt;
    }

    void ap(int p, const T& v) {
        if (!p) return;
        i[p].ap(v);
        t[p].ap(v);
    }

    void pu(int p) {
        if (!p) return;
        i[p].size = i[ls[p]].size + i[rs[p]].size + 1;
    }

    void pd(int p) {
        if (!p) return;
        if (ls[p]) ap(ls[p], t[p]);
        if (rs[p]) ap(rs[p], t[p]);
        t[p] = T();
    }

    // 按数值切分：左树 <= val，右树 > val
    void split_value(int cur, int val, int &l, int &r) {
        if (!cur) {
            l = r = 0;
            return;
        }
        pd(cur);
        if (i[cur].val <= val) {
            l = cur;
            split_value(rs[cur], val, rs[cur], r);
            pu(l);
        } else {
            r = cur;
            split_value(ls[cur], val, l, ls[cur]);
            pu(r);
        }
    }

    // 标准有序合并：要求 max(l) <= min(r)
    int merge(int l, int r) {
        if (!l || !r) return l | r;
        if (pri[l] >= pri[r]) {
            pd(l);
            rs[l] = merge(rs[l], r);
            pu(l);
            return l;
        } else {
            pd(r);
            ls[r] = merge(l, ls[r]);
            pu(r);
            return r;
        }
    }

    // 核心拓展：针对本题值域交错重叠的一般性归并
    int merge_it(int l, int r) {
        if (!l || !r) return l | r;
        if (pri[l] < pri[r]) std::swap(l, r);
        pd(l);
        int x = 0, y = 0;
        split_value(r, i[l].val, x, y);
        ls[l] = merge_it(ls[l], x);
        rs[l] = merge_it(rs[l], y);
        pu(l);
        return l;
    }
};

/*
使用说明与核心 API 映射:
FHQ<I, T> treap;
treap.init(n);

int u = treap.create(I{val, 1}); // 创建叶子节点
treap.split_value(treap.rt, k, l, r); // 按值二分剥离
treap.rt = treap.merge(l, r); // 有序拓扑拼接
treap.rt = treap.merge_it(l, r); // 乱序/交错值域强行归并
*/

