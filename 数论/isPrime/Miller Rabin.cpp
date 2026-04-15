#include "bits/stdc++.h"
//判断n是否为素数，使用Miller-Rabin素性测试
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
int qmul(int a,int b,int mod)//快速乘
{
    int c=(long double)a/mod*b;
    int res=(unsigned long long)a*b-(unsigned long long)c*mod;
    return (res+mod)%mod;
}
int qpow(int a,int n,int mod)//快速幂
{
    int res=1;
    while(n)
    {
        if(n&1) res=qmul(res,a,mod);
        a=qmul(a,a,mod);
        n>>=1;
    }
    return res;
}
bool MRtest(int n)//Miinter Rabin Test
{
    if(n<3||n%2==0) return n==2;//特判
    int u=n-1,t=0;
    while(u%2==0) u/=2,++t;
    int ud[]={2,325,9375,28178,450775,9780504,1795265022};
    for(int a:ud)
    {
        int v=qpow(a,u,n);
        if(v==1||v==n-1||v==0) continue;
        for(int j=1;j<=t;j++)
        {
            v=qmul(v,v,n);
            if(v==n-1&&j!=t){v=1;break;}//出现一个n-1，后面都是1，直接跳出
            if(v==1) return 0;//这里代表前面没有出现n-1这个解，二次检验失败
        }
        if(v!=1) return 0;//Fermat检验
    }
    return 1;
}
/*

*/
void slu() {
    int n;
    cin>>n;
    if(MRtest(n)) cout<<"Yes"<<endl;
    else cout<<"No"<<endl;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int T = 1;
    cin >> T;
    while (T--)slu();
    return 0;
}
