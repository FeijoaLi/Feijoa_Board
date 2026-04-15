struct SAM {
    // --- 配置项 ---
    static constexpr int ALPHABET_SIZE = 26;
    static constexpr int OFFSET = 'a';

    // --- 节点结构体 ---
    struct Node {
        int len;                              // r(u): maxlen
        int link;                             // parent (suffix link)
        std::array<int, ALPHABET_SIZE> next;  // DAG 边 (大字符集请换成 map)
        int cnt_init;                         // 初始标记: 1=实点, 0=克隆

        Node() : len(0), link(0), cnt_init(0) { next.fill(0); }
    };

    // --- 公开成员 ---
    std::vector<Node> t;      // 节点池 (有效下标 1 ~ size()-1)
    int last;                 // 当前光标位置
    long long uniq_sub;       // 【核心】实时维护本质不同子串总数

    // --- 统计数据 ---
    std::vector<int> CNT;     // 内部出现次数数组 (需 build)
    std::vector<int> tax, rk; // 辅助内存 (基数排序用)

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
        t.assign(2, Node());  // 0:哨兵, 1:根
        t[0].next.fill(1);
        t[0].len = -1;
        last = 1;
        uniq_sub = 0;         // 重置计数器
    }

    // --- 核心操作 ---

    // 【广义 SAM 关键】: 插入新串前必须调用，重置光标到根
    void restart() { last = 1; }

    int extend(int c_int) {
        int p = last;

        // [分支1] 路径已存在 (广义 SAM 特有或重复子串)
        // 此时不产生新的本质不同子串，仅做路径复用或拆分
        if (t[p].next[c_int]) {
            int q = t[p].next[c_int];
            if (t[q].len == t[p].len + 1) {
                last = q; // 状态完全匹配，直接复用
            } else {
                // 状态被包含，需要拆分 (Clone)
                int r = newNode();
                t[r].len = t[p].len + 1;
                t[r].link = t[q].link;
                t[r].next = t[q].next;
                t[r].cnt_init = 0; // 克隆点无初始贡献
                t[q].link = r;
                while (t[p].next[c_int] == q) {
                    t[p].next[c_int] = r;
                    p = t[p].link;
                }
                last = r;
            }
            // 注意：分支1 只是维护结构，不更新 uniq_sub
            return last;
        }

        // [分支2] 创建新实点 (Cur)
        // 只有此处代表产生了新的子串集合
        int cur = newNode();
        t[cur].len = t[p].len + 1;
        t[cur].cnt_init = 1; // 实点有初始贡献

        while (!t[p].next[c_int]) {
            t[p].next[c_int] = cur;
            p = t[p].link;
        }

        // 寻找 link
        int link_node;
        if (t[p].next[c_int]) {
            int q = t[p].next[c_int];
            if (t[q].len == t[p].len + 1) {
                link_node = q;
            } else {
                int r = newNode();
                t[r].len = t[p].len + 1;
                t[r].link = t[q].link;
                t[r].next = t[q].next;
                t[r].cnt_init = 0;
                t[q].link = r;
                while (t[p].next[c_int] == q) {
                    t[p].next[c_int] = r;
                    p = t[p].link;
                }
                link_node = r;
            }
        } else {
            link_node = 1;
        }
        t[cur].link = link_node;

        // 【核心统计】更新本质不同子串数
        // 增量 = len(cur) - len(link(cur))
        uniq_sub += (long long)t[cur].len - t[t[cur].link].len;

        last = cur;
        return cur;
    }

    int extend(char c) { return extend(c - OFFSET); }
    void extend(const std::string& s) { for (char c : s) extend(c); }

    // --- 统计与构建 (O(N)) ---
    // 注意：如果只求 uniq_sub，不需要调用 build()
    // 只有需要查询 cnt(u) 或 query(string) 时才调用
    void build() {
        int n = t.size();
        CNT.assign(n, 0);

        // 1. 初始贡献
        for (int i = 0; i < n; i++) CNT[i] = t[i].cnt_init;

        // 2. 基数排序
        // 为了安全覆盖所有可能的长度，这里用 n 作为桶的大小
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

    // 获取实时本质不同子串数 (无需 build)
    long long get_uniq_sub() const {
        return uniq_sub;
    }

    // 获取节点 u 的出现次数 (需 build)
    long long cnt(int u) const {
        return CNT[u];
    }

    // 查特定字符串的出现次数 (需 build)
    long long query(const std::string& T) const {
        int u = run(T);
        return u ? CNT[u] : 0;
    }

    // --- 基础 API ---
    int run(const std::string& T) const {
        int u = 1;
        for (char c : T) {
            int c_int = c - OFFSET;
            if (!t[u].next[c_int]) return 0;
            u = t[u].next[c_int];
        }
        return u;
    }

    int size() const { return t.size(); }
    int link(int u) const { return t[u].link; }
    int r(int u) const { return t[u].len; }
    int l(int u) const { return t[t[u].link].len + 1; }
    int next(int u, char c) const { return t[u].next[c - OFFSET]; }
    int next(int u, int c) const { return t[u].next[c]; }

   private:
    int newNode() {
        t.emplace_back();
        return t.size() - 1;
    }
};

/* ========================================================================
   SAM 模版接口说明与使用指南
========================================================================

========================================================================
一、构建与初始化
------------------------------------------------------------------------
void init(int n = 0)
   - 作用：初始化自动机，清空节点池和计数器。
   - 参数 n：(可选) 预估字符串长度，用于 reserve 内存。

void restart()
   - 作用：【广义 SAM 专用】将 last 指针重置为 1 (根节点)。
   - 场景：在插入多个字符串时，每处理完一个串调用一次，再插入下一个。

int extend(char c) / void extend(string s)
   - 作用：插入字符或字符串，增量构建 SAM。
   - 机制：内部会实时更新 uniq_sub (本质不同子串数)。

void build()
   - 作用：【必须显式调用】对 SAM 进行基数排序和拓扑更新，计算所有节点的 CNT。
   - 时机：在所有字符串 extend 完毕后调用。
   - 复杂度：O(N)。

========================================================================
二、DAG (自动机图) 维度查询
------------------------------------------------------------------------
int next(int u, char c) / int next(int u, int c)
   - 作用：查询转移边。支持传字符('a')或数字(0)。

int run(const std::string& s)
   - 作用：在自动机上跑字符串 s，返回最终停留的节点编号。
   - 返回：若中途失配返回 0。

long long query(const std::string& T)
   - 作用：查询 T 在原串中的出现次数。需先 build()。

========================================================================
三、Parent Tree (后缀链接树) 维度查询
------------------------------------------------------------------------
int link(int u)
   - 作用：获取父节点 (Suffix Link)。

int r(int u)
   - 作用：获取节点 u 代表的最长子串长度 (Max Length)。

int l(int u)
   - 作用：获取节点 u 代表的最短子串长度 (Min Length)。
   - 公式：sam.r(sam.link(u)) + 1。
   - 解释：节点 u 代表的子串长度区间为 [l(u), r(u)]。

========================================================================
四、数据获取接口 (Data Access)
------------------------------------------------------------------------
long long cnt(int u)
   - 作用：获取节点 u 代表子串的出现次数。
   - 前置：必须先 build()。

long long get_uniq_sub()
   - 作用：获取本质不同的子串总数。
   - 特性：在线算法，extend 过程中实时更新，无需 build。

========================================================================
五、API 速查 (Signature - Usage)
------------------------------------------------------------------------
SAM sam(n);      // void - 初始化SAM，n为预估大小
sam.init(n);     // void - 初始化SAM，n为预估大小
sam.restart();   // void - 重置插入点last=1，用于广义SAM换串
sam.extend(c);   // int - 插入字符c，返回节点编号
sam.extend(s);   // void - 插入字符串s
sam.build();     // void - 构建统计信息(CNT)，必须最后显式调用

sam.cnt(u);      // long long - 获取节点u的出现次数(需build)
sam.get_uniq_sub(); // long long - 获取本质不同子串总数(在线，无需build)
sam.query(T);    // long long - 查询字符串T的出现次数(需build)
sam.run(s);      // int - 在SAM上运行s，返回节点编号或0

sam.next(u, c);  // int - 获取u经过c转移到的节点编号，如果不存在返回0
sam.link(u);     // int - 获取u的后缀链接(父节点)
sam.r(u);        // int - 获取u的最长子串长度(maxlen)
sam.l(u);        // int - 获取u的最短子串长度(minlen)
sam.size();      // int - 获取SAM总节点数(有效下标1~size-1)
========================================================================
*/