struct LinearBasis {
    constexpr static int BITS = 60; // 适配 long long，如果是 int 范围可改 31
    std::array<int, BITS> basis;

    void init() {
        std::fill(basis.begin(), basis.end(), 0); // 多组数据记得清空
    }

    void insert(int x) {
        for (int i = BITS - 1; i >= 0; i--) {
            if (!(x & (1LL << i))) continue; // 第 i 位为 0，跳过
            if (!basis[i]) { basis[i] = x; return; } // 该位无基底，插入并结束
            x ^= basis[i]; // 该位有基底，进行异或消元
        }
    }

    int query_max() {
        int res = 0;
        for (int i = BITS - 1; i >= 0; i--)
            res = std::max(res, res ^ basis[i]); // 贪心：如果异或后变大就选
        return res;
    }

    int query_min() {
        for (int i = 0; i < BITS; i++)
            if (basis[i]) return basis[i]; // 最低位的基底就是最小非零异或值
        return 0; // 线性基为空
    }

    bool check(int x) {
        for (int i = BITS - 1; i >= 0; i--) {
            if (!(x & (1LL << i))) continue; // 不需要消去该位
            if (!basis[i]) return false; // 需要消但没有对应基底，无法表示
            x ^= basis[i]; // 消元
        }
        return true; // 能被消成 0 说明可以表示
    }
} lb;
/*
lb.init();          // 初始化/清空线性基 (多组数据必调)
lb.insert(x);       // 插入元素 x
lb.query_max();     // 查询能异或出的最大值
lb.query_min();     // 查询能异或出的最小非零值
lb.check(x);        // 检查 x 是否能被线性基表示 (true:能)
*/