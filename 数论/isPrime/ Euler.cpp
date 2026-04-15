//筛选1-n
const int N = 5e6 + 5;//定义筛法计算的上限（1,000,005）
std::vector<int> pri;//存储素数的向量
bool not_prime[N];//标记非素数的布尔数组
int phi[N];//存储欧拉函数值的数组--->欧拉函数φ(n)表示小于等于n的正整数中与n互质的数的个数。
void pre(int n) {
    phi[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (!not_prime[i]) {
            pri.push_back(i);
            phi[i] = i - 1;
        }
        for (int pri_j : pri) {
            if (i * pri_j > n) break;
            not_prime[i * pri_j] = true;
            if (i % pri_j == 0) {
                phi[i * pri_j] = phi[i] * pri_j;
                break;
            }
            phi[i * pri_j] = phi[i] * phi[pri_j];
        }
    }
}

std::vector<int> minp, primes;
//primes: 保存所有 ≤ n 的质数。
// minp [x]: 记录数 x 的最小质因子(minimum prime factor)。
void sieve(int n) {
    minp.assign(n + 1, 0);
    primes.clear();

    for (int i = 2; i <= n; i++) {
        if (minp[i] == 0) {
            minp[i] = i;
            primes.push_back(i);
        }

        for (auto p : primes) {
            if (i * p > n) {
                break;
            }
            minp[i * p] = p;
            if (p == minp[i]) {
                break;
            }
        }
    }
}