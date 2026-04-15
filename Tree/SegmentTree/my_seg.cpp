template <class I, class T>
struct Seg {
    int n;
    std::vector<T> t;
    std::vector<I> i;

    void ap(int p, const T& v) {
        i[p].ap(v);
        t[p].ap(v);
    }
    void pu(int p) { i[p] = i[p << 1] + i[p << 1 | 1]; }
    void pd(int p) {
        ap(p << 1, t[p]);
        ap(p << 1 | 1, t[p]);
        t[p] = T();
    }

    void st(int p, int l, int r, int x, const I& v) {
        if (r - l == 1) {
            i[p] = v;
            return;
        }
        int m = l + r >> 1;
        pd(p);
        if (x < m) {
            st(p >> 1, l, m, x, v);
        } else {
            st(p >> 1 | 1, m, r, x, v);
        }
        pu(p);
    }

    I rq(int p, int l, int r, int x, int y) {
        if (l >= y || r <= x) return I();
        if (l >= x && r <= y) return i[p];

        int m = l + r >> 1;
        pd(p);
        return rq(p << 1, l, m, x, y) + rq(p << 1 | 1, m, r, x, y);
    }

    void ra(int p, int l, int r, int x, int y, const T& v) {
        if (l >= y || r <= x) return;
        if (l >= x && r <= y) return ap(p, v);

        int m = l + r >> 1;
        pd(p);
        ra(p << 1, l, m, x, y, v);
        ra(p << 1 | 1, m, r, x, y, v);
        pu(p);
    }

    template <class F>
    int ff(int p, int l, int r, int x, int y, F&& f) {
        if (l >= y || r <= x) return -1;
        if (l >= x && r <= y) {
            if (!f(i[p], l, r)) return -1;
            if (r - l == 1) return l;
        }

        int m = l + r >> 1;
        pd(p);
        int res = ff(p << 1, l, m, x, y, f);
        if (res == -1) {
            res = ff(p << 1 | 1, m, r, x, y, f);
        }
        return res;
    }

    template <class F>
    int fl(int p, int l, int r, int x, int y, F&& f) {
        if (l >= y || r <= x) return -1;
        if (l >= x && r <= y) {
            if (!f(i[p], l, r)) return -1;
            if (r - l == 1) return l;
        }

        int m = l + r >> 1;
        pd(p);
        int res = ff(p << 1 | 1, m, r, x, y, f);
        if (res == -1) {
            res = ff(p << 1, l, m, x, y, f);
        }
        return res;
    }
};

struct T {
    int cnt = 0;
    void ap(const T& t) { cnt += t.cnt; }
};
struct I {
    int op = 0;
    int cl = 0;

    void ap(const T& t) {
        if (t.cnt & 1) {
            std::swap(op, cl);
        }
    }
};

I operator+(const I& a, const I& b) {
    I res = {a.op + b.op, a.cl + b.cl};
    return res;
}

/*
// 查找 [l, r) 内第一个 / 最后一个 满足 f(I) 的位置，不存在返回 -1
seg.ff(1, 0, n, l, r, [&](I info, int l, int r) { return info.s >= k; }); // first: 优先递归左子树，左边不行才去右边。
seg.fl(1, 0, n, l, r, [&](I info, int l, int r) { return info.s > 0; }); // last: 优先递归右子树，右边不行才去左边。
// pred f: 先看区间左/右子树如果满足条件就进入，否则扩大范围向右/左子树移动。

int s = 0;
int x = 0;
auto pred = [&](const I &i, int l, int r) {
    if (s + i.cnt >= x) {
        return true;
    } else {
        s += i.cnt;
        return false;
    }
};
int id = seg.ff(1, 0, n, 0, n, pred);
seg.qry(1, 0, n, 0, id + 1).sum;

seg.st(1, 0, n, x, v);
seg.rq(1, 0, n, x, y);
seg.ra(1, 0, n, x, y, v);

Seg<I, T> seg;
seg.n = n;
seg.i.assign(n << 2, I());
seg.t.assign(n << 2, T());
*/