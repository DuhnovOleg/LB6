#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>
#include <stack>
#include <queue>
using namespace std;

typedef struct Node
{
	int vertex;
	Node* Next;
};

typedef struct Graf
{
	int num;
	Node** Array;
};

Node* init_node(int meaning)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->vertex = meaning;
	node->Next = NULL;
	return node;
}

Graf* init_graf(int n)
{
	Graf* graf = (Graf*)malloc(sizeof(Graf));
	graf->num = n;
	graf->Array = (Node**)malloc(n * sizeof(Node*));
	for (int i = 0; i < n; i++)
		graf->Array[i] = NULL;
	return graf;
}

void add_spisok(int i, int j, Graf* graf)
{
	Node* NewNode = init_node(j);
	if (graf->Array[i] == NULL)
	{
		graf->Array[i] = NewNode;
		NewNode = NULL;
	}
	Node* temp = graf->Array[i];
	while (temp->Next != NULL)
		temp = temp->Next;
	temp->Next = NewNode;
	NewNode = init_node(i);
	if (graf->Array[j] == NULL)
	{
		graf->Array[j] = NewNode;
		NewNode = NULL;
	}
	temp = graf->Array[j];
	while (temp->Next != NULL)
		temp = temp->Next;
	temp->Next = NewNode;
}

void print(Graf* graf)
{
	for (int i = 0; i < graf->num; i++)
	{
		Node* temp = graf->Array[i];
		printf("%d - ая вершина: ", i + 1);
		while (temp != NULL)
		{
			printf("%d ", temp->vertex + 1);
			temp = temp->Next;
		}
		printf("\n");
	}
}

int** create_arr(int n)
{
	int s = 1;
	int** Arr;

	Arr = (int**)malloc(n * sizeof(int*));
	srand(time(NULL));
	for (int i = 0; i < n; i++)
	{
		Arr[i] = (int*)malloc(n * sizeof(int));
		Arr[i][i] = 0;

		for (int j = s; j < n; j++)
		{
			if (rand() % 100 > 60)
			{
				Arr[i][j] = 0;
			}
			else {
				Arr[i][j] = 1;
			}
		}
		s++;
	}
	s = 1;
	for (int i = 0; i < n; i++)
	{
		for (int j = s; j < n; j++)
			Arr[j][i] = Arr[i][j];
		s++;
	}

	for (int i = 0; i < n; i++)
		printf("V%d ", i + 1);

	for (int i = 0; i < n; i++)
	{
		printf("\n");
		for (int j = 0; j < n; j++)
		{
			printf("%2d ", Arr[i][j]);
		}
	}
	return Arr;
}

void BFSD(int n, int* dist, int x, int** Arr)
{
	queue<int> que;
	que.push(x);
	dist[x] = 0;
	while (!que.empty())
	{
		x = que.front();
		que.pop();
		printf("%d ", x + 1);
		for (int i = 0; i < n; i++)
		{
			if (Arr[x][i] == 1 && dist[i] == -1)
			{
				que.push(i);
				dist[i] = dist[x] + 1;
			}
		}
	}
}

void DFS(int n, int** Arr, int x, int* dist, int k)
{
	dist[x] = k;
	printf("%d ", x + 1);
	for (int i = 0; i < n; i++)
	{
		if (Arr[x][i] == 1 && dist[i] == -1)
			DFS(n, Arr, i, dist, k + 1);	
		if (Arr[x][i] == 1 && dist[i] > k)
			DFS(n, Arr, i, dist, k + 1);
	}
}

void bfsd_spisok(Graf* graf, int n, int x, int* dist)
{
	queue<int> que;
	que.push(x);
	dist[x] = 0;
	while (!que.empty())
	{
		x = que.front();
		que.pop();
		printf("%d ", x + 1);
		Node* temp = graf->Array[x];
		while (temp)
		{
			if (dist[temp->vertex] == -1)
			{
				que.push(temp->vertex);
				dist[temp->vertex] = dist[x] + 1;
			}
			temp = temp->Next;
		}
	}
}

void dfs_spisok(Graf* graf, int* dist, int x, int k)
{
	dist[x] = k;
	printf("%d ", x + 1);
	Node* temp = graf->Array[x];
	while (temp)
	{
		if (dist[temp->vertex] == -1)
			dfs_spisok(graf, dist, temp->vertex, k + 1);	
		if (dist[temp->vertex] > k)
			dfs_spisok(graf, dist, temp->vertex, k + 1);
		temp = temp->Next;
	}
}

int main()
{
	setlocale(LC_ALL, "Ru");
	clock_t start, stop, st, end;
	int n, x, ** t;


	printf("Введите размер массива: ");
	scanf("%d", &n);
	t = create_arr(n);
	printf("\nВведите, из какой вершины вы хотите начать: ");
	scanf("%d", &x);
	while ((x <= 0) || (x > n))
	{
		printf("Такой вершины нет! Введите другую вершину: ");
		scanf("%d", &x);
	}
	x--;
	printf("\n\n");


	int* dist = (int*)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++)
		dist[i] = -1;
	printf("\nРезультат обхода в ширину: ");
	start = clock();
	BFSD(n, dist, x, t);
	stop = clock();
	float res = stop - start;
	for (int i = 0; i < n; i++)
	{
		if (dist[i] != -1)
			printf("\nРасстояние до %d - ой вершины: %d ", i + 1, dist[i]);
		else
			printf("\nДо вершины %d - нет расстояния", i + 1);
	}
	printf("\n\ntime in ms: %f", res);


	Graf* graf = init_graf(n);
	printf("\n");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (t[i][j] == 1 && i > j)
				add_spisok(i, j, graf);
		}
	}


	for (int i = 0; i < n; i++)
		dist[i] = -1;
	printf("\nРезультат обхода списком в ширину: ");
	start = clock();
	bfsd_spisok(graf, n, x, dist);
	stop = clock();
	float resul = stop - start;
	for (int i = 0; i < n; i++)
	{
		if (dist[i] != -1)
			printf("\nРасстояние до %d - ой вершины: %d ", i + 1, dist[i]);
		else
			printf("\nДо вершины %d - нет расстояния", i + 1);
	}
	printf("\n\ntime in ms: %f", resul);
	printf("\n\n");


	for (int i = 0; i < n; i++)
		dist[i] = -1;
	printf("\nРезультат обхода в глубину: ");
	start = clock();
	DFS(n, t, x, dist, 0);
	stop = clock();
	float result = stop - start;
	for (int i = 0; i < n; i++)
	{
		if (dist[i] != -1)
			printf("\nРасстояние до %d - ой вершины: %d ", i + 1, dist[i]);
		else
			printf("\nДо вершины %d - нет расстояния", i + 1);
	}
	printf("\n\ntime in ms: %f", result);
	printf("\n\n");


	for (int i = 0; i < n; i++)
		dist[i] = -1;
	printf("\nРезультат обхода в глубину списком: ");
	start = clock();
	dfs_spisok(graf, dist, x, 0);
	stop = clock();
	float resultat = stop - start;
	for (int i = 0; i < n; i++)
	{
		if (dist[i] != -1)
			printf("\nРасстояние до %d - ой вершины: %d ", i + 1, dist[i]);
		else
			printf("\nДо вершины %d - нет расстояния", i + 1);
	}
	printf("\n\ntime in ms: %f", resultat);


	_getch();
}
