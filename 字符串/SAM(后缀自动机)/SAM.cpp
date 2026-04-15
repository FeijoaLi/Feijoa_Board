struct SAM {
    static constexpr int ALPHABET_SIZE = 26;
    struct Node {
        int len;
        int link;
        std::array<int, ALPHABET_SIZE> next;
        Node() : len{}, link{}, next{} {}
    };
    std::vector<Node> t;
    SAM() {
        init();
    }
    void init() {
        t.assign(2, Node());
        t[0].next.fill(1);
        t[0].len = -1;
    }
    int newNode() {
        t.emplace_back();
        return t.size() - 1;
    }
    int extend(int p, int c) {
        if (t[p].next[c]) {
            int q = t[p].next[c];
            if (t[q].len == t[p].len + 1) {
                return q;
            }
            int r = newNode();
            t[r].len = t[p].len + 1;
            t[r].link = t[q].link;
            t[r].next = t[q].next;
            t[q].link = r;
            while (t[p].next[c] == q) {
                t[p].next[c] = r;
                p = t[p].link;
            }
            return r;
        }
        int cur = newNode();
        t[cur].len = t[p].len + 1;
        while (!t[p].next[c]) {
            t[p].next[c] = cur;
            p = t[p].link;
        }
        t[cur].link = extend(p, c);
        return cur;
    }
    int extend(int p, char c, char offset = 'a') {
        return extend(p, c - offset);
    }

    int next(int p, int x) {
        return t[p].next[x];
    }

    int next(int p, char c, char offset = 'a') {
        return next(p, c - 'a');
    }

    int link(int p) {
        return t[p].link;
    }

    int len(int p) {
        return t[p].len;
    }

    int size() {
        return t.size();
    }
};
/*
// -------- 1. 构建与基础操作 --------
sam.init();                         // void: 初始化，设置节点0为哨兵，节点1为Root
int cur = sam.extend(last, c);      // int:  插入字符 c，必须传入上一次的节点 last。返回当前新节点编号
int cur = sam.extend(last, c, 'a'); // int:  同上，支持由字符自动转为偏移量 (c - 'a')

// -------- 2. 节点属性访问 (API封装) --------
int l = sam.len(u);                 // int:  获取节点 u 的最大子串长度 (maxlen)
int p = sam.link(u);                // int:  获取节点 u 的后缀链接 (Parent/Fail指针)
int v = sam.next(u, c);             // int:  获取 DAG 转移边，无路返回 0 (注意不是-1)

// -------- 3. 内部直接访问 (手写逻辑常用) --------
sam.t[u].len;                       // 变量: maxlen
sam.t[u].link;                      // 变量: parent link
sam.t[u].next[c];                   // 数组: 转移边 (0表示空)

// -------- 4. 实战核心约定 (Crucial!) --------
// [节点索引]: 0 是哨兵(Dummy)，1 是根节点(Root/空串)，有效节点从 1 开始。
// [遍历所有]: for(int i = 1; i < sam.size(); i++) { ... }
// [最短长度]: minlen(u) = sam.t[sam.t[u].link].len + 1
// [本质不同]: sum(len(u) - len(link(u)))  (u > 1)

// -------- 5. 标准构建用法示例 --------
SAM sam;
int last = 1; // !重要: 初始状态必须是 1
for (char c : s) {
    last = sam.extend(last, c);
}
*/
