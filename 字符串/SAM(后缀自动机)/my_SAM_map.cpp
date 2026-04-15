/*
    SAM Template (Map Version)
    适用场景: 字符集很大 (如 10^9) 或 未知字符集
    特性:
    1. 广义 SAM 支持 (restart)
    2. 在线统计本质不同子串 (uniq_sub)
    3. std::map 处理转移边
*/
struct SAM {
    // --- 节点结构体 ---
    struct Node {
        int len;                  // r(u): maxlen
        int link;                 // parent (suffix link)
        std::map<int, int> next;  // 【关键】使用 map 处理大字符集
        int cnt_init;             // 初始标记: 1=实点, 0=克隆

        Node() : len(0), link(0), cnt_init(0) {}
    };

    // --- 公开成员 ---
    std::vector<Node> t;      // 节点池 (有效下标 1 ~ size()-1)
    int last;                 // 当前光标位置
    long long uniq_sub;       // 【核心】实时维护本质不同子串总数

   private:
    std::vector<int> CNT;     // 内部出现次数数组 (需 build)
    std::vector<int> tax, rk; // 辅助内存

   public:
    // --- 构造与初始化 ---
    SAM() { init(); }
    SAM(int n) { init(n); }

    void init(int n = 0) {
        t.clear();
        CNT.clear();
        if (n > 0) {
            t.reserve(n * 2 + 1);
            CNT.reserve(n * 2 + 1);
            tax.reserve(n + 1);
            rk.reserve(n * 2 + 1);
        }
        t.emplace_back(); // 0: 哨兵
        t.emplace_back(); // 1: 根
        t[0].len = -1;
        t[1].len = 0;
        t[1].link = 0;
        last = 1;
        uniq_sub = 0;
    }

    // --- 核心操作 ---

    // 【广义 SAM 关键】: 插入新串前调用，重置光标到根
    void restart() { last = 1; }

    // 【核心构建逻辑】
    // 返回值：当前的本质不同子串总数
    long long extend(int c) {
        int p = last;

        // [分支1] 路径已存在 (广义 SAM 特有或重复子串)
        // 此时不产生新的本质不同子串，仅做路径复用或拆分
        if (t[p].next.count(c)) {
            int q = t[p].next[c];
            if (t[q].len == t[p].len + 1) {
                last = q; // 完全复用
            } else {
                // 状态被包含，需要拆分 (Clone)
                int r = newNode();
                t[r].len = t[p].len + 1;
                t[r].link = t[q].link;
                t[r].next = t[q].next; // map 拷贝
                t[r].cnt_init = 0;     // 克隆点无初始贡献
                t[q].link = r;
                while (p && t[p].next.count(c) && t[p].next[c] == q) {
                    t[p].next[c] = r;
                    p = t[p].link;
                }
                last = r;
            }
            // 分支1 不更新 uniq_sub
            return uniq_sub;
        }

        // [分支2] 创建新实点 (Cur)
        // 只有此处代表产生了新的子串集合
        int cur = newNode();
        t[cur].len = t[p].len + 1;
        t[cur].cnt_init = 1; // 实点有初始贡献

        while (p && !t[p].next.count(c)) {
            t[p].next[c] = cur;
            p = t[p].link;
        }

        if (!p) {
            t[cur].link = 1;
        } else {
            int q = t[p].next[c];
            if (t[q].len == t[p].len + 1) {
                t[cur].link = q;
            } else {
                int r = newNode();
                t[r].len = t[p].len + 1;
                t[r].link = t[q].link;
                t[r].next = t[q].next; // map 拷贝
                t[r].cnt_init = 0;
                t[q].link = r;
                t[cur].link = r;
                while (p && t[p].next.count(c) && t[p].next[c] == q) {
                    t[p].next[c] = r;
                    p = t[p].link;
                }
            }
        }

        // 【核心统计】更新本质不同子串数
        // 增量 = len(cur) - len(link(cur))
        uniq_sub += (long long)t[cur].len - t[t[cur].link].len;

        last = cur;
        return uniq_sub;
    }

    // 辅助重载
    void extend(const std::vector<int>& s) { for (int c : s) extend(c); }
    void extend(const std::string& s) { for (char c : s) extend(c); }

    // --- 统计与构建 (O(N)) ---
    // 只有需要查询 cnt(u) 或 query(string) 时才调用
    void build() {
        int n = t.size();
        CNT.assign(n, 0);

        // 1. 初始贡献
        for (int i = 0; i < n; i++) CNT[i] = t[i].cnt_init;

        // 2. 基数排序
        if (tax.size() <= n) tax.assign(n + 1, 0);
        else std::fill(tax.begin(), tax.begin() + n + 1, 0);

        if (rk.size() < n) rk.resize(n);

        for (int i = 1; i < n; i++) tax[t[i].len]++;
        for (int i = 1; i <= n; i++) tax[i] += tax[i - 1];
        for (int i = 1; i < n; i++) rk[tax[t[i].len]--] = i;

        // 3. 拓扑累加
        for (int i = n - 1; i >= 1; i--) {
            int u = rk[i];
            if (t[u].link > 1) CNT[t[u].link] += CNT[u];
        }
    }

    // --- 统一查询接口 ---

    long long get_uniq_sub() const { return uniq_sub; }
    long long cnt(int u) const { return CNT[u]; }

    // 泛型 Run: 支持 vector<int> 或 string
    template <typename Container>
    int run(const Container& T) const {
        int u = 1;
        for (auto c : T) {
            auto it = t[u].next.find(c);
            if (it == t[u].next.end()) return 0;
            u = it->second;
        }
        return u;
    }

    // 查询出现次数 (需先 build)
    long long query(const std::vector<int>& T) const {
        int u = run(T);
        return u ? CNT[u] : 0;
    }
    long long query(const std::string& T) const {
        int u = run(T);
        return u ? CNT[u] : 0;
    }

    // --- 基础 API ---
    int size() const { return t.size(); }
    int link(int u) const { return t[u].link; }
    int r(int u) const { return t[u].len; }
    int l(int u) const { return t[t[u].link].len + 1; }

    // 安全获取 next，不存在返回 0
    int next(int u, int c) const {
        auto it = t[u].next.find(c);
        return (it != t[u].next.end()) ? it->second : 0;
    }

   private:
    int newNode() {
        t.emplace_back();
        return t.size() - 1;
    }
};

/* ========================================================================
   SAM (Map Version) 模版接口说明与使用指南
========================================================================

========================================================================
一、构建与初始化
------------------------------------------------------------------------
void init(int n = 0)
   - 作用：初始化自动机，清空节点池和计数器。
   - 参数 n：(可选) 预估总长度，用于 reserve 内存。

void restart()
   - 作用：【广义 SAM 专用】将 last 指针重置为 1 (根节点)。
   - 场景：在插入多个字符串时，每处理完一个串调用一次，再插入下一个。

long long extend(int c)
   - 作用：插入一个字符(或数字)。
   - 返回：当前的本质不同子串总数 (uniq_sub)。
   - 复杂度：O(log Σ) per char，map 操作带来 log 开销。

void extend(vector<int> s) / void extend(string s)
   - 作用：批量插入整个序列。

void build()
   - 作用：【显式调用】对 SAM 进行基数排序和拓扑更新，计算 CNT。
   - 时机：只有在需要查询子串出现次数时才调用。
   - 复杂度：O(N)。

========================================================================
二、数据获取与查询 (Query)
------------------------------------------------------------------------
long long get_uniq_sub()
   - 作用：获取本质不同的子串总数。
   - 特性：在线算法 (Online)，extend 过程中实时更新，无需 build。

long long cnt(int u)
   - 作用：获取节点 u 代表子串的出现次数。
   - 前置：必须先 build()。

long long query(vector<int> T) / query(string T)
   - 作用：查询序列 T 在原串中的出现次数。
   - 前置：必须先 build()。
   - 返回：出现次数，若 T 不是子串返回 0。

int run(vector<int> T) / run(string T)
   - 作用：在自动机上跑序列 T。
   - 返回：最终停留的节点编号，若失配返回 0。

========================================================================
三、API 速查 (Signature - Usage)
------------------------------------------------------------------------
SAM sam(n);             // 构造函数，n 为预估大小
sam.init(n);            // 初始化/重置
sam.restart();          // 广义 SAM 换串时重置光标

sam.extend(100);        // 插入数字 100，返回当前 uniq_sub
sam.extend(vec);        // 插入 vector<int>
sam.build();            // 构建 CNT 数组 (统计用)

sam.get_uniq_sub();     // 获取本质不同子串总数 (long long)
sam.cnt(u);             // 获取节点 u 的出现次数 (long long)
sam.query(T);           // 查询 T 的出现次数 (long long)

sam.size();             // 获取总节点数 (int)
sam.link(u);            // 获取后缀链接/父节点 (int)
sam.r(u);               // 获取最长子串长度 len (int)
sam.l(u);               // 获取最短子串长度 minlen (int)
sam.next(u, c);         // 获取转移边 u->c，不存在返回 0 (int)


sam.t[u].len;           // 节点 u 的长度
sam.t[u].link;          // 节点 u 的父节点
sam.t[u].next[c];       // (不推荐直接访问 map) 节点 u 的转移边
sam.uniq_sub;           // 实时本质不同子串数
========================================================================
*/