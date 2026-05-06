template<class I>
struct ZZKW {
    int n;
    std::vector<I> info;

    // 只需要开 2n 的空间
    ZZKW(int n_) : n(n_), info(2 * n_) {}

    // 单点修改
    void modify(int p, const I &v) {
        // 1. 直接定位到叶子节点
        p += n;
        info[p] = v;
        // 2. 沿着父节点一路向上更新 (p >>= 1 即 p = p / 2)
        for (p >>= 1; p > 0; p >>= 1) {
            info[p] = info[2 * p] + info[2 * p + 1];
        }
    }

    // 区间查询 [l, r)
    I rangeQuery(int l, int r) {
        I L = I(), R = I();
        // 直接从底层的叶子节点开始，向中间靠拢
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            // 如果 l 是右孩子，说明它的父节点包含了区间外的值，只能单独把 l 加进来，然后 l 走向下一个节点
            if (l & 1) L = L + info[l++];
            // 如果 r 是右孩子，说明 r-1 是左孩子，单独把 r-1 加进来
            if (r & 1) R = info[--r] + R;
        }
        return L + R; // 注意加法顺序：左边的结果 + 右边的结果
    }
};

struct I {
    int x = 0;
};

I operator+(const I& a ,const I&b) {
    return {a.x + b.x};
}