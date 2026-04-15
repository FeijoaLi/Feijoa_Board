constexpr int inf = 1E9;
template <class T>
struct MaxFlow {
    struct _Edge {
        int to;
        T cap;
        _Edge(int to, T cap) : to(to), cap(cap) {}
    };

    int n;
    std::vector<_Edge> e;
    std::vector<std::vector<int>> g;
    std::vector<int> cur, h;

    MaxFlow() {}
    MaxFlow(int n) { init(n); }

    void init(int n) {
        this->n = n;
        e.clear();
        g.assign(n, {});
        cur.resize(n);
        h.resize(n);
    }

    bool bfs(int s, int t) {
        h.assign(n, -1);
        std::queue<int> que;
        h[s] = 0;
        que.push(s);
        while (!que.empty()) {
            const int u = que.front();
            que.pop();
            for (int i : g[u]) {
                auto [v, c] = e[i];
                if (c > 0 && h[v] == -1) {
                    h[v] = h[u] + 1;
                    if (v == t) {
                        return true;
                    }
                    que.push(v);
                }
            }
        }
        return false;
    }

    T dfs(int u, int t, T f) {
        if (u == t) {
            return f;
        }
        auto r = f;
        for (int &i = cur[u]; i < int(g[u].size()); ++i) {
            const int j = g[u][i];
            auto [v, c] = e[j];
            if (c > 0 && h[v] == h[u] + 1) {
                auto a = dfs(v, t, std::min(r, c));
                e[j].cap -= a;
                e[j ^ 1].cap += a;
                r -= a;
                if (r == 0) {
                    return f;
                }
            }
        }
        return f - r;
    }
    void addEdge(int u, int v, T c) {
        g[u].push_back(e.size());
        e.emplace_back(v, c);
        g[v].push_back(e.size());
        e.emplace_back(u, 0);
    }
    T flow(int s, int t) {
        T ans = 0;
        while (bfs(s, t)) {
            cur.assign(n, 0);
            ans += dfs(s, t, std::numeric_limits<T>::max());
        }
        return ans;
    }

    std::vector<bool> minCut() {
        std::vector<bool> c(n);
        for (int i = 0; i < n; i++) {
            c[i] = (h[i] != -1);
        }
        return c;
    }

    struct Edge {
        int from;
        int to;
        T cap;
        T flow;
    };
    std::vector<Edge> edges() {
        std::vector<Edge> a;
        for (int i = 0; i < e.size(); i += 2) {
            Edge x;
            x.from = e[i + 1].to;
            x.to = e[i].to;
            x.cap = e[i].cap + e[i + 1].cap;
            x.flow = e[i + 1].cap;
            a.push_back(x);
        }
        return a;
    }
};

/*
  Dinic 最大流模板
  使用方法：
    MaxFlow<int> mf(n);          // 创建一个支持 int 容量的最大流对象，点数 n
    mf.addEdge(u, v, cap);       // 添加边
    auto ans = mf.flow(s, t);    // 求解从 s 到 t 的最大流
    auto cut = mf.minCut();      // 得到最小割的可达点集
    auto es = mf.edges();        // 得到所有边的 (from, to, cap, flow)

  -------------------------------
  主要接口：

  1. 初始化
    void init(int n);
    @param n 节点数 (int)
    @effect 初始化一个 n 个节点的空图

  2. 添加边
    void addEdge(int u, int v, T c);
    @param u 起点 (int) / v 终点 (int) / c 容量 (T)
    @effect 添加容量为 c 的有向边 u->v，并自动加反向边 (v->u, cap=0)

  3. 求最大流
    T flow(int s, int t);
    @param s 源点 (int) / t 汇点 (int)
    @return 最大流的流量值 (T)
    @effect 运行 Dinic 算法，求解 s-t 最大流

  4. 最小割
    std::vector<bool> minCut();
    @return bool 数组 (size=n)，c[i]=true 表示节点 i 在 s 可达集合中
    @effect 调用 flow() 后使用，返回最小割对应的点集划分

  5. 边信息
    struct Edge {
      int from;  @field 边的起点 (int)
      int to;    @field 边的终点 (int)
      T cap;     @field 边的总容量 (T)
      T flow;    @field 已经流过的流量 (T)
    };

    std::vector<Edge> edges();
    @return 返回所有原始边（不含反向边）的列表 (vector<Edge>)
    @effect 遍历存图时成对的正/反边，合并为一条包含 from/to/cap/flow 的边
*/




