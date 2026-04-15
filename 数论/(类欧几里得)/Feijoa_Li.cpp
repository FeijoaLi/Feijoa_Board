/**
* 类欧几里得算法
 * 计算 \sum_{i=0}^{n-1} floor((a*i + b) / c)
 */
struct Euclidean {
    using i64 = long long;
    using i128 = __int128;

    // 内部递归逻辑：solve(a, b, c, n)
    static i128 solve(i64 a, i64 b, i64 c, i64 n) {
        if (n <= 0) return 0;
        i128 ans = 0;

        // 1. 提取 a / c 和 b / c 的整数部分
        if (a >= c) {
            ans += (i128)n * (n - 1) / 2 * (a / c);
            a %= c;
        }
        if (b >= c) {
            ans += (i128)n * (b / c);
            b %= c;
        }

        // 2. 边界情况
        i64 y_max = (a * n + b) / c;
        if (y_max == 0) return ans;

        // 3. 递归核心：坐标轴翻转变换
        i64 x_max = (y_max * c - b);
        ans += (i128)(n - (x_max + a - 1) / a) * y_max;
        ans += solve(c, (a - x_max % a) % a, a, y_max);

        return ans;
    }

    /**
     * @brief 对外接口 cul(a, b, c, n)
     * @param a 系数
     * @param b 偏移量
     * @param c 分母
     * @param n 迭代范围 [0, n)
     */
    static i64 cul(i64 a, i64 b, i64 c, i64 n) {
        if (n <= 0) return 0;
        i128 offset = 0;

        // 处理负数平移逻辑，保证 a_pos, b_pos >= 0
        i64 a_pos = (a % c + c) % c;
        i64 b_pos = (b % c + c) % c;

        // 补偿由于平移产生的差值（使用 i128 防止 n*(n-1) 溢出）
        offset -= (i128)n * (n - 1) / 2 * ((a_pos - a) / c);
        offset -= (i128)n * ((b_pos - b) / c);

        return (i64)(offset + solve(a_pos, b_pos, c, n));
    }
};

/*

计算 \sum_{i=0}^{n-1} floor((a*i + b) / c) -> { [0,n) :> (ai + b) / m 下取整 }
Euclidean::cul(a, b, c, n)
a 系数 / b 偏移量 / c 分母 / n 迭代范围 [0, n)

*/
