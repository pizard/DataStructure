#include <stdio.h>
#include <stdlib.h>
#define MAX_VERT 100 //최대 정점의 개수
typedef struct node *node_pointer;
typedef struct node {
	int vertex;
	int dur;
	node_pointer link;
} node;
typedef struct {
	int count;
	node_pointer link;
} node2;
node2 graph[MAX_VERT];
node2 reverse_graph[MAX_VERT];
int earliest[MAX_VERT];
int latest[MAX_VERT];
int stack[MAX_VERT];
int top = -1;

int Insert(node2[], int vertex1, int vertex2, int dur);
int Print(node2[], int vertex1, int check);
void Topsort(node2[], int n, int check);
void Reverse(int n);
void Critical_Print(node2[], int n);
void Push(int value);
int Pop();
int main()
{
	int start_vertex, end_vertex, duration;
	int max_vertex = 0;
	printf("AOE network \n");
	printf("입력이 끝났다면 시작 정점에 -1를 입력하세요. \n");
	while (1){
		printf("시작 정점, 도착 정점, Path의 길이 순으로 입력해 주세요.\n");
		scanf_s("%d", &start_vertex);
		if (start_vertex == -1)
			break;
		scanf_s("%d %d", &end_vertex, &duration);
		if (end_vertex <= start_vertex)
			printf("\n잘못된 값을 입력했습니다. 다시 입력해 주세요.\n");
		else if (max_vertex < end_vertex){
			max_vertex = end_vertex;
			Insert(graph, start_vertex, end_vertex, duration);
		}
		else {
			Insert(graph, start_vertex, end_vertex, duration);
		}
	}
	Print(graph, max_vertex, 0); //인접리스트를 출력합니다.
	Topsort(graph, max_vertex + 1, 0); //인접리스트를 정렬합니다.
	Reverse(max_vertex); // 역인접 리스트 생성합니다.
	Topsort(reverse_graph, max_vertex + 1, 1); //역인접 리스트를 정리합니다.
	Critical_Print(graph, max_vertex); //임계성 여부 출력합니다.
	return 0;
}

int Insert(node2 graph[], int vertex1, int vertex2, int dur) //그래프를 입력하는 함수입니다.
{
	node_pointer element, adjlist;
	element = (node_pointer)malloc(sizeof(node));
	graph[vertex2].count = graph[vertex2].count + 1;
	if (graph[vertex1].link != NULL){
		for (element = graph[vertex1].link; element->link != NULL; element = element->link)
			if (element->vertex == vertex2){ // 같은 정점이 있는 경우
			printf("같은 vertex가 존재합니다.\n");
			exit(1);
			}
		adjlist = (node_pointer)malloc(sizeof(node));
		adjlist->vertex = vertex2;
		adjlist->dur = dur;
		adjlist->link = NULL;
		element->link = adjlist;
	}
	else { //첫번째 입력인 경우
		adjlist = (node_pointer)malloc(sizeof(node));
		adjlist->vertex = vertex2;
		adjlist->dur = dur;
		adjlist->link = NULL;
		graph[vertex1].link = adjlist;
	}
	return 0;
}
void Reverse(int n) //역인접리스트 정렬 함수
{
	int i;
	node_pointer ptr;
	ptr = (node_pointer)malloc(sizeof(node));
	for (i = 0; i < n; i++){
		for (ptr = graph[i].link; ptr; ptr = ptr->link) {
			Insert(reverse_graph, ptr->vertex, i, ptr->dur);
		}
	}
}
void Topsort(node2 graph[], int n, int check)
{
	int i, j, k, l;
	node_pointer ptr;

	if (check == 0){
		printf("\nEarliest Calculation \n");
		for (i = 0; i<n; i++){
			earliest[i] = 0;
			printf(" [%d]", i);
		}
	}
	else {
		for (i = 0; i<n; i++){
			latest[i] = earliest[n - 1];
		}
	}
	if (check ==0)
	printf(" [stack] \n 초 기 : ");
	for (i = 0; i<n; i++) //선행자가 없는 정점들의 스택을 생성 합니다.
		if (!graph[i].count)
			Push(i);
	for (i = 0; i < n; i++) {
		if (check == 0){
			for (l = 0; l < n; l++)
				printf(" %2d ", earliest[l]);
			for (l = top; l >= 0; l--)
				printf(" [%d]", stack[l]);
			printf("\n");
		}


		j = Pop();
		if (check == 0) printf("V%d출력 : ", j);
		for (ptr = graph[j].link; ptr; ptr = ptr->link) {
			k = ptr->vertex;
			graph[k].count--;
			if (!graph[k].count)
				Push(k);
			if (check == 0){
				if (earliest[k] < earliest[j] + ptr->dur)
					earliest[k] = earliest[j] + ptr->dur;
			}
			else{
				if (latest[k] > latest[j] - ptr->dur)
					latest[k] = latest[j] - ptr->dur;
			}
		}
	}
}void Push(int value) { //스택에 값을 저장하는 함수
	stack[++top] = value;
}
int Pop(){ //스택에서 값을 빼네는 함수
	return stack[top--];
}
int Print(node2 graph[], int max_vertex, int check) //AOE네트워크의 리스트를 출력함
{
	int i;
	node_pointer ptr;
	ptr = (node_pointer)malloc(sizeof(node));
	if (check == 0)
		printf("\n\n인접 리스트\n");

	printf("정점:단계 정점:거리\n");
	for (i = 0; i <= max_vertex; i++) /* 인접 리스트에 연결된 노드 모드 출력 */
	{
		printf(" %2d : %2d -> ", i, graph[i].count);
		for (ptr = graph[i].link; ptr; ptr = ptr->link) {
			printf(" %d : %d , ", ptr->vertex, ptr->dur);
		}
		printf("\n");
	}
	return 0;
}
void Critical_Print(node2 graph[], int n) //임계성 여부를 확인해서 출력합니다.
{
	int i, early, late, cnt = 0;
	node_pointer ptr;
	ptr = (node_pointer)malloc(sizeof(node));
	printf("\n\n작업 시작 정점 도착 정점 Late-Early check \n");
	for (i = 0; i < n; i++) {
		for (ptr = graph[i].link; ptr; ptr = ptr->link) {
			early = earliest[i];
			late = latest[ptr->vertex] - ptr->dur;
			printf("a[%d] %6d %8d %9d %9s \n", cnt, early, late, late - early, (late - early == 0) ? "YES" : "NO");
			cnt++;
		}
	}
}