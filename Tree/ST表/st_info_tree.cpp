template <class I>
struct ST {
    int n, lg;
    std::vector<int> d;
    std::vector<std::vector<int>> f; // f[u][j]: 拓扑祖先表
    std::vector<std::vector<I>> i;   // i[u][j]: 路径信息表 (对应 Seg 中的 i 数组)

    void init(const std::vector<int>& p, const std::vector<I>& w) {
        n = p.size() - 1;
        lg = std::__lg(n) + 2;
        d.assign(n + 1, 0);
        f.assign(n + 1, std::vector<int>(lg, 0));
        i.assign(n + 1, std::vector<I>(lg, I()));

        for (int u = 1; u <= n; u++) {
            f[u][0] = p[u];
            i[u][0] = w[u];
        }

        // 记忆化深度计算
        auto get_d = [&](auto &self, int u) -> int {
            if (u == 0) return 0;
            if (d[u] != 0) return d[u];
            return d[u] = self(self, f[u][0]) + 1;
        };
        for (int u = 1; u <= n; u++) get_d(get_d, u);

        // 倍增表阶梯级联递推
        for (int j = 1; j < lg; j++) {
            for (int u = 1; u <= n; u++) {
                f[u][j] = f[f[u][j - 1]][j - 1];
                i[u][j] = i[u][j - 1] + i[f[u][j - 1]][j - 1];
            }
        }
    }

    int lca(int u, int v) const {
        if (d[u] > d[v]) std::swap(u, v);
        for (int j = lg - 1; j >= 0; j--) {
            if ((d[v] - d[u]) >> j & 1) v = f[v][j];
        }
        if (u == v) return u;
        for (int j = lg - 1; j >= 0; j--) {
            if (f[u][j] != f[v][j]) {
                u = f[u][j];
                v = f[v][j];
            }
        }
        return f[u][0];
    }

    int jump(int u, int k) const {
        for (int j = lg - 1; j >= 0; j--) {
            if (k >> j & 1) u = f[u][j];
        }
        return u;
    }

    // 路径信息聚合查询 (Range Query Upwards)
    I rq(int u, int k) const {
        I res = I();
        for (int j = lg - 1; j >= 0; j--) {
            if (k >> j & 1) {
                res = res + i[u][j];
                u = f[u][j];
            }
        }
        return res;
    }
};

struct I {
    int cnt = 0;
    int x = 0;
    int s = 0;
};

I operator+(const I& a, const I& b) {
    return {a.cnt + b.cnt, a.x ^ b.x, a.s + b.s};
}

/*
// 查询树上路径聚合信息 / LCA / 拓扑跳跃
int l = t1.lca(u, v);      // 求 u 和 v 的最低公共祖先 (LCA)
int anc = t1.jump(u, k);   // 从 u 严格向上跳 k 步到达的祖先节点
I info = t1.rq(u, k);      // 从 u 开始向上连续聚合 k 个节点的信息片段

// 结合外部二分：在树上纵向路径中查找第一个/最后一个满足断言 f(I) 的临界位置
// 示例：查找从 u 向上延伸、算术和等于异或和的最长合法区间的顶部节点
int low = 1, high = t1.d[u], id = u;
while (low <= high) {
    int mid = (low + high) / 2;
    I res = t1.rq(u, mid); // 聚合向上数 mid 个节点的路径特征
    if (res.s == res.x) {  // 断言条件满足
        id = t1.jump(u, mid); // 记录当前最远的合法位置
        low = mid + 1;     // 扩大范围，尝试探索更深的祖先
    } else {
        high = mid - 1;    // 发生位冲突，缩小范围
    }
}

// 实例化与初始化
TreeLifting<I> tl;
tl.init(p, w); // p: 1-indexed 父节点数组 (p[1]=0), w: 1-indexed 初始节点信息 I 数组
*/