template<class Info, class Tag>
struct LazySegmentTree {

    int n;
    std::vector<Info> info;
    std::vector<Tag> tag;

    LazySegmentTree() : n(0) {}

    LazySegmentTree(int n_, Info v_ = Info()) {
        init(n_, v_);
    }

    template<class T>
    LazySegmentTree(std::vector<T> init_) {
        init(init_);
    }

    void init(int n_, Info v_ = Info()) {
        init(std::vector(n_, v_));
    }

    template<class T>
    void init(std::vector<T> init_) {
        n = init_.size();
        info.assign(4 << std::__lg(n), Info());
        tag.assign(4 << std::__lg(n), Tag());
        std::function<void(int, int, int)> build = [&](int p, int l, int r) {
            if (r - l == 1) {
                info[p] = init_[l];
                return;
            }
            int m = (l + r) / 2;
            build(2 * p, l, m);
            build(2 * p + 1, m, r);
            pull(p);
        };
        build(1, 0, n);
    }

    void pull(int p) {
        info[p] = info[2 * p] + info[2 * p + 1];
    }

    void apply(int p, const Tag &v) {
        info[p].apply(v);
        tag[p].apply(v);
    }

    void push(int p) {
        apply(2 * p, tag[p]);
        apply(2 * p + 1, tag[p]);
        tag[p] = Tag();
    }

    void modify(int p, int l, int r, int x, const Info &v) {
        if (r - l == 1) {
            info[p] = v;
            return;
        }
        int m = (l + r) / 2;
        push(p);
        if (x < m) {
            modify(2 * p, l, m, x, v);
        } else {
            modify(2 * p + 1, m, r, x, v);
        }
        pull(p);
    }

    void modify(int p, const Info &v) {
        modify(1, 0, n, p, v);
    }

    Info rangeQuery(int p, int l, int r, int x, int y) {
        if (l >= y || r <= x) {
            return Info();
        }
        if (l >= x && r <= y) {
            return info[p];
        }
        int m = (l + r) / 2;
        push(p);
        return rangeQuery(2 * p, l, m, x, y) + rangeQuery(2 * p + 1, m, r, x, y);
    }

    Info rangeQuery(int l, int r) {
        return rangeQuery(1, 0, n, l, r);
    }

    void rangeApply(int p, int l, int r, int x, int y, const Tag &v) {
        if (l >= y || r <= x) {
            return;
        }
        if (l >= x && r <= y) {
            apply(p, v);
            return;
        }
        int m = (l + r) / 2;
        push(p);
        rangeApply(2 * p, l, m, x, y, v);
        rangeApply(2 * p + 1, m, r, x, y, v);
        pull(p);
    }

    void rangeApply(int l, int r, const Tag &v) {
        return rangeApply(1, 0, n, l, r, v);
    }

    void half(int p, int l, int r) {
        if (info[p].act == 0) {
            return;
        }
        if ((info[p].min + 1) / 2 == (info[p].max + 1) / 2) {
            apply(p, {-(info[p].min + 1) / 2});
            return;
        }
        int m = (l + r) / 2;
        push(p);
        half(2 * p, l, m);
        half(2 * p + 1, m, r);
        pull(p);
    }

    void half() {
        half(1, 0, n);
    }

    template<class F>
    int findFirst(int p, int l, int r, int x, int y, F &&pred) {
        if (l >= y || r <= x) {
            return -1;
        }
        if (l >= x && r <= y) {
            if (!pred(info[p], l, r)) {
                return -1;
            }
            if (r - l == 1) {
                return l;
            }
        }

        int m = (l + r) / 2;
        push(p);
        int res = findFirst(2 * p, l, m, x, y, pred);
        if (res == -1) {
            res = findFirst(2 * p + 1, m, r, x, y, pred);
        }
        return res;
    }

    template<class F>
    int findFirst(int l, int r, F &&pred) {
        return findFirst(1, 0, n, l, r, pred);
    }

    template<class F>
    int findLast(int p, int l, int r, int x, int y, F &&pred) {
        if (l >= y || r <= x) {
            return -1;
        }
        if (l >= x && r <= y) {
            if (!pred(info[p], l, r)) {
                return -1;
            }
            if (r - l == 1) {
                return l;
            }
        }

        int m = (l + r) / 2;
        push(p);
        int res = findLast(2 * p + 1, m, r, x, y, pred);
        if (res == -1) {
            res = findLast(2 * p, l, m, x, y, pred);
        }
        return res;
    }

    template<class F>
    int findLast(int l, int r, F &&pred) {
        return findLast(1, 0, n, l, r, pred);
    }

    void maintainL(int p, int l, int r, int pre) {
        if (info[p].difl > 0 && info[p].maxlowl < pre) {
            return;
        }
        if (r - l == 1) {
            info[p].max = info[p].maxlowl;
            info[p].maxl = info[p].maxr = l;
            info[p].maxlowl = info[p].maxlowr = -inf;
            return;
        }
        int m = (l + r) / 2;
        push(p);
        maintainL(2 * p, l, m, pre);
        pre = std::max(pre, info[2 * p].max);
        maintainL(2 * p + 1, m, r, pre);
        pull(p);
    }

    void maintainL() {
        maintainL(1, 0, n, -1);
    }

    void maintainR(int p, int l, int r, int suf) {
        if (info[p].difr > 0 && info[p].maxlowr < suf) {
            return;
        }
        if (r - l == 1) {
            info[p].max = info[p].maxlowl;
            info[p].maxl = info[p].maxr = l;
            info[p].maxlowl = info[p].maxlowr = -inf;
            return;
        }
        int m = (l + r) / 2;
        push(p);
        maintainR(2 * p + 1, m, r, suf);
        suf = std::max(suf, info[2 * p + 1].max);
        maintainR(2 * p, l, m, suf);
        pull(p);
    }

    void maintainR() {
        maintainR(1, 0, n, -1);
    }
};

struct Tag {
    bool act = false;
    int x = 0;

    void apply(const Tag &t) &{
        if (t.act) {
            act = true;
            x += t.x;
        }
    }
};

struct Info {
    bool act = false;
    int x = 0;

    void apply(const Tag &t) &{
        if (t.act) {
            x += t.x;
        }
    }
};

Info operator+(const Info &a, const Info &b) {
    if (!a.act && !b.act) return (Info){};
    if (!a.act) return b;
    if (!b.act) return a;

    Info res = {true};

    return res;
}
/*
// 查找 [l, r) 内第一个/ 最后一个 满足 pred(info) 的位置，不存在返回 -1
seg.findFirst(l, r, [&](const Info &info , int l, int r) { return info.sum >= k; });
seg.findLast(l, r, [&](const Info &info) { return info.x > 0; });
优先递归左子树，左边不行才去右边。
优先递归右子树，右边不行才去左边。
pred -> 先看区间左/右 子树如果满足条件就进入，否则扩大范围向右/左子树移动

ex:
[&](const Info &info, int _l, int _r) { find_first
    int _min = std::min(min , info.min); // 用外部变量维护当前 [l , _r) 最小值
    if (_min - (_r - L - 1) <= 0) { // 左子树满足要求进入
        return true;
    } else { // 扩大范围延伸右子树
        min = _min;
        return false;
    }
}

seg.modify(p, v);           // 单点修改：将位置 p 的信息改为 v (Info)
seg.rangeApply(l, r, v);    // 区间修改：[l, r) 范围应用标记 v (Tag)
seg.rangeQuery(l, r);       // 区间查询：查询 [l, r) 的聚合信息 (Info)

LazySegmentTree<Info, Tag> seg(n);
LazySegmentTree<Info, Tag> seg(_seg);
*/


