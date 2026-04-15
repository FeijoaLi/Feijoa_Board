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
int inverse(int a, int MOD) { return qpow(a, MOD - 2, MOD); }

constexpr int inf = 1e18;
int P = 131, M = 1e9 + 7;
std::vector<int> pw(2e6, 1);
/*

*/
void slu() {
    std::string s;
    std::cin >> s;
    std::vector<int> hash(s.size() + 1, 0);
    for (int i = 1; i <= s.size(); i++) {
        hash[i] = (hash[i - 1] + pw[i - 1] * s[i - 1]) % M;
    }

    auto cul = [&](int l, int r) {
        int k = (hash[r] - hash[l - 1] + M) % M;
        k = (k * inverse(pw[l - 1], M)) % M;
        return k;
    };

    int q;
    std::cin >> q;
    while (q--) {
        int l1, r1, l2, r2;
        cin >> l1 >> r1 >> l2 >> r2;
        if (cul(l1, r1) == cul(l2, r2)) {
            cout << "Yes\n";
        } else {
            cout << "No\n";
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    for (int i = 1; i < pw.size(); i++) {
        pw[i] = (pw[i - 1] * P) % M;
    }

    int t = 1;
    // cin >> t;

    while (t--) slu();
    return 0;
}
