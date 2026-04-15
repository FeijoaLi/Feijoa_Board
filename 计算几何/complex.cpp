// ===== 点/向量类型 =====
using Point = complex<double>;
// 表示二维点 (x,y)，例如 Point(3,4)

// 获取横纵坐标（注意是函数，不是成员变量）
#define x real
#define y imag

// ====== 基本运算 ======

// 点积：a·b = |a||b|cosθ
double dot(Point a, Point b) {
    return (conj(a) * b).x();
}

// 叉积：a×b = |a||b|sinθ
// 几何意义：>0 表示 b 在 a 的逆时针方向，<0 表示顺时针
double cross(Point a, Point b) {
    return (conj(a) * b).y();
}

// 向量长度 ||a||
double norm(Point a) {
    return abs(a);
}

// 向量长度平方（避免开方，更快）
double norm2(Point a) {
    return dot(a, a);
}

// 向量旋转 θ 弧度
Point rotate(Point a, double theta) {
    return a * polar(1.0, theta);
}

// 将向量 a 单位化（长度 = 1）
Point unit(Point a) {
    return a / abs(a);
}

// 两点间距离
double dist(Point a, Point b) {
    return abs(a - b);
}

// 两向量夹角 [0, π]
double angle(Point a, Point b) {
    return acos(dot(a, b) / (abs(a) * abs(b)));
}

// 向量的极角 [-π, π]
double argAngle(Point a) {
    return arg(a); // atan2(a.y(), a.x())
}

// 将点 p 投影到向量 ab 上
Point projection(Point a, Point b, Point p) {
    Point v = b - a, w = p - a;
    return a + v * (dot(w, v) / norm2(v));
}

// 求点 p 关于直线 ab 的对称点
Point reflection(Point a, Point b, Point p) {
    return projection(a, b, p) * 2.0 - p;
}

// 判断点 c 是否在 ab 直线上（允许误差 eps）
bool onLine(Point a, Point b, Point c, double eps = 1e-9) {
    return fabs(cross(b - a, c - a)) < eps;
}

// 判断点 c 是否在线段 ab 上
bool onSegment(Point a, Point b, Point c, double eps = 1e-9) {
    return onLine(a, b, c, eps) &&
           dot(c - a, c - b) <= eps;
}

// 向量的象限 (极角排序辅助函数)
int quadrant(Point p) {
    if (p.y() > 0 || (p.y() == 0 && p.x() >= 0)) return 0; // 上半平面
    return 1; // 下半平面
}

// ====== 凸包 (Monotone Chain) ======
// 输入：点集 a
// 输出：凸包点集（逆时针顺序，不含重复点）
vector<Point> complexHull(vector<Point> a) {
    // 按 x 再按 y 排序
    sort(a.begin(), a.end(), [&](Point p, Point q) {
        if (p.x() != q.x()) return p.x() < q.x();
        return p.y() < q.y();
    });

    vector<Point> l, h;
    for (auto p : a) {
        // 下凸壳
        while (l.size() > 1 && cross(l.back() - l[l.size()-2], p - l.back()) <= 0)
            l.pop_back();
        l.push_back(p);

        // 上凸壳
        while (h.size() > 1 && cross(h.back() - h[h.size()-2], p - h.back()) >= 0)
            h.pop_back();
        h.push_back(p);
    }

    // 拼接：h 逆序 + l 去掉首尾
    reverse(h.begin(), h.end());
    h.insert(h.end(), l.begin()+1, l.end()-1);

    return h; // 凸包点集，按逆时针顺序
}
