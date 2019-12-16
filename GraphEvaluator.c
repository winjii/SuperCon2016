#ifndef _GLOBAL
#define _GLOBAL
#include "./Global.c"
#endif

#ifndef _GRAPHGENERATOR
#define _GRAPHGENERATOR
#include "./GraphGenerator.c"
#endif

#define INF (1001001001)

void GetDistance(int s,int N,int graph[MAX_N][MAX_D],int color[MAX_N],int dist[MAX_N]){
	for(int i=0;i<N;i++)dist[i]=INF;
	dist[s]=0;

	int queue[10][MAX_N],queue_size[10]={};
	queue_size[0]=1;
	queue[0][0]=s;
	
	int r=0;
	for(int i=0;i<=r;i++){
		int m=i%10;
		for(int j=0;j<queue_size[m];j++){
			int v=queue[m][j];
			if(dist[v]<i)continue;
			for(int k=0;k<D;k++){
				int u=graph[v][k];
				int d=dist[v]+W[color[v]][color[u]];
				if(dist[u]>d){
					dist[u]=d;
					queue[d%10][queue_size[d%10]++]=u;
					if(r<d)r=d;
				}
			}
		}
		queue_size[m]=0;
	}
}

void GetAllDistance(int N,int graph[MAX_N][MAX_D], int color[MAX_N], int dist[MAX_N][MAX_N]){

	for(int i=0;i<N;i++)GetDistance(i,N,graph,color,dist[i]);
}

int  ASPL(int N,int graph[MAX_N][MAX_D],int color[MAX_N]){
	int aspl=0;
	int dist[MAX_N];
	for(int i=0;i<N;i++){
		GetDistance(i,N,graph,color,dist);
		for(int j=0;j<N;j++)aspl+=dist[j];
	}
	return aspl/2;
}

//Union Find Tree
int uf_par[MAX_N];
int uf_find(int x){
	return x==uf_par[x]?x:(uf_par[x]=uf_find(uf_par[x]));
}
void uf_unite(int x,int y){
	uf_par[uf_find(y)]=uf_find(x);
}

int IsConnected(int N,int graph[MAX_N][MAX_D]){
	for(int i=0;i<N;i++)uf_par[i]=i;
	for(int i=0;i<N;i++){
		for(int j=0;j<D;j++){
			uf_unite(i,graph[i][j]);
		}
	}

	for(int i=0;i<N;i++)if(uf_find(i)!=uf_find(0))return 0;
	return 1;
}

int IsValid(int N,int graph[MAX_N][MAX_D],int color[MAX_N]){
	for(int i=0;i<N;i++){
		for(int j=0;j<D;j++){
			if(graph[i][j]<0||graph[i][j]>=N){
				puts("out of [0,N)");
				//abort();	
				return 0;
			
			}
			for(int k=0;k<D;k++){
				if(j!=k&&graph[i][j]==graph[i][k]){
					puts("not distinct");
					//abort();
					return 0;
				}
			}
		}
	}

	if(!IsConnected(N,graph)){
		puts("disconnected");
		//abort();	
		return 0;
	}

	int cnt[MAX_C]={0};
	for(int i=0;i<N;i++)cnt[color[i]]++;
	for(int i=0;i<C;i++){
		if(cnt[i]!=M[i]){
			puts("color error");
			//abort();
			return 0;
		}
	}

	return 1;//ok
}