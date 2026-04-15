#include <bits/stdc++.h>

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

using u128 = unsigned __int128;
using i128 = __int128;

constexpr i64 inf = 1E18;

constexpr double ALPHA = 0.292;

struct Node {
    union {
        Node *l;
        int val;
    };
    Node *r;
    i128 siz;
        
    void pull() {
        siz = l->siz + r->siz;
    }
};

bool tooHeavy(i128 a, i128 b) {
    return b < ALPHA * (a + b);
}

std::pair<Node *, Node *> cut(Node *x) {
    return {x->l, x->r};
}

Node *join(Node *x, Node *y) {
    Node *t = new Node {.l = x, .r = y};
    t->pull();
    return t;
}

Node *merge(Node *x, Node *y) {
    if (!x) {
        return y;
    }
    if (!y) {
        return x;
    }
    if (tooHeavy(x->siz, y->siz)) {
        auto [a, b] = cut(x);
        if (tooHeavy(b->siz + y->siz, a->siz)) {
            auto [c, d] = cut(b);
            return merge(merge(a, c), merge(d, y));
        } else {
            return merge(a, merge(b, y));
        }
    } else if (tooHeavy(y->siz, x->siz)) {
        auto [a, b] = cut(y);
        if (tooHeavy(x->siz + a->siz, b->siz)) {
            auto [c, d] = cut(a);
            return merge(merge(x, c), merge(d, b));
        } else {
            return merge(merge(x, a), b);
        }
    } else {
        return join(x, y);
    }
}

int query(Node *t, i64 x) {
    if (!t->r) {
        return t->val;
    }
    if (x < t->l->siz) {
        return query(t->l, x);
    } else {
        return query(t->r, x - t->l->siz);
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int Q;
    std::cin >> Q;

    std::vector<Node *> t(Q + 2);
    t[0] = new Node {.val = 0, .siz = 1};
    t[1] = new Node {.val = 1, .siz = 1};

    for (int i = 1; i <= Q; i++) {
        int L, R;
        i64 X;
        std::cin >> L >> R >> X;

        if (t[L]->siz < inf) {
            t[i + 1] = merge(t[L], t[R]);
        } else {
            t[i + 1] = t[L];
        }

        X--;
        std::cout << query(t[i + 1], X) << "\n";
    }

    return 0;
}
