#include <bits/stdc++.h>
using namespace std;

// jly 风格的可持久化线段树（主席树）
// 用 vector 模拟动态开点，info 是节点维护的值，这里以 cnt 为例
template <typename Info>
struct PersistentSegTree {
    struct Node {
        int lc = 0, rc = 0; // 左右儿子在 pool 中的下标
        Info info = Info(); // 节点信息，默认构造
    };

    int n; // 值域大小
    vector<Node> pool; // 节点池
    vector<int> roots; // 各个版本的根节点在 pool 中的下标

    PersistentSegTree() : n(0) { pool.emplace_back(); } // 下标0代表空节点
    PersistentSegTree(int n_, int maxNodes) : n(n_), pool(1), roots(maxNodes + 1) {}

    // 新建节点（返回下标）
    int newNode() {
        pool.emplace_back();
        return pool.size() - 1;
    }

    // 合并左右儿子信息到节点u (push up)
    void pull(int u) {
        pool[u].info = pool[pool[u].lc].info + pool[pool[u].rc].info;
    }

    // 在pre版本基础上，在位置x增加delta (Info定义了加法，这样复用性好)
    int add(int pre, int l, int r, int x, const Info &delta) {
        int u = newNode();
        pool[u] = pool[pre];
        if (l == r) {
            pool[u].info = pool[pre].info + delta;
            return u;
        }
        int mid = (l + r) >> 1;
        if (x <= mid) {
            pool[u].lc = add(pool[pre].lc, l, mid, x, delta);
        } else {
            pool[u].rc = add(pool[pre].rc, mid + 1, r, x, delta);
        }
        pull(u);
        return u;
    }

    // 对外接口：在preVersion基础上把位置x的值加上delta
    int addVersion(int preVersion, int x, const Info &delta) {
        int u = add(roots[preVersion], 0, n - 1, x, delta);
        roots.push_back(u); // 将新版本加入根列表，版本号 = roots.size() - 1
        return roots.size() - 1;
    }

    // 区间查询 [ql, qr] 的信息，版本为u
    Info rangeQuery(int u, int l, int r, int ql, int qr) {
        if (!u || ql > r || qr < l) return Info();
        if (ql <= l && r <= qr) return pool[u].info;
        int mid = (l + r) >> 1;
        Info leftInfo = rangeQuery(pool[u].lc, l, mid, ql, qr);
        Info rightInfo = rangeQuery(pool[u].rc, mid + 1, r, ql, qr);
        return leftInfo + rightInfo;
    }

    // 对外接口：查询版本ver在区间[ql, qr]的和 (注意 val 需映射到 [0, n-1])
    Info queryVersion(int ver, int ql, int qr) {
        return rangeQuery(roots[ver], 0, n - 1, ql, qr);
    }

    // 主席树经典：查询版本u和版本v的区间第k小 (设值域为 [l, r])
    int kth(int u, int v, int l, int r, int k) {
        if (l == r) return l;
        int mid = (l + r) >> 1;
        int leftCount = pool[pool[v].lc].info.cnt - pool[pool[u].lc].info.cnt;
        if (k <= leftCount)
            return kth(pool[u].lc, pool[v].lc, l, mid, k);
        else
            return kth(pool[u].rc, pool[v].rc, mid + 1, r, k - leftCount);
    }

    int kthVersion(int verL, int verR, int k) {
        return kth(roots[verL], roots[verR], 0, n - 1, k);
    }
};

// 节点信息：这里是出现次数
struct NodeInfo {
    int cnt = 0;
    NodeInfo(int c = 0) : cnt(c) {}
    // 重载加法实现合并
    NodeInfo operator+(const NodeInfo &o) const { return NodeInfo(cnt + o.cnt); }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    // 离散化
    vector<int> v = a;
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    int m = v.size();
    auto getIdx = [&](int x) { return lower_bound(v.begin(), v.end(), x) - v.begin(); };

    // 初始化主席树，值域 [0, m)，预开节点 n*log(m) 量级，这里简单开 n * 20
    PersistentSegTree<NodeInfo> seg(m, n * 20);
    // 初始版本 (空树)
    seg.roots[0] = 0;
    
    for (int i = 0; i < n; ++i) {
        int idx = getIdx(a[i]);
        seg.roots[i + 1] = seg.add(seg.roots[i], 0, m - 1, idx, NodeInfo(1));
    }

    while (q--) {
        int l, r, k;
        cin >> l >> r >> k;
        int ansIdx = seg.kthVersion(l - 1, r, k);
        cout << v[ansIdx] << '\n';
    }
    return 0;
}