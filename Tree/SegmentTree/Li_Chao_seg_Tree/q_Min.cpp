#include <iostream>
#include <algorithm>
#include <cassert>

constexpr long long INF = 1'000'000'000'000'000'000;

struct Line {
    int k;
    long long b;
    Line(int _k = 0, long long _b = INF) : k(_k), b(_b) {}
    long long value(int x) const {
        return 1LL * k * x + b;
    }
};

struct Node {
    Line line;
    Node *lc, *rc;
    Node(const Line &line) : line(line), lc(nullptr), rc(nullptr) {}
};

class LiChaoTree {
    Node *root = nullptr;
    int left, right; // 区间[left, right)

    void modify(Node *&p, int l, int r, const Line &line) {
        if (!p) {
            p = new Node(line);
            return;
        }
        int m = (l + r) / 2;
        bool leftBetter = p->line.value(l) < line.value(l);
        bool midBetter = p->line.value(m) < line.value(m);
        if (!midBetter)
            std::swap(p->line, const_cast<Line&>(line));
        if (r - l == 1)
            return;
        if (leftBetter != midBetter) {
            modify(p->lc, l, m, line);
        } else {
            modify(p->rc, m, r, line);
        }
    }

    long long query(Node *p, int l, int r, int x) const {
        if (!p) return INF;
        long long cur = p->line.value(x);
        if (r - l == 1) return cur;
        int m = (l + r) / 2;
        if (x < m) {
            return std::min(cur, query(p->lc, l, m, x));
        } else {
            return std::min(cur, query(p->rc, m, r, x));
        }
    }

    Node* merge(Node *p, Node *q, int l, int r) {
        if (!p) return q;
        if (!q) return p;
        int m = (l + r) / 2;
        p->lc = merge(p->lc, q->lc, l, m);
        p->rc = merge(p->rc, q->rc, m, r);
        modify(p, l, r, q->line);
        return p;
    }

    LiChaoTree(int l, int r) : root(nullptr), left(l), right(r) {}

    void insert(int k, long long b) {
        modify(root, left, right, Line(k, b));
    }

    long long query(int x) const {
        assert(x >= left && x < right);
        return query(root, left, right, x);
    }

    // 把other合并进来，合并后other无效
    void merge(LiChaoTree &other) {
        root = merge(root, other.root, left, right);
        other.root = nullptr; // 避免double free
    }
};

int main() {
    // 区间[-100000, 100001)
    LiChaoTree tree1(-100000, 100001);
    LiChaoTree tree2(-100000, 100001);

    // 向tree1插入 y = 2x + 3
    tree1.insert(2, 3);
    // 向tree1插入 y = -1x + 10
    tree1.insert(-1, 10);

    // 向tree2插入 y = 1x + 0
    tree2.insert(1, 0);
    // 向tree2插入 y = 0x + 5
    tree2.insert(0, 5);

    // 查询tree1在x=5处的最小值
    std::cout << "tree1 query x=5: " << tree1.query(5) << "\n"; // 计算 y = min(2*5+3, -1*5+10) = min(13,5) = 5

    // 查询tree2在x=5处的最小值
    std::cout << "tree2 query x=5: " << tree2.query(5) << "\n"; // min(5+0, 0+5) = 5

    // 合并tree2到tree1
    tree1.merge(tree2);

    // 合并后查询tree1 x=5最小值
    std::cout << "after merge tree1 query x=5: " << tree1.query(5) << "\n";

    // tree2已合并，不能再用
    return 0;
}
