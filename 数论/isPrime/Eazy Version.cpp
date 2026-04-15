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
bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

/*

*/
void slu() {
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int T = 1;
    // cin >> T;
    while (T--)slu();
    return 0;
}
