#include "bits/stdc++.h"
/*
________  _______       ___         ___      ________      ________
|\  _____\|\  ___ \     |\  \       |\  \    |\   __  \    |\   __  \
\ \  \__/ \ \   __/|    \ \  \      \ \  \   \ \  \|\  \   \ \  \|\  \
 \ \   __\ \ \  \_|/__   \ \  \   __ \ \  \   \ \  \\\  \   \ \   __  \
  \ \  \_|  \ \  \_|\ \   \ \  \ |\  \\_\  \   \ \  \\\  \   \ \  \ \  \
   \ \__\    \ \_______\   \ \__\\ \________\   \ \_______\   \ \__\ \__\
	\|__|     \|_______|    \|__| \|________|    \|_______|    \|__|\|__|
 */
using namespace std;
#define int long long
#define endl "\n"
#define PII pair<int,int>
#define x first
#define y second

const int MAXN = 150001;
vector<vector<int> > tree(MAXN, vector<int>(2, 0));
vector<int> End(MAXN, 0), Pass(MAXN, 0);
int cnt = 1;

void Build() {
	cnt = 1;
}

void Insert(int num) {
	int cur = 1; //头节点
	Pass[cur]++;
	for (int i = 29; i >= 0; i--) {
		int path = (num >> i) & 1;
		if (!tree[cur][path]) {
			tree[cur][path] = ++cnt;
		}
		cur = tree[cur][path];
		Pass[cur]++;
	}
	End[cur]++;
}

int Search(int num) {
	int cur = 1;
	for (int i = 29; i >= 0; i--) {
		int path = (num >> i) & 1;
		if (!tree[cur][path]) {
			return 0;
		}
		cur = tree[cur][path];
	}
	return End[cur];
}

int PrefixNumber(int num) {
	//有多少个以num开头的数字
	int cur = 1;
	for (int i = 29; i >= 0; i--) {
		int path = (num >> i) & 1;
		if (!tree[cur][path]) {
			return 0;
		}
		cur = tree[cur][path];
	}
	return Pass[cur];
}

void Delete(int num) {
	//删除以num开头的数字
	if (Search(num) > 0) {
		int cur = 1;
		Pass[cur]--;
		for (int i = 29; i >= 0; i--) {
			int path = (num >> i) & 1;
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
		for (int j = 0; j < 2; j++) {
			tree[i][j] = 0;
		}
		End[i] = 0;
		Pass[i] = 0;
	}
}

void slu() {
}

signed main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	int T;
	// cin >> T;
	T = 1;
	while (T--)slu();
}

