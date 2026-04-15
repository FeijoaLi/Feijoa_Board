int qpow(int a, int b, int m) {
    int res = 1 % m;
    for (a %= m; b; b >>= 1, a = a * a % m)
        if (b & 1) res = res * a % m;
    return res;
}

int qmul(int a,int b,int mod)//快速乘
{
    int c=(long double)a/mod*b;
    int res=(unsigned long long)a*b-(unsigned long long)c*mod;
    return (res+mod)%mod;
}
int unqpow(int a,int n,int mod)//快速幂
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