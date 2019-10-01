//BISMILLAHIR RAHMANIR RAHIM
#include<bits/stdc++.h>
//Solution Idea
//

using namespace std;

typedef long long int ll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<vi> vvi;
typedef vector<vl> vvl;
typedef pair<int,int> pii;
typedef pair < int , pii > piii;
typedef pair<double, int > pdi;
typedef pair<ll, ll> pll;
typedef vector<pii> vii;
typedef vector<pll> vll;
typedef pair < ll , pll > plll;
typedef pair < pll , pll > pllll;

#define all(v) (v).begin(), (v).end()
#define KeepUnique(v) (v).erase( unique(all(v)), v.end() )
#define PB push_back
#define F first
#define S second
#define MP make_pair
#define endl '\n'
#define mx 100005
#define mx1 100005
#define mx2 200005
#define mx3 300005
#define WHITE 0
#define RED 1
#define BLACK 2

#define bp __builtin_popcountll
#define fraction() cout.unsetf(ios::floatfield); cout.precision(14); cout.setf(ios::fixed,ios::floatfield);
#define hi cout<<"Dhukse " << endl;
#define si(x) scanf("%d",&x)
#define sii(x,y) scanf("%d %d",&x,&y)
#define siii(x,y,z) scanf("%d %d %d",&x,&y,&z)
#define sl(x) scanf("%lld",&x)
#define sll(x,y) scanf("%lld %lld",&x,&y)
#define slll(x,y,z) scanf("%lld %lld %lld",&x,&y,&z)

const double PI = acos(-1);
const double eps = 1e-12;
const int inf = 2000000000;
const ll infLL = (ll)2e18;
ll MOD =  1000000007;
int MOD1 = 1000000007;
int MOD2 = 1000000009;
#define harmonic(n) 0.57721566490153286l(Eulers-Mascharoni constant)+log(n) ( 1/1 + 1/2 + .... + 1/n );

#define mem(a,b) memset(a, b, sizeof(a) )
#define gcd(a,b) __gcd(a,b)
#define lcm(a,b) (a*(b/gcd(a,b)))
#define sqr(a) ((a) * (a))
#define optimize() ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);

typedef vector<int>::iterator vit;
typedef set<int>::iterator sit;

inline bool checkBit(ll n, ll i) { return n&(1LL<<i); }
inline ll setBit(ll n, ll i) { return n|(1LL<<i);; }
inline ll resetBit(ll n, ll i) { return n&(~(1LL<<i)); }

int dx[] = {0, 0, +1, -1};
int dy[] = {+1, -1, 0, 0};
//int dx[] = {+1, 0, -1, 0, +1, +1, -1, -1};
//int dy[] = {0, +1, 0, -1, +1, -1, +1, -1};

inline bool EQ(double a, double b) { return fabs(a-b) < 1e-9; }
inline bool isLeapYear(ll year) { return (year%400==0) || (year%4==0 && year%100!=0); }
inline void normal(ll &a) { a %= MOD; (a < 0) && (a += MOD); }
inline ll modMul(ll a, ll b) { a %= MOD, b %= MOD; normal(a), normal(b); return (a*b)%MOD; }
inline ll modAdd(ll a, ll b) { a %= MOD, b %= MOD; normal(a), normal(b); return (a+b)%MOD; }
inline ll modSub(ll a, ll b) { a %= MOD, b %= MOD; normal(a), normal(b); a -= b; normal(a); return a; }
inline ll modPow(ll b, ll p) { ll r = 1LL; while(p) { if(p&1) r = modMul(r, b); b = modMul(b, b); p >>= 1LL; } return r; }
inline ll modDiv(ll a, ll b) { return modMul(a, modPow(b, MOD-2)); }

bool comp( const pair < ll , pll > &p1 , const pair < ll , pll > &p2 ){ return p1.F > p2.F ;}
bool comp1( const pll &p1 , const pll &p2 ){
          if( p1.F == p2.F ){
                 return p1.S > p2.S;
          }
          return p1.F < p2.F ;
}
ll converter( string a ){
    ll i , mul = 1 , r , t ,ans = 0LL;if( a.length() == 0 )return 0;for( i = a.length() - 1 ; i >= 0 ; i-- ){
        t = a[i] - '0';r = t%10;ans += (mul*r);mul = mul*10;
    }
    return ans;
}
int msb(unsigned x) {
	union { double a; int b[2]; };
	a = x;
	return (b[1] >> 20) - 1023;
}

//
//debug
#ifdef RIP
template < typename F, typename S >
ostream& operator << ( ostream& os, const pair< F, S > & p ) {
            return os << "(" << p.first << ", " << p.second << ")";
}

template < typename T >
ostream &operator << ( ostream & os, const vector< T > &v ) {
            os << "{";
                for(auto it = v.begin(); it != v.end(); ++it) {
                                if( it != v.begin() ) os << ", ";
                                        os << *it;
                                            }
                    return os << "}";
}

template < typename T >
ostream &operator << ( ostream & os, const set< T > &v ) {
            os << "[";
                for(auto it = v.begin(); it != v.end(); ++it) {
                                if( it != v.begin() ) os << ", ";
                                        os << *it;
                                            }
                    return os << "]";
}

template < typename T >
ostream &operator << ( ostream & os, const multiset< T > &v ) {
            os << "[";
                for(auto it = v.begin(); it != v.end(); ++it) {
                                if( it != v.begin() ) os << ", ";
                                        os << *it;
                                            }
                    return os << "]";
}

template < typename F, typename S >
ostream &operator << ( ostream & os, const map< F, S > &v ) {
            os << "[";
                for(auto it = v.begin(); it != v.end(); ++it) {
                                if( it != v.begin() ) os << ", ";
                                        os << it -> first << " = " << it -> second ;
                                            }
                    return os << "]";
}

#define dbg(args...) do {cerr << #args << " : "; faltu(args); } while(0)

void faltu () {
            cerr << endl;
}

template <typename T>
void faltu( T a[], int n ) {
            for(int i = 0; i < n; ++i) cerr << a[i] << ' ';
                cerr << endl;
}

template <typename T, typename ... hello>
void faltu( T arg, const hello &... rest) {
            cerr << arg << ' ';
                faltu(rest...);
}
#else
#define dbg(args...)
#endif  //RIP

const int MAX = (int)1e5+ 5;

ll dp[MAX] , pwsum[MAX] , cnt[MAX];

int main() {
    optimize();
    //fraction();
    //freopen("input.txt","r",stdin) ;

    ll i , sum = 1  , n , q ;

    string s;
    cin >> n >> q >> s;

    dp[1] = 1;

    for(i = 2 ; i < MAX ; ++i){
           sum = (sum*2)%MOD;
           dp[i] = sum;
           dp[i] += dp[i-1];
           dp[i] %= MOD;
    }
    for(i = 1 ; i <= n; ++i){
        cnt[i] = (s[i-1] == '1') ;
        cnt[i] += cnt[i-1];
    }

    for(i = 1 ; i <= q ; ++i){
        int l  , r ;
        cin >> l >> r;
        ll one = cnt[r] - cnt[l - 1];
        ll zero = (r - l + 1 ) - one;
        zero = (( zero * ( zero + 1))/2 );
        zero = zero % MOD;
        ll cur1 = dp[one];
        ll cur2 = (zero*(dp[one]))%MOD;
        cout << (cur1 + cur2)%MOD << endl;
    }






    return 0;
}






