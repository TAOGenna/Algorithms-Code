#pragma GCC optimize ("Ofast")
#pragma GCC target ("avx,avx2")
#pragma GCC optimize ("trapv")
 
 
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/trie_policy.hpp>
 
using namespace std;
using namespace __gnu_pbds;
 
#define fastio ios_base::sync_with_stdio(0); cin.tie(0); cin.exceptions(cin.failbit);
#define rep(i, a, b) for(lli i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define ff first
#define ss second
#define mp make_pair
#define pb push_back
#define all(x) (x).begin(), (x).endd()
#define sz(x) (lli)(x).size()
#define what_is(x) cerr << #x << " is " << x << enddl;
#define wis cout<<enddl<<"I already speak english, bitch"<<enddl<<enddl;
 
typedef long long int lli;
typedef vector<lli> vi;
typedef pair<lli,lli> ii;
typedef vector<ii> vii;
typedef pair<ii,lli> iii;
 
#define trace(args...) { string  _s =#args; replace(_s.begin(), _s.end(), ',',' '); stringstream _ss(_s); istream_iterator<string> _it(_ss); err(_it, args);}
void err(istream_iterator<string> it){}
template<typename T, typename... Args>
void err(istream_iterator<string> it, T a, Args... args){
    cout <<  *it  << " : " << a << endl;
    err(++it, args...);
}
 
template <typename T, class compare>
using ordered_set = tree<T, null_type, compare, rb_tree_tag, tree_order_statistics_node_update>;
//usage: ordered_set< el tipo , el comparador> nombre;


const int N=1e3+10;
vector<int> g[N],arti;
int n;
bool vis[N],is_art[N],vis_aux[N];
set<int> graph[N];

////////art points stuff///////////
bool visited[N];
int tin[N],low[N];
int timer;

void IS_CUTPOINT(int node){
	arti.pb(node);
	is_art[node]=true;
}

void dfs(int v, int p = -1) {
	if(sz(g[v])==1) IS_CUTPOINT(v);
    visited[v] = true;
    tin[v] = low[v] = timer++;
    int children=0;
    for (int to : g[v]) {
        if (to == p) continue;
        if (visited[to]) {
            low[v] = min(low[v], tin[to]);
        } else {
            dfs(to, v);
            low[v] = min(low[v], low[to]);
            if (low[to] >= tin[v] && p!=-1)
                IS_CUTPOINT(v);
            ++children;
        }
    }
    if(p == -1 && children > 1)
        IS_CUTPOINT(v);
}

void find_cutpoints() {
    timer = 0;
    rep(i,1,n+1) tin[i]=-1,low[i]=-1;
    for (int i = 1; i <= n; ++i) {
        if (!visited[i])
            dfs(i);
    }
}
/////////////////////////////////////////////////

struct UF{
  int p[N];
  void clear(){ rep(i,0,N) p[i] = i; }
  int find(int i){ return (p[i] == i) ? i : (p[i] = find(p[i])); }
  void join(int i, int j){ p[find(i)] = find(j); }
}dsu;

//////////////////////////////////////////////////
void ot_dfs(int node,int art_p){
	if(vis[node]) return;
	vis[node]=1;
	if(is_art[node] and node!=art_p){
		//cmp[node].pb(art_p);
		//cmp[art_p].pb(node);
		return;
	}
	for(auto x : g[node]){
		ot_dfs(x,art_p);
		if(!is_art[x] and !is_art[node]){
			dsu.join(x,node);
		}
	}
}

void global_union(int node){
	if(vis_aux[node]) return;
	vis_aux[node]=1;
	for(auto x : g[node]) global_union(x);
}

/////////////////////////////////////////////////


void build_graphs(void){
	find_cutpoints();
	dsu.clear();
	for(auto x : arti) ot_dfs(x,x);
	for(int i=1;i<=n;i++) if(!vis[i]) ot_dfs(i,-1);

	for(auto x : arti){
		for(auto y : g[x]){
			int num=dsu.find(y);
			graph[x].insert(num);
			graph[num].insert(x);
		}
	}
	vector<int> loc;
	for(int i=1;i<=n;i++){
		if(!vis_aux[i]){
			global_union(i);
			loc.pb(dsu.find(i));
		}
	}
	rep(i,0,sz(loc)-1){
		graph[loc[i]].insert(loc[i+1]);
		graph[loc[i+1]].insert(loc[i]);
	}
}

//////////////////////////////////////////////////////

// 				LCA
const int LG=23;
int dep[N],p[N][LG],pai[N],counter=0;
map<int,int> inde;
vector<int> transver;

void _dfs(int node,int father,int prof){

	transver.pb(transver[inde[father]]+((is_art[node])?1:0));
	inde[node]=counter++;

	dep[node]=prof;
	for(auto x : graph[node]){
		if(x==father) continue;
		_dfs(x,node,prof+1);
		pai[x]=node;
	}
}

int __lca(int a, int b){
	if(dep[a] < dep[b]) swap(a,b);
	for(int j = LG-1; j >= 0; --j)
		if( dep[a]-(1<<j) >= dep[b] )
			a = p[a][j];
	if(a == b) return a;
	for(int j = LG-1; j >= 0; --j)
		if(p[a][j] != p[b][j])
			a = p[a][j], b = p[b][j];
	return pai[a];
}

void build(){
	for(int i = 1; i <= n; ++i)
		for(int j = 0; j < LG; ++j)
			p[i][j] = -1;
	
	for(int i = 1; i <= n; ++i)
		p[i][0] = pai[i];
	
	for(int j = 1; j < LG; ++j)
		for(int i = 1; i <= n; ++i)
			if(p[i][j-1] != -1)
				p[i][j] = p[p[i][j-1]][j-1];
}

int query(int a,int b){
	int anc=__lca(a,b);
	return transver[inde[a]]+transver[inde[b]]-2*transver[inde[anc]];
}

///////////////////////////////////////////////

int main(){
	fastio;

	int m; cin>>n>>m;
	rep(i,0,m){
		int a,b; cin>>a>>b;
		g[a].pb(b);
		g[b].pb(a);
	}
	build_graphs();
	for(int i=1;i<=n;i++){
		cout<<i<<" -> "<<dsu.p[i]<<endl;
	}
	cout<<endl<<"these are the articulation points"<<endl;
	for(auto x : arti) cout<<x<<" "; cout<<endl;

	cout<<"grafo comprimido( se supone que debe ser un TREE )"<<endl;
	for(int i=1;i<=n;i++){
		if(sz(graph[i])){
			cout<<i<<" -> ";
			for(auto x : graph[i]) cout<<x<<" ";cout<<endl;
		}
	}
	//_dfs(1,-1,0);
	//build();
	/*int q; cin>>q;
	while(q--){
		int a,b; cin>>a>>b;
		cout<<query(a,b)<<endl;
	}*/

	return 0;
}
