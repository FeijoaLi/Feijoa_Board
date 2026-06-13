class SAM {
public:
    // 核心数组
    std::vector<int> len;          // len[u]: 状态 u 能表示的最长子串长度
    std::vector<int> lk;         // lk[u]: 后缀链接，指向最长真后缀所在的状态 (child -> fa)
    std::vector<int> fpos;         // fpos[u]: 状态 u 的某个结束位置（通常用于真实节点）
    std::vector<int> is_cp;     // is_cp[u] = 1 表示克隆节点，0 表示真实节点（对应一个前缀）
    std::vector<std::array<int, 26>> nxt; // 转移边

    // 辅助信息（在建完 SAM 后调用 build() 填充）
    std::vector<std::vector<int>> ch;   // parent 树的孩子列表（用于遍历子树） (fa -> child_1 , child_2, ...)
    std::vector<int> sz;                     // sz[u]: 状态 u 的 endpos 集合大小（出现次数）
    std::vector<int> order;                  // 按 len 拓扑排序的节点序列（升序）

    int last;   // 当前前缀对应的状态
    int cnt;    // 本质不同子串个数（在线维护，也可等 build 后计算）

    SAM() {
        len.push_back(0);
        lk.push_back(-1);
        fpos.push_back(0);
        is_cp.push_back(0);
        nxt.emplace_back();
        last = 0;
        cnt = 0;
    }

    // 重定向函数：将 u 往上所有指向 v 的 c 号转移边改为指向 w
    // 返回最终走到的 u（遍历后的 lk 位置）
    int rd(int u, int c, int v, int w) {
        while (u != -1 && nxt[u][c] == v) {
            nxt[u][c] = w;
            u = lk[u];
        }
        return u;
    }

    // 分裂函数：处理 u --c--> v 这条边，让新的纯净终点 w 拥有连续长度特性
    // 返回分裂后的新终点（可能为 v 或新节点 w）
    int sp(int u, int c, int v) {
        if (len[u] + 1 == len[v]) return v;   // 已经满足，无需分裂

        int w = len.size();
        nxt.push_back(nxt[v]);
        lk.push_back(lk[v]);
        len.push_back(len[u] + 1);
        is_cp.push_back(1);                // 克隆节点
        fpos.push_back(fpos[v]);              // 继承结束位置（父节点的某个结束位置）

        lk[v] = w;
        rd(u, c, v, w);                       // 将原来指向 v 的边改指 w
        return w;
    }

    // 扩展一个字符 c（0~25）
    // 返回新的 last 状态编号
    int extend(int u, int c) {
        if (nxt[u][c]) {                    // 该边已存在，说明 SAM 已经包含这个子串
            return sp(u, c, nxt[u][c]);
        }

        int w = len.size();
        nxt.emplace_back();
        lk.push_back(-1);
        len.push_back(len[u] + 1);
        is_cp.push_back(0);              // 真实节点
        fpos.push_back(len[w]);             // 该前缀的结束位置

        u = rd(u, c, 0, w);                 // 把之前死胡同的边指向新节点
        if (u == -1) {
            lk[w] = 0;
        } else {
            lk[w] = sp(u, c, nxt[u][c]);
        }

        cnt += len[w] - len[lk[w]];       // 在线维护本质不同子串个数
        return w;
    }

    // 插入整个字符串
    void insert(const std::string& s) {
        for (char ch : s) last = extend(last, ch - 'a');
    }

    // 在建完所有字符后调用，完成以下工作：
    // 1. 按 len 对节点基数排序 (拓扑序)
    // 2. 计算每个状态的出现次数 sz（即 endpos 大小）
    // 3. 构建 parent 树的孩子列表
    void build() {
        int sz_nodes = len.size();
        // 基数排序
        std::vector<int> bucket(len.back() + 2, 0);
        for (int i = 0; i < sz_nodes; ++i) bucket[len[i]]++;
        for (int i = 1; i < bucket.size(); ++i) bucket[i] += bucket[i-1];
        order.resize(sz_nodes);
        for (int i = sz_nodes - 1; i >= 0; --i) order[--bucket[len[i]]] = i; // 升序：len 小的在前
        // 注意：order 里是 0..sz_nodes-1 的排列，[0] 是根，但我们要逆序处理来计算 sz
        // 所以实际使用逆序遍历：从大到小
        // 计算 sz：真实节点贡献 1，然后沿 lk 累加
        sz.assign(sz_nodes, 0);
        for (int i = sz_nodes - 1; i >= 0; --i) {
            int u = order[i];
            if (!is_cp[u]) sz[u] += 1;   // 真实节点本身拥有一个结束位置
            if (lk[u] != -1) sz[lk[u]] += sz[u];
        }

        // 构建 parent 树
        ch.resize(sz_nodes);
        for (int i = 1; i < sz_nodes; ++i) {
            ch[lk[i]].push_back(i);
        }
    }

    // 判断 t 是否为 S 的子串
    bool contains(const std::string& t) const {
        int p = 0;
        for (char ch : t) {
            int c = ch - 'a';
            if (!nxt[p][c]) return false;
            p = nxt[p][c];
        }
        return true;
    }

    // 子串 t 的出现次数 （要求事先 build() 计算了 sz）
    int count(const std::string& t) const {
        int p = 0;
        for (char ch : t) {
            int c = ch - 'a';
            if (!nxt[p][c]) return 0;
            p = nxt[p][c];
        }
        return sz[p];
    }

    // 本质不同子串个数（若 build 后调用，也可直接返回 cnt，或通过遍历统计）
    long long distinct_substrings() const {
        // 在线维护了 cnt，直接返回即可
        return cnt;
        // 或者通过公式 sum(len[i] - len[lk[i]])
    }

    // t 第一次出现的起始位置（1‑based），不存在返回 -1
    int first_pos(const std::string& t) const {
        int p = 0;
        for (char ch : t) {
            int c = ch - 'a';
            if (!nxt[p][c]) return -1;
            p = nxt[p][c];
        }
        // p 状态的某个结束位置减去长度加1
        return fpos[p] - (int)t.size() + 1;
    }

    // 获取 t 在 S 中所有出现的起始位置（1‑based）
    // 采用迭代栈遍历 parent 子树，防止递归栈溢出
    std::vector<int> all_pos(const std::string& t) const {
        int p = 0, L = t.size();
        for (char ch : t) {
            int c = ch - 'a';
            if (!nxt[p][c]) return {};
            p = nxt[p][c];
        }

        std::vector<int> res;
        std::stack<int> stk;
        stk.push(p);
        while (!stk.empty()) {
            int u = stk.top(); stk.pop();
            if (!is_cp[u]) {               // 真实节点
                res.push_back(fpos[u] - L + 1);
            }
            for (int v : ch[u]) stk.push(v);
        }
        // 结果已按任意顺序收集，为了后续使用可以排序
        // 如果需要有序，可调用 sort，或者使用 BFS 保证顺序
        return res;
    }

    // 计算 S 与模式串 t 的最长公共子串长度
    int lcs(const std::string& t) const {
        int u = 0, cur_len = 0, ans = 0;
        for (char ch : t) {
            int c = ch - 'a';
            while (u && !nxt[u][c]) {
                u = lk[u];
                cur_len = len[u];
            }
            if (nxt[u][c]) {
                u = nxt[u][c];
                ++cur_len;
            } else {
                u = 0;
                cur_len = 0;
            }
            ans = std::max(ans, cur_len);
        }
        return ans;
    }

    // 长度对应的状态（如果需要得到某个长度的任意子串对应的状态，可以沿着转移走）
    // 这里不单独实现，可根据需要添加。
};

class SAM {
    // ... (类的完整定义) ...
};

/*
1. 创建与插入
   SAM sam;
   sam.insert(s);           // s 为 std::string 类型，插入整个串。
   或逐字符扩展：
   int last = 0;
   for (char c : s) last = sam.extend(last, c - 'a');

2. 建立辅助信息（必须在使用以下函数前调用一次）
   sam.build();             // 基数排序、计算 sz、构建 parent 树 children

3. 常用查询接口
   // 判断 t 是否为 S 的子串
   bool sam.contains(t);    // 存在任意一次出现即返回 true

   // t 在 S 中的出现次数
   int sam.count(t);        // 要求事先 build()，利用 sz 数组

   // 本质不同子串总数
   long long sam.distinct_substrings();  // 在线维护，O(1) 返回

   // t 第一次出现的起始位置（1‑based），不存在返回 -1
   int sam.first_pos(t);

   // t 在 S 中所有出现的起始位置（1‑based）
   // 返回 vector<int>，无顺序，通常需自行 sort
   vector<int> sam.all_pos(t);   // 采用迭代栈防爆栈

   // S 与模式串 t 的最长公共子串长度
   int sam.lcs(t);           // O(|t|)

4. 成员变量含义（只读）
   - len[i]      : 状态 i 内最长子串长度
   - link[i]     : 后缀链接指向的父状态
   - fpos[i]     : 状态 i 对应的某个结束位置
   - is_clone[i] : 0 表示真实节点（对应原串前缀），1 表示克隆节点
   - nxt[i][c]   : 状态 i 通过字符 c 的转移目标
   - sz[i]       : 状态 i 的 endpos 大小（出现次数），build() 后有效
   - children[i] : parent 树上 i 的孩子列表，build() 后有效
   - order[i]    : 按 len 升序排列的状态编号，build() 后有效
   - cnt         : 本质不同子串个数（在线维护）

5. 进阶操作

   a) 获取某个状态的任意子串长度区间：
      最短长度 = len[link[i]] + 1，最长长度 = len[i]

   b) 遍历 parent 树（如需要 endpos 集合）：
      for (int v : sam.children[u]) { ... } // u 的所有孩子

   c) 按拓扑序遍历所有状态（适用于 DP）：
      for (int i = sam.order.size()-1; i >= 0; --i) {
          int u = sam.order[i];   // 从长到短
          ... 做 DP 或统计 ...
      }

   d) 如果要修改字符集大小（非 26），需要调整 nxt 的 array 大小。

*/