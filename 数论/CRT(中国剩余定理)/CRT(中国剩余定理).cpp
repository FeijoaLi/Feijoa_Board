#include "bits/stdc++.h"
/*
________  _______       ___         ___      ________      ________
|\  _____\|\  ___ \     |\  \       |\  \    |\   __  \    |\   __  \
\ \  \__/ \ \   __/|    \ \  \      \ \  \   \ \  \|\  \   \ \  \|\  \
 \ \   __\ \ \  \_|/__   \ \  \   __ \ \  \   \ \  \\\  \   \ \   __  \
  \ \  \_|  \ \  \_|\ \   \ \  \ |\  \\_\  \   \ \  \\\  \   \ \  \ \  \
   \ \__\    \ \_______\   \ \__\\ \________\   \ \_______\   \ \__\ \__\
    \|__|     \|_______|    \|__| \|________|    \|_______|    \|__|\|__|
 */
using namespace std;
#define int long long
#define endl "\n"
#define PII pair<int,int>
/*
https://www.luogu.com.cn/problem/P1495
https://www.luogu.com.cn/problem/P4777
https://www.luogu.com.cn/problem/P3868
https://www.luogu.com.cn/problem/P4774
*/
//CRT中国剩余定理
//x ≡ r (mod m)     m互质
struct CRT {
    using i64 = long long;

    std::vector<i64> r, m;

    // 支持初始化大小
    CRT(int n = 0) {
        r.resize(n);
        m.resize(n);
    }

    // 清空数据（用于多组数据）
    void clear() {
        r.clear();
        m.clear();
    }

    // 添加一个同余方程 x ≡ rem (mod mod)
    void add_equation(i64 mod, i64 rem) {
        m.push_back(mod);
        r.push_back(rem);
    }

    // 求 ax + by = gcd(a, b)
    static i64 exgcd(i64 a, i64 b, i64 &x, i64 &y) {
        if (b == 0) {
            x = 1, y = 0;
            return a;
        }
        i64 g = exgcd(b, a % b, y, x);
        y -= a / b * x;
        return g;
    }

    // 求 a 在 mod m 下的逆元，如果不存在返回 -1
    static i64 modInverse(i64 a, i64 mod) {
        i64 x, y;
        i64 g = exgcd(a, mod, x, y);
        if (g != 1) return -1;
        x %= mod;
        if (x < 0) x += mod;
        return x;
    }

    // 龟速乘法,防止溢出
    static i64 fast_mul(i64 a, i64 b, i64 mod) {
        a = (a % mod + mod) % mod;
        b = (b % mod + mod) % mod;
        i64 res = 0;
        while (b) {
            if (b & 1) res = (res + a) % mod;
            a = (a + a) % mod;
            b >>= 1;
        }
        return res;
    }

    // 求解：返回最小非负解 x，若无解返回 -1
    i64 solve() {
        int n = r.size();
        i64 M = 1;
        for (int i = 0; i < n; ++i) M *= m[i];

        i64 res = 0;
        for (int i = 0; i < n; ++i) {
            i64 Mi = M / m[i];
            i64 inv = modInverse(Mi, m[i]);
            if (inv == -1) return -1;
            i64 term = fast_mul(fast_mul(r[i], Mi, M), inv, M);
            res = (res + term) % M;
        }
        return (res + M) % M;
    }
};

//拓展：ki * x ≡ ri (mod mi)   且mi不互质
struct ExtendedCRT {
    using i64 = long long;
    using i128 = __int128;
    vector<i64> k, r, m;

    ExtendedCRT(int n = 0) {
        k.resize(n);
        r.resize(n);
        m.resize(n);
    }
    void clear() {
        k.clear(); r.clear(); m.clear();
    }

    // 添加方程 k*x ≡ rem (mod mod)
    void add_equation(i64 ki, i64 mod, i64 rem) {
        k.push_back(ki);
        m.push_back(mod);
        r.push_back(rem);
    }

    // 扩展欧几里得: ax+by=g
    static i64 exgcd(i64 a, i64 b, i64 &x, i64 &y) {
        if (b == 0) { x = 1; y = 0; return a; }
        i64 g = exgcd(b, a % b, y, x);
        y -= a / b * x;
        return g;
    }

    // 合并两个标准形式方程 x≡r1(mod m1) 和 x≡r2(mod m2)
    static bool merge(i64 r1, i64 m1, i64 r2, i64 m2, i64 &r0, i64 &m0) {
        r1 = (r1 % m1 + m1) % m1;
        r2 = (r2 % m2 + m2) % m2;
        i64 x, y;
        i64 g = exgcd(m1, m2, x, y);
        if ((r2 - r1) % g != 0) return false;
        i128 lcm = (i128)m1 / g * m2;
        i128 diff = (i128)(r2 - r1) / g;
        i128 mul = (diff * (x % (m2/g) + (m2/g)) ) % (m2/g);
        i128 res = (i128)r1 + mul * m1;
        r0 = (i64)((res % lcm + lcm) % lcm);
        m0 = (i64)lcm;
        return true;
    }

    // 求解返回最小非负 x，若无解返回 -1
    i64 solve() {
        int n = k.size();
        if (n == 0) return 0;
        // 先将 k*x≡r (mod m) 化为标准 x≡r' (mod m')
        vector<i64> rr(n), mm(n);
        for (int i = 0; i < n; ++i) {
            i64 ki = k[i], mi = m[i], ri = r[i];
            i64 x0, y0;
            i64 g = exgcd(ki, mi, x0, y0);
            if (ri % g != 0) return -1;
            // 化简
            ki /= g; mi /= g; ri /= g;
            // 求逆
            i64 inv = (x0 % mi + mi) % mi;
            // 标准余数
            rr[i] = (i128)inv * ri % mi;
            mm[i] = mi;
        }
        // 迭代合并
        i64 r0 = rr[0], m0 = mm[0];
        for (int i = 1; i < n; ++i) {
            i64 r1, m1;
            if (!merge(r0, m0, rr[i], mm[i], r1, m1)) return -1;
            r0 = r1; m0 = m1;
        }
        return r0;
    }
};

void slu() {
    int n;
    std::cin >> n;
    CRT crt(n); // 预设大小
    for (int i = 0; i < n; ++i) {
        std::cin >> crt.m[i] >> crt.r[i];
    }

    long long ans = crt.solve();
    if (ans == -1) std::cout << "No solution\n";
    else std::cout << ans << "\n";

    crt.clear(); // 如果有多组数据，可以复用
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int T = 1;
    // cin >> T;
    while (T--)slu();
    return 0;
}
