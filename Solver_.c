#define ANSWER_FILE "answer.txt"

#ifndef _GLOBAL
#define _GLOBAL
#include "./Global.c"
#endif

#ifndef _GRAPHEVALUATOR
#define _GRAPHEVALUATOR
#include "./GraphEvaluator.c"
#endif

#ifndef _RANDOMMANAGER
#define _RANDOMMANAGER
#include "./RandomManager.c"
#endif

int answerASPL = INT_MAX;
double destroyedProb = 0.188;

void SaveConfig()
{
	FILE *fp = fopen("config.txt", "w");
	if (fp == NULL)
	{
		puts("Config open failed.");
		return;
	}
	fprintf(fp, "%.16f\n", destroyedProb);
	fclose(fp);
}

void LoadConfig()
{
	FILE *fp = fopen("config.txt", "r");
	if (fp == NULL)
	{
		puts("Config is not found. file is made.");
		SaveConfig();
		return;
	}
	double temp = destroyedProb;
	if (fscanf(fp, "%lf", &destroyedProb) == EOF || destroyedProb < 0 || 1 < destroyedProb)
	{
		puts("Config is invalid.");
		destroyedProb = temp;
		fclose(fp);
		return;
	}
	fclose(fp);
	double abs = temp - destroyedProb;
	if (abs < 0) abs *= -1;
	if (abs > 1e-8) puts("Config is updated");
}

void Save(int graph[MAX_N][MAX_D])
{
	if (IsValid(N, graph, Color) == 0)
	{
		puts("Answer is invalid.");
		return;
	}
	int ret = ASPL(N, graph, Color);
	if (answerASPL > ret)
	{
		answerASPL = ret;
		printf("Answer is updated!!(ASPL = %d)\n", ret);
	}
	else return;
	char str[20] = ANSWER_FILE;
	
	FILE *fp = fopen(str, "w");
	if (fp == NULL)
	{
		puts("File open failed.");
		return;
	}
	int cnt = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < D; j++)
		{
			if (i >= graph[i][j]) continue;
			fprintf(fp, "%d %d\n", i, graph[i][j]);
			cnt++;
		}
	}
	fclose(fp);
	if (cnt != N*D/2)
	{
		puts("few edges.");
		return;
	}
}

void Load(int graph[MAX_N][MAX_D])
{
	char str[20] = ANSWER_FILE;
	int size[MAX_N] = {};
	FILE *fp = fopen(str, "r");
	if (fp == NULL)
	{
		puts("File is not found. Graph is generated.");
		generate_random_graph(N, D, graph);
		return;
	}
	for (int i = 0; i < N*D/2; i++)
	{
		int a, b;
		if (fscanf(fp, "%d%d", &a, &b) == EOF)
		{
			puts("Answer is invalid. Graph is generated.");
			generate_random_graph(N, D, graph);
			fclose(fp);
			return;
		}
		graph[a][size[a]++] = b;
		graph[b][size[b]++] = a;
	}
	fclose(fp);
	puts("Answer was loaded.");
}

int TrySwapEdge(int graph[MAX_N][MAX_D], int u, int a, int v, int b)
{
	if (graph[u][a] == v || graph[v][b] == u || graph[u][a] == graph[v][b])
		return 0;
	for (int i = 0; i < D; i++)
	{
		if (graph[u][i] == graph[v][b]) return 0;
	}
	for (int i = 0; i < D; i++)
	{
		if (graph[v][i] == graph[u][a]) return 0;
	}
	int c, d;
	for (int i = 0; i < D; i++)
	{
		if (graph[graph[u][a]][i] == u) { c = i; break; }
	}
	for (int i = 0; i < D; i++)
	{
		if (graph[graph[v][b]][i] == v) { d = i; break; }
	}
	Swap(graph[graph[u][a]] + c, graph[graph[v][b]] + d);
	Swap(graph[u] + a, graph[v] + b);
	return 1;
}

long long EvaluateVertex(int n, int graph[MAX_N][MAX_D], int s)
{
	int dist[MAX_N];
	GetDistance(s, N, graph, Color, dist);
	long long res = 0;
	for (int i = 0; i < N; i++)
	{
		if (dist[i] > N) return LLONG_MAX/2;
		res += dist[i]*dist[i];
	}
	return res;
}

int Search(int graph[MAX_N][MAX_D])
{
	int loop = 10000;
	long long allCount = 0;
	int graphCopy[MAX_N][MAX_D] = {}, asplCopy = INT_MAX/2;
	Save(graph);
	while (1)
	{
		LoadConfig();
		for (int step = 0; step < 10; step++)
		{
			int ret = ASPL(N, graph, Color);
			if (ret - asplCopy > 200)
			{
				for (int i = 0; i < N; i++) for (int j = 0; j < D; j++)
					graph[i][j] = graphCopy[i][j];
				ret = asplCopy;
			}
			else
			{
				for (int i = 0; i < N; i++) for (int j = 0; j < D; j++)
					graphCopy[i][j] = graph[i][j];
				asplCopy = ret;
			}
			printf("score = %d, prob = %f, allCount = %lld\n",
				ret, destroyedProb, allCount);
			for (int cnt = 0; cnt < loop; cnt++, allCount++)
			{
				long long score = 0;
				int r = Rand(0, N*D), s = Rand(0, N*D);
				int u = r/D, a = r%D, v = s/D, b = r%D;
				score +=
					EvaluateVertex(N, graph, u) + EvaluateVertex(N, graph, v) +
					EvaluateVertex(N, graph, graph[u][a]) + EvaluateVertex(N, graph, graph[v][b]);
				
				int isAble = TrySwapEdge(graph, u, a, v, b);
				
				score -=
					EvaluateVertex(N, graph, u) + EvaluateVertex(N, graph, v) +
					EvaluateVertex(N, graph, graph[u][a]) + EvaluateVertex(N, graph, graph[v][b]);
				
				if (score <= 0)
				{
					if (isAble) TrySwapEdge(graph, u, a, v, b);
					else
					{
						if (DoubleRand() > destroyedProb) TrySwapEdge(graph, u, a, v, b);
					}
				}
			}
		}
		Save(graph);
	}
	return ASPL(N, graph, Color);
}

int main()
{
	Input();
	puts("FinishedInput");
	puts("l a t t e");
	int graph[MAX_N][MAX_D];
	Load(graph);
	printf("%d\n", Search(graph));
	return 0;
}