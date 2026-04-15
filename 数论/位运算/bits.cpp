int msb_diff(i64 l, i64 r) { return 63 - __builtin_clzll(l ^ r); }
int high_bit(i64 x) { return x ? 63 - __builtin_clzll(x) : -1; }
int low_bit(i64 x) { return x ? __builtin_ctzll(x) : 64; }
int popcount(i64 x) { return __builtin_popcountll(x); }
i64 lowbit_val(i64 x) { return x & -x; }
bool is_pow2(i64 x) { return x > 0 && !(x & (x - 1)); }
i64 ceil_pow2(i64 x) { return x <= 1 ? 1 : 1LL << (64 - __builtin_clzll(x - 1)); }
/*
注：所有“第几位”均指【从右往左数】（0-based），即二进制最低位(LSB)为第 0 位。

msb_diff(l, r);    // 返回 l 和 r 二进制最高位不同的是第几位。
high_bit(x);       // 返回 x 最高位 1 的位置。等价于 floor(log2(x))。x=0 返回 -1。
low_bit(x);        // 返回 x 最低位 1 的位置。x=0 返回 64。
popcount(x);       // 返回二进制中 1 的个数。
lowbit_val(x);     // 返回 x 最低位 1 代表的数值 (即 2^low_bit(x))。
is_pow2(x);        // 判断是否是 2 的幂 (1, 2, 4, 8...)。
ceil_pow2(x);      // 返回 >= x 的最小的 2 的幂。
*/