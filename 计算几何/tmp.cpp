using Real = double;
const Real EPS = 1e-9;
const Real PI = acos(-1.0);

// 符号函数：处理精度
template <typename T>
int sign(T x) {
    if constexpr (std::is_floating_point_v<T>) {
        if (std::abs(x) < EPS) return 0;
    }
    return x < 0 ? -1 : 1;
}

template <typename T>
int dcmp(T x, T y) {
    return sign(x - y);
}

template <typename T>
struct Point {
    T x, y;

    constexpr Point() : x(0), y(0) {}
    constexpr Point(T x, T y) : x(x), y(y) {}

    // 基础运算
    constexpr Point operator+(const Point &r) const { return {x + r.x, y + r.y}; }
    constexpr Point operator-(const Point &r) const { return {x - r.x, y - r.y}; }
    constexpr Point operator*(T k) const { return {x * k, y * k}; }
    constexpr Point operator/(T k) const { return {x / k, y / k}; }
    
    constexpr Point &operator+=(const Point &r) { x += r.x; y += r.y; return *this; }
    constexpr Point &operator-=(const Point &r) { x -= r.x; y -= r.y; return *this; }
    constexpr Point &operator*=(T k) { x *= k; y *= k; return *this; }
    constexpr Point &operator/=(T k) { x /= k; y /= k; return *this; }

    // 比较运算 (用于排序或Map)
    bool operator<(const Point &r) const {
        int s = dcmp(x, r.x);
        return s == 0 ? dcmp(y, r.y) < 0 : s < 0;
    }
    bool operator==(const Point &r) const { return dcmp(x, r.x) == 0 && dcmp(y, r.y) == 0; }
    bool operator!=(const Point &r) const { return !(*this == r); }

    // 几何属性
    T abs2() const { return x * x + y * y; } // 模长的平方
    double abs() const { return std::sqrt(abs2()); } // 模长
    Point unit() const { return *this / abs(); } // 单位向量
    Point normal() const { return {-y, x}; } // 法向量（逆时针旋转90度）
    
    // 旋转 (逆时针 rad)
    Point rotate(double rad) const {
        return {x * cos(rad) - y * sin(rad), x * sin(rad) + y * cos(rad)};
    }

    // 点积与叉积
    friend T dot(const Point &a, const Point &b) { return a.x * b.x + a.y * b.y; }
    friend T cross(const Point &a, const Point &b) { return a.x * b.y - a.y * b.x; }
    // 两个向量的夹角 A·B = |A||B|cos(theta)
    friend double angle(const Point &a, const Point &b) {
        return acos(std::clamp<double>(dot(a, b) / a.abs() / b.abs(), -1.0, 1.0));
    }

    // 输入输出
    friend std::istream &operator>>(std::istream &is, Point &p) { return is >> p.x >> p.y; }
    friend std::ostream &operator<<(std::ostream &os, const Point &p) { return os << p.x << " " << p.y; }
};



// 直线 (点向式: p + t*v)
template <typename T>
struct Line {
    Point<T> p, v;
    
    constexpr Line() {}
    constexpr Line(Point<T> p, Point<T> v) : p(p), v(v) {}

    // 两点构造直线
    static Line fromPoints(Point<T> a, Point<T> b) { return {a, b - a}; }
    
    // 点是否在直线上
    bool onLine(Point<T> a) const { return sign(cross(v, a - p)) == 0; }
    
    // 点到直线的距离
    double dist(Point<T> a) const { return std::abs(cross(v, a - p)) / v.abs(); }
    
    // 点在直线上的投影点
    Point<T> proj(Point<T> a) const { return p + v * (dot(v, a - p) / v.abs2()); }
    
    // 点关于直线的对称点
    Point<T> reflect(Point<T> a) const { return proj(a) * 2 - a; }
};

using L = Line<Real>;

// ----- 常用几何算法函数库 -----

// 1. 线段相交判定 (包含端点)
// 快速排斥 + 跨立实验
template<typename T>
bool onSegment(Point<T> p, Point<T> a, Point<T> b) {
    return sign(cross(a - p, b - p)) == 0 && sign(dot(a - p, b - p)) <= 0;
}

template<typename T>
bool segIntersect(Point<T> a1, Point<T> a2, Point<T> b1, Point<T> b2) {
    T c1 = cross(a2 - a1, b1 - a1), c2 = cross(a2 - a1, b2 - a1);
    T c3 = cross(b2 - b1, a1 - b1), c4 = cross(b2 - b1, a2 - b1);
    if (sign(c1) * sign(c2) < 0 && sign(c3) * sign(c4) < 0) return true;
    // 处理端点重合或共线情况
    if (onSegment(b1, a1, a2)) return true;
    if (onSegment(b2, a1, a2)) return true;
    if (onSegment(a1, b1, b2)) return true;
    if (onSegment(a2, b1, b2)) return true;
    return false;
}

// 2. 直线交点 (调用前需保证不平行: cross(v1, v2) != 0)
// Returns P + v * t
template<typename T>
Point<T> lineIntersection(Point<T> p1, Point<T> v1, Point<T> p2, Point<T> v2) {
    T t = cross(p2 - p1, v2) / cross(v1, v2);
    return p1 + v1 * t;
}

// 3. 多边形面积 (鞋带公式)
template<typename T>
double polygonArea(const std::vector<Point<T>>& poly) {
    double area = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        area += cross(poly[i], poly[(i + 1) % n]);
    }
    return std::abs(area) / 2.0;
}

// 4. 点在多边形内判定 (射线法)
// 2: 内部, 1: 边界, 0: 外部
template<typename T>
int pointInPoly(Point<T> p, const std::vector<Point<T>>& poly) {
    int n = poly.size();
    int wn = 0;
    for (int i = 0; i < n; i++) {
        Point<T> p1 = poly[i], p2 = poly[(i + 1) % n];
        if (onSegment(p, p1, p2)) return 1; // 边界
        int k = sign(cross(p2 - p1, p - p1));
        int d1 = sign(p1.y - p.y);
        int d2 = sign(p2.y - p.y);
        if (k > 0 && d1 <= 0 && d2 > 0) wn++;
        if (k < 0 && d2 <= 0 && d1 > 0) wn--;
    }
    return wn != 0 ? 2 : 0;
}
using P = Point<long long>; // 或 double
/* 使用说明：
P a(1, 2), b(3, 4);
a + b, a - b;        // 向量加减
a * 2.5, a / 2;      // 数乘除
dot(a, b);           // 点积
cross(a, b);         // 叉积
a.abs();             // 模长
a.unit();            // 单位向量
a.rotate(PI/2);      // 旋转

Line<double> l = Line<double>::fromPoints(a, b);
l.proj(p);           // 点 p 在直线 l 上的投影
l.dist(p);           // 点 p 到直线 l 的距离

onSegment(p, a, b);        // 点是否在线段 ab 上
segIntersect(a, b, c, d);  // 线段 ab 与 cd 是否相交
lineIntersection(p1, v1, p2, v2); // 求直线交点
polygonArea(pts);          // 多边形面积
*/