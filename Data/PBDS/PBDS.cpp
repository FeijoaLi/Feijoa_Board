#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
// include 放到前面，否则会出现未定义的引用错误

template<typename T>
using ordered_set = tree<
    T,
    null_type,
    std::less<T>,
    rb_tree_tag,
    tree_order_statistics_node_update
>;
//ordered_multiset<pair<int, int>> == multiset;
class PBDSMultiSet {
private:
    using pii = std::pair<int, int>;
    using Tree = tree<pii, null_type, std::less<pii>, rb_tree_tag, tree_order_statistics_node_update>;

    Tree ms;
    int uid = 0;  // 用于区分重复元素

public:
    void insert(int x) {
        ms.insert({x, uid++});
    }

    void erase(int x) {
        auto it = ms.lower_bound({x, 0});
        if (it != ms.end() && it->first == x) {
            ms.erase(it);
        }
    }

    int count(int x) {
        return ms.order_of_key({x + 1, 0}) - ms.order_of_key({x, 0});
    }

    int size() const {
        return ms.size();
    }

    int kth(int k) {  // k 从 0 开始
        if (k < 0 || k >= (int)ms.size()) return -1;
        return ms.find_by_order(k)->first;
    }

    int rank(int x) {  // 小于 x 的数量
        return ms.order_of_key({x, 0});
    }

    bool empty() const {
        return ms.empty();
    }

    void clear() {
        ms.clear();
        uid = 0;
    }
};

//map
template<typename Key, typename Value>
class PBDSMap {
private:
    using omap = tree<
        Key, Value, std::less<Key>,
        rb_tree_tag,
        tree_order_statistics_node_update
    >;

    omap mp;

public:
    Value& operator[](const Key& k) {
        return mp[k];  // 自动插入默认值
    }

    bool contains(const Key& k) const {
        return mp.find(k) != mp.end();
    }

    void erase(const Key& k) {
        mp.erase(k);
    }

    int size() const {
        return mp.size();
    }

    int rank(const Key& k) const {
        return mp.order_of_key(k);
    }

    std::pair<Key, Value> kth(int k) const {
        if (k < 0 || k >= (int)mp.size()) throw out_of_range("k out of range");
        auto it = mp.find_by_order(k);
        return {it->first, it->second};
    }

    void clear() {
        mp.clear();
    }
};

/*
https://codeforces.com/problemset/problem/1915/F
| 功能       | 接口                  | 复杂度           |
| -------- | ------------------- | ------------- |
| 插入元素     | `insert(x)`         | log n |
| 删除元素     | `erase(x)`-'erase(lower_bound(x))'          | log n |
| 第 k 小     | `*find_by_order(k)` | log n |
| 小于 x 的数量 | `order_of_key(x)`   | log n |
| 支持重复元素   | 插入 `pair<val, id>`  | log n |
| 查询元素排名   | `order_of_key(x)`   | log n |

*/
void multiset_test() {
    PBDSMultiSet s;

    s.insert(5);
    s.insert(5);
    s.insert(3);
    s.insert(7);

    cout << "Count of 5: " << s.count(5) << "\n";       // 2
    cout << "Rank of 5: " << s.rank(5) << "\n";         // 1
    cout << "1st smallest: " << s.kth(1) << "\n";       // 5
    s.erase(5);
    cout << "Count of 5 after erase: " << s.count(5) << "\n"; // 1
}
void map_test() {
    PBDSMap<int, string> mymap;

    mymap[10] = "ten";
    mymap[5] = "five";
    mymap[8] = "eight";

    cout << mymap.rank(9) << "\n";          // 输出 2（5, 8）
    auto [k, v] = mymap.kth(1);              // 第 1 小的 key
    cout << k << " => " << v << "\n";       // 输出 8 => eight

    if (mymap.contains(10)) {
        cout << "10 exists\n";
    }

}
void slu() {
    multiset_test();
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int t = 1;
    // cin >> t;
    while (t--)slu();
    return 0;
}
