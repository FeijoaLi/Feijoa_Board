typedef unsigned long long ull;
struct HashPair {
    ull h1, h2;
    // 重载 == 运算符
    bool operator==(const HashPair& other) const { return h1 == other.h1 && h2 == other.h2; }
    // 重载 != 运算符
    bool operator!=(const HashPair& other) const { return !(*this == other); }
};

struct RollingHash {
    static const ull MOD2 = 1e9 + 7;
    static ull BASE1, BASE2;
    static bool initialized;

    std::vector<ull> h1, p1, h2, p2;

    static void init_random_base() {
        if (initialized) return;
        std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        BASE1 = (rng() % 5000) + 13331;
        BASE2 = (rng() % 5000) + 13331;
        initialized = true;
    }

    template <typename T>
    RollingHash(const std::vector<T>& arr) {
        init_random_base();
        int n = arr.size();  // 原数组长度

        h1.resize(n + 1);
        p1.resize(n + 1);
        h2.resize(n + 1);
        p2.resize(n + 1);
        p1[0] = 1;
        h1[0] = 0;
        p2[0] = 1;
        h2[0] = 0;

        for (int i = 0; i < n; i++) {
            // 使用 i % len 模拟循环
            ull val = (ull)arr[i]+ 1;

            p1[i + 1] = p1[i] * BASE1;
            h1[i + 1] = h1[i] * BASE1 + val;

            p2[i + 1] = (p2[i] * BASE2) % MOD2;
            h2[i + 1] = (h2[i] * BASE2 + val) % MOD2;
        }
    }

    // 修改为左闭右开区间 [l, r)
    HashPair query(int l, int r) {
        // h[r] 恰好代表前 r 个元素 (下标 0 到 r-1) 的哈希
        // h[l] 恰好代表前 l 个元素 (下标 0 到 l-1) 的哈希
        // 这里的下标直接对应哈希数组的索引，非常直观
        ull res1 = h1[r] - h1[l] * p1[r - l];
        ull res2 = (h2[r] - h2[l] * p2[r - l] % MOD2 + MOD2) % MOD2;

        return {res1, res2};
    }
};

// 静态成员初始化
ull RollingHash::BASE1 = 131;
ull RollingHash::BASE2 = 13331;
bool RollingHash::initialized = false;

/*
DOCUMENTATION:
RollingHash hasher(a);
HashPair query(l,r)
区间定义：左闭右开 [l, r)，0-based。内部进行了双倍扩容，l 和 r 可以超过原数组长度 len。
子段对应的 HashPair 对象，支持 == 和 != 比较。
*/