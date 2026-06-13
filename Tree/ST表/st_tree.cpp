struct ST {
    int n, lg;
    std::vector<int> d;
    std::vector<std::vector<int>> f;  // f[u][j]: 拓扑祖先矩阵

    void init(const std::vector<int>& p) {
        n = p.size() - 1;
        lg = std::__lg(n) + 2;
        d.assign(n + 1, 0);
        f.assign(n + 1, std::vector<int>(lg, 0));

        for (int u = 1; u <= n; u++) {
            f[u][0] = p[u];
        }

        // 记忆化深度递推模型
        auto get_d = [&](auto& self, int u) -> int {
            if (u == 0) return 0;
            if (d[u] != 0) return d[u];
            return d[u] = self(self, f[u][0]) + 1;
        };
        for (int u = 1; u <= n; u++) get_d(get_d, u);

        // 二进制空间阶梯递推
        for (int j = 1; j < lg; j++) {
            for (int u = 1; u <= n; u++) {
                f[u][j] = f[f[u][j - 1]][j - 1];
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
};