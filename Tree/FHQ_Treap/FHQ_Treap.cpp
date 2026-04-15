#include "bits/stdc++.h"
using namespace std;

struct FHQ_Treap {
    struct Node {
        int val = 0;            // 节点存储的值
        int size = 0;           // 以该节点为根的子树大小（含重复节点）
        int l = 0, r = 0;       // 左右子节点下标（0 表示空）
        double priority = 0.0;  // 随机优先级（保持堆性质）
        Node() = default;
    };

    int root = 0;       // 根节点下标，0 表示空树
    int cnt = 0;        // 已使用节点数量
    vector<Node> node;  // 节点数组，0号节点为空节点（哨兵）

    // 构造函数，传入预分配大小
    FHQ_Treap(int size) {
        srand(time(0));  // 初始化随机数种子
        node.resize(size);
        root = 0;
        cnt = 0;
        node[0] = Node();  // 0号节点作为空节点
    }

    // 重置整个树，清空所有数据
    void clear() {
        for (int i = 0; i <= cnt; i++) node[i] = Node();
        root = 0;
        cnt = 0;
    }

    // 维护节点大小：等于左子树大小 + 右子树大小 + 自身计数
    void update(int x) {
        node[x].size = node[node[x].l].size + node[node[x].r].size + 1;
        ;
    }

    void split(int cur, int val, int &l, int &r) {  // 按照value分裂

        if (cur == 0) {  // 空树直接返回空
            l = r = 0;
            return;
        }

        // 左 ： 小于等于val的元素
        // 右 ： 大于val的元素
        if (node[cur].val <= val) {  // 当前节点应归到左子树
            l = cur;
            split(node[cur].r, val, node[cur].r, r);
            update(l);
        } else {  // 当前节点应归到右子树
            r = cur;
            split(node[cur].l, val, l, node[cur].l);
            update(r);
        }
    }
    void split1(int cur, int rank, int &l, int &r) {  // 按照rank分裂

        if (cur == 0) {  // 空树直接返回空
            l = r = 0;
            return;
        }

        // 左 ： 小于等于rank的元素
        // 右 ： 大于rank的元素
        if (node[node[cur].l].size + 1 <= rank) {  // 当前节点应归到左子树
            l = cur;
            split(node[cur].r, rank - node[node[cur].l].size - 1, node[cur].r, r);
            update(l);
        } else {  // 当前节点应归到右子树
            r = cur;
            split(node[cur].l, rank, l, node[cur].l);
            update(r);
        }
    }

    /*
     * 将两棵树 l 和 r 合并为一棵树，满足 BST 性质
     * 依据优先级保持堆性质
     */
    int merge(int l, int r) {
        if (l == 0 || r == 0) return l + r;  // 一棵空树，返回另一棵
        if (node[l].priority >= node[r].priority) {
            node[l].r = merge(node[l].r, r);
            update(l);
            return l;
        } else {
            node[r].l = merge(l, node[r].l);
            update(r);
            return r;
        }
    }

    // 查找值为 val 的节点下标，找不到返回0
    int find(int cur, int val) {
        if (cur == 0) return 0;
        if (node[cur].val == val) {
            return cur;
        } else if (node[cur].val > val) {
            return find(node[cur].l, val);
        } else {
            return find(node[cur].r, val);
        }
    }

    // 插入值 val
    void insert(int val) {
        int l, r;
        split(root, val, l, r);  // 分裂成 <= val 和 > val 两棵树

        node[++cnt].val = val;
        node[cnt].size = 1;
        node[cnt].priority = (double)rand() / RAND_MAX;
        node[cnt].l = node[cnt].r = 0;

        root = merge(merge(l, cnt), r);
    }

    // 删除值 val
    void erase(int val) {
        int l, r, lm;
        split(root, val, l, r);    // l包含 <= val, r > val
        split(l, val - 1, l, lm);  // lm只包含 val
        // lm 根节点即为一个 val 节点
        // 删除 lm 根节点，合并左右子树
        lm = merge(node[lm].l, node[lm].r);
        // 合并回去
        root = merge(merge(l, lm), r);
    }

    // 计算子树 cur 中小于 val 的元素个数（含计数）
    int cnt_less_than(int val) {
        int l, r;
        split(root, val - 1, l, r);  // l 是 <= val-1，r 是 >= val
        int res = node[l].size;
        root = merge(l, r);  // 记得恢复原树结构
        return res;
    }

    // 返回 val 的排名（小于 val 的元素个数 + 1）
    int order_of_val(int val) { return cnt_less_than(val) + 1; }

    // 返回排名为 k 的元素（k 从1开始），找不到返回 -1
    int find_by_order(int k) {
        int cur = root;
        while (cur) {
            int l_size = node[node[cur].l].size;  // 左子树大小

            if (k <= l_size) {            // k 在左子树中
                cur = node[cur].l;        // 转向左子树
            } else if (k > l_size + 1) {  // k 在右子树中
                k -= l_size + 1;
                cur = node[cur].r;
            } else {
                return node[cur].val;
            }
        }
        return -1;
    }

    // 找到子树 cur 最大值
    int get_max(int cur) {
        if (cur == 0) return INT_MIN;
        while (node[cur].r) cur = node[cur].r;
        return node[cur].val;
    }

    // 找到子树 cur 最小值
    int get_min(int cur) {
        if (cur == 0) return INT_MAX;
        while (node[cur].l) cur = node[cur].l;
        return node[cur].val;
    }

    // 返回 val 的前驱（小于 val 的最大元素）
    int predecessor(int val) {
        int l, r;
        split(root, val - 1, l, r);  // l ≤ val-1, r ≥ val
        int res = get_max(l);
        root = merge(l, r);
        return res;
    }

    // 返回 val 的后继（大于 val 的最小元素）
    int successor(int val) {
        int l, r;
        split(root, val, l, r);  // l ≤ val, r > val
        int res = get_min(r);
        root = merge(l, r);
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    FHQ_Treap treap(1e5 + 7);
    int n;
    cin >> n;
    while (n--) {
        int op, x;
        cin >> op >> x;
        if (op == 1)
            treap.insert(x);  // 插入 x
        else if (op == 2)
            treap.erase(x);  // 删除 x
        else if (op == 3)
            cout << treap.order_of_val(x) << "\n";  // x 的排名
        else if (op == 4)
            cout << treap.find_by_order(x) << "\n";  // 找排名为 x 的元素
        else if (op == 5)
            cout << treap.predecessor(x) << "\n";  // x 的前驱
        else
            cout << treap.successor(x) << "\n";  // x 的后继
    }
    return 0;
}
