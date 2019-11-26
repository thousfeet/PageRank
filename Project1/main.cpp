#include <fstream>
#include<iostream>
#include<cstdio>
#include <iomanip>
#include<algorithm>
#include<stack>
#include<vector>
using namespace std;

#define MAXN 1007
#define MAXM 10007
int N, M;

int head[MAXN];
int nex[MAXM];
int to[MAXM];
int cur = 0;
void addEdge(int u, int v) {
    to[++cur] = v;
    nex[cur] = head[u];
    head[u] = cur;
}

int dfn[MAXN];
int low[MAXN];
bool vis[MAXN];
stack<int> sta;
vector<int> SCC;
bool isSCC[MAXN];
int step = 0;
void tarjan(int u) {
    vis[u] = 1;
    sta.push(u);
    dfn[u] = low[u] = ++step;
    for (int i = head[u]; ; i = nex[i]) {
        if (i == 0) break;
        int v = to[i];
        if (!dfn[v]) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
        }
        else {
            if (vis[v]) low[u] = min(low[u], dfn[v]);
        }
    }

    if (dfn[u] == low[u]) {
        int now = sta.top();
        vis[u] = 0;
        if (now == u) sta.pop();
        else { //找到大SCC
            SCC.push_back(u);
            isSCC[u] = 1;
            while (now != u) {
                SCC.push_back(now);
                isSCC[now] = 1;
                sta.pop();
                vis[now] = 0;
                now = sta.top();
            }
            sta.pop();
        }
    }
}

double ran[MAXN][2]; //pageRank值
int out[MAXN]; //在SCC中重算出度
int old = 0;
int now = 1;
void pageRank() {
    int len = SCC.size();
    for (int i = 0; i < len; i++) {
        int u = SCC[i];
        int cnt_u = 0;
        for (int c = head[u]; c != 0; c = nex[c]) {
            int v = to[c];
            if (isSCC[v]) cnt_u++;
        }
        out[u] = cnt_u;
    }

    //--init
    for (int i = 0; i < len; i++) {
        int u = SCC[i];
        ran[u][old] = 1 / (double)len;
    }

    //--work
    double eps = 1; //两次迭代间的差值
    while (eps > 1e-9) {
        //clear 
        for (int i = 0; i < N; i++) 
            ran[i][now] = 0;
        
        //update
        for (int i = 0; i < len; i++) {
            int u = SCC[i];
            for (int c = head[u]; c != 0; c = nex[c]){
                int v = to[c];
                if (isSCC[v]) {
                    ran[v][now] += ran[u][old] / out[u];
                }
            }
        }

        eps = 0; 
        for (int i = 0; i < len; i++) {
            int u = SCC[i];
            eps = max(eps, abs(ran[u][now] - ran[u][old]));
        }
            
        old = now;
        now = (now + 1) % 2;
    }    
}

int main() {
    ifstream myfile;
    myfile.open("xx.in");
    myfile >> N >> M;
    for (int i = 0; i < M; i++) {
        int a, b; myfile >> a >> b;
        addEdge(a, b);
    }

    //tarjan寻找SCC
    for (int i = 0; i < N; i++) {
        if (!dfn[i]) tarjan(i);
    }

    pageRank();
    int K; myfile >> K;
    while(K--) {
        int num; myfile >> num;
        if (!isSCC[num]) cout << "None" << endl;
        else cout << fixed << setprecision(5) << ran[num][now] << endl;
    }
    return 0;
}