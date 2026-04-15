#include "bits/stdc++.h"
/*
关注b站<Feijoa_Li>谢谢喵
UID:248614274
https://space.bilibili.com/248614274
 */
using namespace std;
using u64 = unsigned long long;
using u32 = unsigned;
using u128 = unsigned __int128;
using i128 = __int128;

#define int long long
#define Ist first
#define Ind second
#define itn long long
#define O (int)0
#define endl "\n"
#define PII pair<int,int>
const int inf = 1e18;


struct AVL {
    struct Node {
        int val;    // 节点值
        int siz;    // 子树大小
        int height; // 高度
        int l, r;   // 左右子节点在数组中的下标
    };

    vector<Node> tr;
    int root = 0, tot = 0;

    AVL(int max_nodes) { // 构造函数，动态设定空间大小
        tr.resize(max_nodes + 1); // 节点编号从 1 开始
    }

    // 创建新节点
    int new_node(int val) {
        tr[++tot] = {val, 1, 1, 0, 0};
        return tot;
    }

    // 获取节点高度
    int height(int p) {
        return p ? tr[p].height : 0;
    }

    // 获取子树大小
    int size(int p) {
        return p ? tr[p].siz : 0;
    }

    // 更新节点信息（大小、高度）
    void push_up(int p) {
        tr[p].siz = size(tr[p].l) + size(tr[p].r) + 1;
        tr[p].height = max(height(tr[p].l), height(tr[p].r)) + 1;
    }

    // 左旋操作
    void rotate_left(int &p) {
        int q = tr[p].r;
        tr[p].r = tr[q].l;
        tr[q].l = p;
        push_up(p);
        push_up(q);
        p = q;
    }

    // 右旋操作
    void rotate_right(int &p) {
        int q = tr[p].l;
        tr[p].l = tr[q].r;
        tr[q].r = p;
        push_up(p);
        push_up(q);
        p = q;
    }

    // 平衡操作
    void balance(int &p) {
        if (!p) return;
        int balance_factor = height(tr[p].l) - height(tr[p].r);
        if (balance_factor == 2) {
            if (height(tr[tr[p].l].l) < height(tr[tr[p].l].r)) {
                rotate_left(tr[p].l);
            }
            rotate_right(p);
        } else if (balance_factor == -2) {
            if (height(tr[tr[p].r].r) < height(tr[tr[p].r].l)) {
                rotate_right(tr[p].r);
            }
            rotate_left(p);
        } else {
            push_up(p);
        }
    }

    // 插入值 x 到以 p 为根的子树中
    void insert(int &p, int x) {
        if (!p) {
            p = new_node(x);
            return;
        }
        if (x < tr[p].val) insert(tr[p].l, x);
        else insert(tr[p].r, x);
        balance(p);
    }

    void insert(int x) {
        insert(root, x);
    }

    // 删除值 x 从以 p 为根的子树中
    void erase(int &p, int x) {
        if (!p) return;
        if (x < tr[p].val) {
            erase(tr[p].l, x);
        } else if (x > tr[p].val) {
            erase(tr[p].r, x);
        } else {
            if (!tr[p].l || !tr[p].r) {
                p = tr[p].l + tr[p].r;
            } else {
                int q = tr[p].r;
                while (tr[q].l) q = tr[q].l;
                tr[p].val = tr[q].val;
                erase(tr[p].r, tr[q].val);
            }
        }
        if (p) balance(p);
    }

    void erase(int x) {
        erase(root, x);
    }

    // order_of_key(x): 查询严格小于 x 的个数
    int order_of_key(int p, int x) {
        if (!p) return 0;
        if (x <= tr[p].val) {
            return order_of_key(tr[p].l, x);
        } else {
            return size(tr[p].l) + 1 + order_of_key(tr[p].r, x);
        }
    }

    int order_of_key(int x) {
        return order_of_key(root, x);
    }

    // find_by_order(k): 查询第 k 个（0-based）数
    int find_by_order(int p, int k) {
        if (!p) return -1;
        if (k < size(tr[p].l)) {
            return find_by_order(tr[p].l, k);
        } else if (k == size(tr[p].l)) {
            return tr[p].val;
        } else {
            return find_by_order(tr[p].r, k - size(tr[p].l) - 1);
        }
    }

    int find_by_order(int k) {
        return find_by_order(root, k);
    }

    // 查询前驱：小于 x 的最大值
    int predecessor(int p, int x, int res = -1) {
        if (!p) return res;
        if (tr[p].val < x) {
            return predecessor(tr[p].r, x, tr[p].val);
        } else {
            return predecessor(tr[p].l, x, res);
        }
    }

    int predecessor(int x) {
        return predecessor(root, x);
    }

    // 查询后继：大于 x 的最小值
    int successor(int p, int x, int res = -1) {
        if (!p) return res;
        if (tr[p].val > x) {
            return successor(tr[p].l, x, tr[p].val);
        } else {
            return successor(tr[p].r, x, res);
        }
    }

    int successor(int x) {
        return successor(root, x);
    }
};

void slu() {
    AVL avl(1e5+1);
    int q; cin >> q;
    while (q--) {
        int op, x;
        cin >> op >> x;
        if (op == 1)      avl.insert(x);
        else if (op == 2) avl.erase(x);
        else if (op == 3) cout << avl.order_of_key(x)+1  << "\n";
        else if (op == 4) cout << avl.find_by_order(x-1) << "\n";
        else if (op == 5) cout << avl.predecessor(x)   << "\n";
        else              cout << avl.successor(x)     << "\n";
    }
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    slu();
    return 0;
}