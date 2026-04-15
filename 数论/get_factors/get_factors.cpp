std::vector<std::pair<i64, int>> get_factors(i64 n) {
    std::vector<std::pair<i64, int>> res;
    if (n % 2 == 0) {
        int cnt = 0;
        while (n % 2 == 0) {
            n /= 2;
            cnt++;
        }
        res.emplace_back(2, cnt);
    }
    for (i64 i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                cnt++;
            }
            res.emplace_back(i, cnt);
        }
    }
    if (n > 1) res.emplace_back(n, 1);

    return res;
}// 返回值：vector<{质因子, 该因子的次数}>