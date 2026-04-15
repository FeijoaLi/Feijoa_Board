std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
i64 rnd(i64 l, i64 r) {
    assert(l < r && "❌ 随机数错误: 区间 [l, r) 必须满足 l < r");
    return std::uniform_int_distribution<i64>(l, r - 1)(rng);
}