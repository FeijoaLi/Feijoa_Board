#include <iostream>
#include <algorithm>
#include <cassert>

constexpr long long INF = 1'000'000'000'000'000'000;
constexpr long long NEG_INF = -INF;  // 最大值查询用负无穷

struct Line {
    int k;
    long long b;
    Line(int _k = 0, long long _b = NEG_INF) : k(_k), b(_b) {}
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
        bool leftBetter = p->line.value(l) > line.value(l);  // 【改为最大值判定】
        bool midBetter = p->line.value(m) > line.value(m);   // 【改为最大值判定】
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
        if (!p) return NEG_INF;  // 【空节点返回负无穷】
        long long cur = p->line.value(x);
        if (r - l == 1) return cur;
        int m = (l + r) / 2;
        if (x < m) {
            return std::max(cur, query(p->lc, l, m, x));  // 【min改为max】
        } else {
            return std::max(cur, query(p->rc, m, r, x));  // 【min改为max】
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

    void merge(LiChaoTree &other) {
        root = merge(root, other.root, left, right);
        other.root = nullptr;
    }
};

int main() {
    LiChaoTree tree1(-100000, 100001);
    LiChaoTree tree2(-100000, 100001);

    // 插入线段 y=2x+3 和 y=-1x+10 到 tree1
    tree1.insert(2, 3);
    tree1.insert(-1, 10);

    // 插入线段 y=1x+0 和 y=0x+5 到 tree2
    tree2.insert(1, 0);
    tree2.insert(0, 5);

    std::cout << "tree1 max at x=5: " << tree1.query(5) << "\n"; // max(2*5+3=13, -1*5+10=5) = 13
    std::cout << "tree2 max at x=5: " << tree2.query(5) << "\n"; // max(1*5+0=5, 0+5=5) = 5

    // 合并tree2到tree1
    tree1.merge(tree2);

    std::cout << "after merge tree1 max at x=5: " << tree1.query(5) << "\n"; // max(13,5) = 13

    return 0;
}
