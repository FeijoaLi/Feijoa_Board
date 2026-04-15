struct SAM {
    std::vector<int> len{0},lk{-1},cp{0};
    std::vector<std::map<int, int>> a{{}};
    int cnt = 0;

    int rd(int u, int c, int v, int w) {
        while (u != -1 && a[u][c] == v) {
            a[u][c] = w;
            u = lk[u];
        }
        return u;
    }

    int sp(int u, int c, int v) {
        if (len[u] + 1 == len[v]) return v;

        int w = a.size();
        a.push_back(a[v]);
        len.push_back(len[u] + 1);
        lk.push_back(lk[v]);
        cp.push_back(1);

        lk[v] = w;
        rd(u, c, v, w);
        return w;
    }

    int st(int c, int u) {
        if (a[u][c]) return sp(u, c, a[u][c]);

        int w = a.size();
        a.push_back({});
        len.push_back(len[u] + 1);
        lk.push_back(-1);
        cp.push_back(0);

        u = rd(u, c, 0, w);
        lk[w] = (u == -1) ? 0 : sp(u, c, a[u][c]);

        cnt += len[w] - len[lk[w]];
        return w;
    }
};