struct Graph {
    int n;
    std::vector<std::vector<int>> e;
    Graph(int n) : n(n), e(n) {}
    void addEdge(int u, int v) {
        e[u].push_back(v);
        e[v].push_back(u);
    }
    std::vector<int> findMatching(int m, const auto &init) {
        std::vector<int> match(n, -1), vis(n), link(n), f(n), dep(n);
        for (auto [x, y] : init) {
            match[x] = y;
            match[y] = x;
        }
        // disjoint set union
        auto find = [&](int u) {
            while (f[u] != u) u = f[u] = f[f[u]];
            return u;
        };
        auto lca = [&](int u, int v) {
            u = find(u);
            v = find(v);
            while (u != v) {
                if (dep[u] < dep[v]) std::swap(u, v);
                u = find(link[match[u]]);
            }
            return u;
        };
        std::queue<int> que;
        auto blossom = [&](int u, int v, int p) {
            while (find(u) != p) {
                link[u] = v;
                v = match[u];
                if (vis[v] == 0) {
                    vis[v] = 1;
                    que.push(v);
                }
                f[u] = f[v] = p;
                u = link[v];
            }
        };
        // find an augmenting path starting from u and augment (if exist)
        auto augment = [&](int u) {
            while (!que.empty()) que.pop();
            std::iota(f.begin(), f.end(), 0);
            // vis = 0 corresponds to inner vertices, vis = 1 corresponds to outer vertices
            std::fill(vis.begin(), vis.end(), -1);
            que.push(u);
            vis[u] = 1;
            dep[u] = 0;
            int y = -1;
            while (!que.empty()) {
                int u = que.front();
                que.pop();
                if (u >= m) {
                    y = u;
                }
                for (auto v : e[u]) {
                    if (vis[v] == -1) {
                        vis[v] = 0;
                        link[v] = u;
                        dep[v] = dep[u] + 1;
                        // found an augmenting path
                        if (match[v] == -1) {
                            for (int x = v, y = u, temp; y != -1; x = temp, y = x == -1 ? -1 : link[x]) {
                                temp = match[y];
                                match[x] = y;
                                match[y] = x;
                            }
                            return;
                        }
                        vis[match[v]] = 1;
                        dep[match[v]] = dep[u] + 2;
                        que.push(match[v]);
                    } else if (vis[v] == 1 && find(v) != find(u)) {
                        // found a blossom
                        int p = lca(u, v);
                        blossom(u, v, p);
                        blossom(v, u, p);
                    }
                }
            }
            if (y != -1) {
                for (int x = -1, temp; y != -1; x = temp, y = x == -1 ? -1 : link[x]) {
                    temp = match[y];
                    if (x != -1) {
                        match[x] = y;
                    }
                    match[y] = x;
                }
            }
        };
        for (int u = 0; u < m; ++u)
            if (match[u] == -1) augment(u);
        return match;
    }
};
/*
  带花树算法（Edmonds’ Blossom Algorithm）—— 一般图最大匹配模板

  使用方法：
    Graph g(n);                        // 创建一个 n 个点的无向图
    g.addEdge(u, v);                   // 添加一条无向边 (u, v)
    auto match = g.findMatching(n, {}); // 求最大匹配，n 表示起始点范围（一般为 n），初始匹配为空

    // 结果处理
    for (int i = 0; i < n; ++i)
        if (match[i] != -1 && i < match[i])
            std::cout << i << " " << match[i] << "\n";

  --------------------------------
  主要接口：

  1. 初始化
    Graph(int n);
    @param n 节点数 (int)
    @effect 初始化一个 n 个点的无向图，编号为 [0, n)

  2. 添加边
    void addEdge(int u, int v);
    @param u, v 无向边的两个端点 (int)
    @effect 在图中添加边 (u, v)，自动双向存储

  3. 求最大匹配
    std::vector<int> findMatching(int m, const auto &init);
    @param m 起始点范围，一般取 n；若为二分图可设为左部点数
    @param init 初始匹配边集，如 {{u1, v1}, {u2, v2}}，可为空 {}
    @return 长度为 n 的数组 match，match[i] = j 表示 i 与 j 匹配，-1 表示未匹配
    @effect 运行带花树算法，返回一般图的最大匹配结果

  4. 输出匹配边集
    遍历结果时需注意匹配对称性，推荐输出方式：
      for (int i = 0; i < n; ++i)
          if (i < match[i])
              std::cout << i << " " << match[i] << "\n";

  5. 匹配大小
    int matching_size = count_if(match.begin(), match.end(), [&](int v){ return v > -1; }) / 2;

  --------------------------------
  算法说明：
  - 该算法实现 Edmonds 的带花树算法 (Blossom Algorithm)
  - 可处理任意无向图（含奇环），时间复杂度 O(n^3)
  - 自动维护并查集、层次图、花缩合等结构
  - 支持在已有匹配基础上继续扩展（通过 init 参数）
  - 对于二分图匹配，仅需将 m 设置为左侧点数即可

*/
