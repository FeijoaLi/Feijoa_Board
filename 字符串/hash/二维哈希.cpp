#include "bits/stdc++.h"
/*
关注b站<Feijoa_Li>谢谢喵
UID:248614274
https://space.bilibili.com/248614274
 */
using namespace std;

using u64 = unsigned long long;
using u32 = unsigned;
using u128 = unsigned __int128;
using i128 = __int128;

#define int long long
#define itn long long
#define Ist first
#define Ind second
#define O (int)0
#define endl "\n"
#define PII pair<int, int>
constexpr int inf = 1e18;

int qpow(int a, int b, int m) {
    a %= m;
    int res = 1;
    while (b > 0) {
        if (b & 1) res = res * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return res % m;
}

int MOD = 1e9 + 7;
int P = 131, Q = 13331;
int iP = qpow(P, MOD - 2, MOD);
int iQ = qpow(Q, MOD - 2, MOD);
int N = 1e3 + 7;

vector<int> pw(N, 1), qw(N, 1), ipw(N, 1), iqw(N, 1);
void init() {
    for (int i = 1; i < N; i++) {
        pw[i] = pw[i - 1] * P % MOD;
        qw[i] = qw[i - 1] * Q % MOD;
        ipw[i] = ipw[i - 1] * iP % MOD;
        iqw[i] = iqw[i - 1] * iQ % MOD;
    }
}
vector<vector<int>> buildHash(const vector<string> &s,const vector<int> &pw,const vector<int> &qw) {
    int n = s.size(), m = s[0].size();
    vector<vector<int>> H(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            H[i][j] = (H[i - 1][j] + H[i][j - 1] - H[i - 1][j - 1] + MOD) % MOD;
            int val = s[i - 1][j - 1];
            H[i][j] = (H[i][j] + 1LL * val * pw[i] % MOD * qw[j] % MOD) % MOD;
        }
    }
    return H;
}

int getHash(const vector<vector<int>> &H,const vector<int> &ipw,const vector<int> &iqw ,int x1, int y1, int x2, int y2) {
    int h = (H[x2][y2] - H[x1 - 1][y2] - H[x2][y1 - 1] + H[x1 - 1][y1 - 1]) % MOD;
    h = (h + MOD) % MOD;
    h = h * ipw[x1] % MOD * iqw[y1] % MOD; // 归一化
    return h;
}

void slu() {
    int n, m, a, b;
    cin >> n >> m >> a >> b;
    vector<string> s(n);
    for (int i = 0; i < n; i++) {
        cin >> s[i];
    }

    auto H = buildHash(s, pw, qw);
    map<int, int> mp;

    for (int i = 1; i + a - 1 <= n; i++) {
        for (int j = 1; j + b - 1 <= m; j++) {
            int ir = i + a - 1, jr = j + b - 1;
            int qh = getHash(H, ipw, iqw,i, j, ir, jr);
            mp[qh]++;
        }
    }

    int q;
    cin >> q;
    while (q--) {
        vector<string> t(a);
        for (int i = 0; i < a; i++) {
            cin >> t[i];
        }
        auto T = buildHash(t,pw,qw);
        int th = getHash(T, ipw, iqw,1, 1, a, b);
        if (mp.contains(th)) {
            cout << 1 << endl;
        } else {
            cout << 0 << endl;
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int t = 1;
    // cin >> t;

    init();
    while (t--) slu();
    return 0;
}
