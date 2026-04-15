/*
      State Vector Transfer: M * [a_{i-1}, a_{i-2}, ..., a_{i-n}, i^2, i, 1]^T
a_{k} = res.mat[n - 1][0] -> M^k * [a_{n-1}, a_{n-2}, ..., a_{0}  , n^2, n, 1]^T
   Target  | a_{i-1} | a_{i-2} | ... | a_{i-n} |  i^2  |  i  |  1  |  Notes
   --------|---------|---------|-----|---------|-------|-----|-----|---------------------------
   a_i     |   c1    |   c2    | ... |   cn    |   r   |  q  |  p  | a_i = Σ(ci*a_{i-j}) + r*i^2 + q*i + p
   a_{i-1} |   1     |   0     | ... |   0     |   0   |  0  |  0  | shift
   a_{i-2} |   0     |   1     | ... |   0     |   0   |  0  |  0  | shift
   ...     |   ...   |   ...   | ... |   ...   |  ...  | ... | ... | ...
   (i+1)^2 |   0     |   0     | ... |   0     |   1   |  2  |  1  | (i+1)^2 = 1 * i^2 + 2 * i + 1
   (i+1)   |   0     |   0     | ... |   0     |   0   |  1  |  1  | (i+1)   = 0 * i^2 + 1 * i + 1
   1       |   0     |   0     | ... |   0     |   0   |  0  |  1  | 1       = 0 * i^2 + 0 * i + 1
*/
template <typename T>
struct Matrix {
    // 定义最半环(+ , *)
    // 标准矩阵乘法：C[i][j] = Σ (A[i][k] * B[k][j])
    struct Semiring {
        // Additive Identity: 加法的单位元 (e.g., 0 for +, -inf for max, 1 for *)
        static T add_id() { return 0; }
        // Multiplicative Identity: 乘法的单位元
        static T mul_id() { return 1; }

        static T add(T a, T b) { return (a + b) % MOD; }
        static T mul(T a, T b) { return (a * b) % MOD; }
    };

    int rows, cols;
    std::vector<T> mat;  // 一维扁平化存储

    Matrix(int r, int c) : rows(r), cols(c), mat(r * c, Semiring::add_id()) {}

    // 下标访问接口
    inline T& operator()(int r, int c) { return mat[r * cols + c]; }
    inline const T& operator()(int r, int c) const { return mat[r * cols + c]; }

    Matrix(std::initializer_list<std::initializer_list<T>> list) {
        rows = list.size();
        cols = (rows > 0) ? list.begin()->size() : 0;
        mat.assign(rows * cols, Semiring::add_id());
        int i = 0;
        for (const auto& row : list) {
            std::copy(row.begin(), row.begin() + std::min((int)row.size(), cols), mat.begin() + i * cols);
            i++;
        }
    }

    static Matrix identity(int n) {
        Matrix res(n, n);
        for (int i = 0; i < n; i++) res.mat[i * n + i] = Semiring::mul_id();
        return res;
    }

    Matrix operator*(const Matrix& other) const {
        assert(cols == other.rows);
        Matrix res(rows, other.cols);

        // 通用高性能 I-K-J 循环
        for (int i = 0; i < rows; i++) {
            int i_off = i * cols;
            int i_res_off = i * other.cols;
            for (int k = 0; k < cols; k++) {
                T left_val = mat[i_off + k];
                if (left_val == Semiring::add_id()) continue;  // 零元优化

                int k_off = k * other.cols;
                for (int j = 0; j < other.cols; j++) {
                    T product = Semiring::mul(left_val, other.mat[k_off + j]);
                    res.mat[i_res_off + j] = Semiring::add(res.mat[i_res_off + j], product);
                }
            }
        }
        return res;
    }

    Matrix& operator*=(const Matrix& other) {
        *this = *this * other;
        return *this;
    }

    Matrix qpow(long long b) const {
        assert(rows == cols);
        Matrix res = Matrix::identity(rows);
        Matrix base = *this;
        while (b > 0) {
            if (b & 1) res = res * base;
            base = base * base;
            b >>= 1;
        }
        return res;
    }
};
