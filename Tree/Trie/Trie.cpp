#include "bits/stdc++.h"
/*
关注b站<Feijoa_Li>谢谢喵
UID:248614274
https://space.bilibili.com/248614274
________  _______       ___         ___      ________      ________
|\  _____\|\  ___ \     |\  \       |\  \    |\   __  \    |\   __  \
\ \  \__/ \ \   __/|    \ \  \      \ \  \   \ \  \|\  \   \ \  \|\  \
 \ \   __\ \ \  \_|/__   \ \  \   __ \ \  \   \ \  \\\  \   \ \   __  \
  \ \  \_|  \ \  \_|\ \   \ \  \ |\  \\_\  \   \ \  \\\  \   \ \  \ \  \
   \ \__\    \ \_______\   \ \__\\ \________\   \ \_______\   \ \__\ \__\
    \|__|     \|_______|    \|__| \|________|    \|_______|    \|__|\|__|
 */
using namespace std;

using u64 = unsigned long long;
using u32 = unsigned;
using u128 = unsigned __int128;
using i128 = __int128;

#define int long long
#define Ist first
#define Ind second
#define itn long long
#define O (int)0
#define endl "\n"
#define PII pair<int, int>
const int inf = 1e18;

const int MAXN = 150001;
vector<vector<int> > tree(MAXN, vector<int>(26, 0));
vector<int> End(MAXN, 0), Pass(MAXN, 0);
int cnt = 1;

void Build() { cnt = 1; }

void Insert(string word) {
    int cur = 1;  // 头节点
    Pass[cur]++;
    for (int i = 0, path; i < word.length(); i++) {
        path = word[i] - 'a';
        if (tree[cur][path] == 0) {
            // 新建节点
            tree[cur][path] = ++cnt;
        }
        cur = tree[cur][path];  // 跳过去
        Pass[cur]++;            // pass++
    }
    End[cur]++;
}

int Search(string word) {
    int cur = 1;
    for (int i = 0, path; i < word.length(); i++) {
        path = word[i] - 'a';
        if (tree[cur][path] == 0) {
            return 0;
        }
        cur = tree[cur][path];
    }
    return End[cur];
}

int PrefixNumber(string pre) {
    // 有多少个以pre开头的单词
    int cur = 1;
    for (int i = 0, path; i < pre.length(); i++) {
        path = pre[i] - 'a';
        if (tree[cur][path] == 0) {
            return 0;
        }
        cur = tree[cur][path];
    }
    return Pass[cur];
}

void Delete(string word) {
    if (Search(word) > 0) {
        int cur = 1;
        Pass[cur]--;
        for (int i = 0, path; i < word.length(); i++) {
            path = word[i] - 'a';
            int id = tree[cur][path];
            if (--Pass[id] == 0) {
                tree[cur][path] = 0;
                return;
            }
            cur = tree[cur][path];
        }
        End[cur]--;
    }
}

void clear() {
    for (int i = 1; i <= cnt; i++) {
        for (int j = 0; j < 26; j++) {
            tree[i][j] = 0;
        }
        End[i] = 0;
        Pass[i] = 0;
    }
}

void slu() {}

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int T;
    // cin >> T;
    T = 1;
    while (T--) slu();
}
