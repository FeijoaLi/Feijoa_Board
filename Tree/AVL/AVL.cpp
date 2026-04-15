constexpr int MAXN = 1e5 + 10;

struct AVL {
    struct Node {
        int val;      // 当前节点值
        int height;   // 当前节点高度
        int size;     // 当前子树大小
        int l, r;     // 左右儿子在数组中的编号
    } tr[MAXN];

    int root = 0, idx = 0;

    // 新建一个节点
    int new_node(int val) {
        tr[++idx] = {val, 1, 1, 0, 0};
        return idx;
    }

    // 获取节点高度
    int height(int p) {
        return p ? tr[p].height : 0;
    }

    // 获取子树大小
    int size(int p) {
        return p ? tr[p].size : 0;
    }

    // 更新节点高度和大小
    void pull(int p) {
        tr[p].height = max(height(tr[p].l), height(tr[p].r)) + 1;
        tr[p].size = size(tr[p].l) + size(tr[p].r) + 1;
    }

    // 平衡因子
    int bf(int p) {
        return height(tr[p].l) - height(tr[p].r);
    }

    // 右旋操作
    int rotate_right(int p) {
        int q = tr[p].l;
        tr[p].l = tr[q].r;
        tr[q].r = p;
        pull(p);
        pull(q);
        return q;
    }

    // 左旋操作
    int rotate_left(int p) {
        int q = tr[p].r;
        tr[p].r = tr[q].l;
        tr[q].l = p;
        pull(p);
        pull(q);
        return q;
    }

    // 平衡处理
    int maintain(int p) {
        if (bf(p) == 2) {
            if (bf(tr[p].l) < 0) {
                tr[p].l = rotate_left(tr[p].l);
            }
            p = rotate_right(p);
        } else if (bf(p) == -2) {
            if (bf(tr[p].r) > 0) {
                tr[p].r = rotate_right(tr[p].r);
            }
            p = rotate_left(p);
        } else {
            pull(p);
        }
        return p;
    }

    // 插入 val
    int insert(int p, int val) {
        if (!p) return new_node(val);
        if (val < tr[p].val) {
            tr[p].l = insert(tr[p].l, val);
        } else {
            tr[p].r = insert(tr[p].r, val);
        }
        return maintain(p);
    }

    void insert(int val) {
        root = insert(root, val);
    }

    // 获取中序最小值
    int get_min(int p) {
        while (tr[p].l) p = tr[p].l;
        return p;
    }

    // 删除 val
    int erase(int p, int val) {
        if (!p) return 0;
        if (val == tr[p].val) {
            if (!tr[p].l || !tr[p].r) {
                return tr[p].l + tr[p].r;
            } else {
                int q = get_min(tr[p].r);
                tr[p].val = tr[q].val;
                tr[p].r = erase(tr[p].r, tr[q].val);
            }
        } else if (val < tr[p].val) {
            tr[p].l = erase(tr[p].l, val);
        } else {
            tr[p].r = erase(tr[p].r, val);
        }
        return maintain(p);
    }

    void erase(int val) {
        root = erase(root, val);
    }

    // 查询比 x 小的元素个数（PBDS: order_of_key）
    int order_of_key(int x) {
        int p = root, ans = 0;
        while (p) {
            if (x <= tr[p].val) {
                p = tr[p].l;
            } else {
                ans += size(tr[p].l) + 1;
                p = tr[p].r;
            }
        }
        return ans;
    }

    // 查询第 k 小（0-based）（PBDS: find_by_order）
    int find_by_order(int k) {
        int p = root;
        while (p) {
            int left = size(tr[p].l);
            if (k < left) {
                p = tr[p].l;
            } else if (k == left) {
                return tr[p].val;
            } else {
                k -= left + 1;
                p = tr[p].r;
            }
        }
        return -1; // 不存在
    }

    // 查询 x 的前驱（小于 x 的最大值）
    int prev(int x) {
        int p = root, ans = -1;
        while (p) {
            if (tr[p].val < x) {
                ans = tr[p].val;
                p = tr[p].r;
            } else {
                p = tr[p].l;
            }
        }
        return ans;
    }

    // 查询 x 的后继（大于 x 的最小值）
    int next(int x) {
        int p = root, ans = -1;
        while (p) {
            if (tr[p].val > x) {
                ans = tr[p].val;
                p = tr[p].l;
            } else {
                p = tr[p].r;
            }
        }
        return ans;
    }
};
/*
AVL 树接口说明：

构造函数：
    AVL(int max_nodes)       // 指定最大节点数，动态分配空间

主要操作：
    void insert(int x)       // 插入值 x（支持重复）
    void erase(int x)        // 删除值 x（存在则删除一个）

查询操作：
    int order_of_key(int x)  // 返回严格小于 x 的元素个数 == rank - 1
    int find_by_order(int k) // 返回第 k 个（0-based）元素的值，越界返回 -1

辅助操作：
    int predecessor(int x)   // 返回严格小于 x 的最大值，若不存在返回 -1
    int successor(int x)     // 返回严格大于 x 的最小值，若不存在返回 -1
*/
