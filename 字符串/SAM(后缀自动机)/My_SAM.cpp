struct SAM {
    std::vector<int> l{0},lk{-1},cp{0};
    std::vector<std::array<int, 26>> a{{}};
    int cnt = 0;

    // 重定向函数 (u, c, v, w)
    // 语义：顺着 u 往上，把原本 u --c--> v 的边，重定向为 u --c--> w
    int rd(int u, int c, int v, int w) {
        while (u != -1 && a[u][c] == v) {
            a[u][c] = w;
            u = lk[u];
        }
        return u;
    }

    // 分裂函数 (u, c, v)
    // 语义：分裂 u --c--> v 这条边，返回纯净的新终点 w
    int sp(int u, int c, int v) {
        if (l[u] + 1 == l[v]) return v;

        int w = l.size();
        a.push_back(a[v]);
        lk.push_back(lk[v]);
        l.push_back(l[u] + 1);
        cp.push_back(1);


        lk[v] = w;
        // 调用极其自然：把指向 v 的 c 边，全改成 w
        rd(u, c, v, w);
        return w;
    }

    int st(int u,int c) {
        // 1. 若旧边已存在，直接交由 sp 处理 (u --c--> nxt[c])
        if (a[u][c]) return sp(u, c, a[u][c]);

        int w = l.size();
        a.push_back({});
        lk.push_back(-1);
        l.push_back(l[u] + 1);
        cp.push_back(0);

        // 2. 修新路：顺着 u 往上，把原本死胡同的边 (u --c--> 0)，改成通往 w
        // 【核心亮点】：直接复用参数 u 作为往上跳的游标，连变量声明都省了！
        u = rd(u, c, 0, w);

        // 3. 找归宿
        lk[w] = (u == -1) ? 0 : sp(u, c, a[u][c]);
        cnt += l[w] - l[lk[w]];
        return w;
    }
};