#include "bits/stdc++.h"
using namespace std;

struct FHQTreap {
    // 无旋Treap：满足BST性质(val)和堆性质(priority)
    struct Node {
        int val;
        int size = 0;
        int priority;  // 随机优先级
        Node *left, *right;
        Node(int val) : val(val), priority(rand()), left(NULL), right(NULL), size(1) {}
        Node(int val, int priority) : val(val), priority(priority), left(NULL), right(NULL), size(1) {}
    };

    bool cmp(int a, int b) { return a <= b; }  // 自定义比较函数，满足BST性质
    Node *root;

    FHQTreap() : root(NULL) { srand((unsigned)time(nullptr)); }

    // 更新节点size
    void update(Node *root) {
        if (!root) return;
        root->size = 1;
        if (root->left) root->size += root->left->size;
        if (root->right) root->size += root->right->size;
    }

    // 合并两棵树a,b为root，满足BST和堆性质
    void merge(Node *&root, Node *a, Node *b) {
        if (!a || !b) {  // 存在空树，直接返回
            root = a ? a : b;
        } else {
            if (a->priority > b->priority) {
                root = a;
                merge(a->right, a->right, b);
            } else {
                root = b;
                merge(b->left, a, b->left);
            }
        }

        update(root);
    }

    // 按val拆分root为a,b，a中所有val <= val，b中所有val > val
    void split(Node *root, Node *&a, Node *&b, int val) {
        if (!root) {
            a = b = NULL;
            return;
        }

        if (cmp(root->val, val)) {
            a = root;
            split(root->right, a->right, b, val);
        } else {
            b = root;
            split(root->left, a, b->left, val);
        }

        update(root);
    }

    // 插入val
    void insert(int val) {
        Node *a, *b;
        split(root, a, b, val);
        merge(a, a, new Node(val));
        merge(root, a, b);
    }

    // 删除val（只删除一个）
    void erase(int val) {
        Node *a, *b, *c;
        split(root, a, b, val);
        split(a, a, c, val - 1);
        if (c) {
            // 删除c根节点，合并其左右子树
            merge(a, a, c->left);
            merge(a, a, c->right);
            delete c;  // 防止内存泄漏
        }
        merge(root, a, b);
    }

    // 返回val的排名（比val小的元素个数+1）
    int order_of_key(int val) {
        Node *a, *b;
        split(root, a, b, val - 1);
        int res = (a ? a->size : 0) + 1;
        merge(root, a, b);
        return res;
    }

    // 查询第k小元素，k从1开始，越界返回-1
    int find_by_order(int k) { return kth_query(root, k); }

    int kth_query(Node *root, int k) {
        if (!root) return -1;
        int leftsize = root->left ? root->left->size : 0;
        if (k <= leftsize) return kth_query(root->left, k);  // 左子树中第k小
        if (k == leftsize + 1) return root->val;             // 找到第k小元素
        return kth_query(root->right, k - leftsize - 1);     // 右子树中第k-leftsize-1小
    }

    // 查找val是否存在
    bool find(int val) {
        Node *a, *b;
        split(root, a, b, val);
        bool res = (a && find_max(a) == val);
        merge(root, a, b);
        return res;
    }

    // val的前驱（小于val的最大元素），不存在返回-1
    int predecessor(int val) {
        Node *a, *b;
        split(root, a, b, val - 1);
        int res = find_max(a);
        merge(root, a, b);
        return res;
    }

    // val的后继（大于val的最小元素），不存在返回-1
    int successor(int val) {
        Node *a, *b;
        split(root, a, b, val);
        int res = find_min(b);
        merge(root, a, b);
        return res;
    }

    // 中序打印
    void print(Node *root) {
        if (!root) return;
        print(root->left);
        cout << root->val << " ";
        print(root->right);
    }

    int find_max(Node *root) {
        if (!root) return -1;
        while (root->right) root = root->right;
        return root->val;
    }

    int find_min(Node *root) {
        if (!root) return -1;
        while (root->left) root = root->left;
        return root->val;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    FHQTreap treap;
    int n;
    cin >> n;
    while (n--) {
        int op, x;
        cin >> op >> x;
        if (op == 1)
            treap.insert(x);
        else if (op == 2)
            treap.erase(x);
        else if (op == 3)
            cout << treap.order_of_key(x) << "\n";
        else if (op == 4)
            cout << treap.find_by_order(x) << "\n";
        else if (op == 5)
            cout << treap.predecessor(x) << "\n";
        else if (op == 6)
            cout << treap.successor(x) << "\n";
    }

    return 0;
}
