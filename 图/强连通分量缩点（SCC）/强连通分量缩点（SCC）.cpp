struct SCC {
    int n;                              // 图的顶点数
    std::vector<std::vector<int>> adj;  // 邻接表存储图
    std::vector<int> stk;               // 用于DFS的栈
    std::vector<int> dfn;               // 每个节点的DFS序（被访问的时间戳）
    std::vector<int> low;               // 通过回溯能到达的最小DFS序
    std::vector<int> bel;               // 每个节点所属的SCC编号（分量id）
    int cur, cnt;                       // cur：当前时间戳计数，cnt：SCC计数器

    SCC() {}
    SCC(int n) { init(n); }

    void init(int n) {
        this->n = n;
        adj.assign(n, {});
        dfn.assign(n, -1);
        low.resize(n);
        bel.assign(n, -1);
        stk.clear();
        cur = cnt = 0;
    }

    void addEdge(int u, int v) { adj[u].push_back(v); }

    void dfs(int x) {
        dfn[x] = low[x] = cur++;
        stk.push_back(x);

        for (auto y : adj[x]) {
            if (dfn[y] == -1) {
                dfs(y);
                low[x] = std::min(low[x], low[y]);
            } else if (bel[y] == -1) {
                low[x] = std::min(low[x], dfn[y]);
            }
        }

        if (dfn[x] == low[x]) {
            int y;
            do {
                y = stk.back();
                bel[y] = cnt;
                stk.pop_back();
            } while (y != x);
            cnt++;
        }
    }

    std::vector<int> work() {
        for (int i = 0; i < n; i++) {
            if (dfn[i] == -1) {
                dfs(i);
            }
        }
        return bel;
    }
};
