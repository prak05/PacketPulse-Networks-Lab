#include <stdio.h>
#define INF 999
#define MAX 10
int cost[MAX][MAX], dist[MAX], prev[MAX], visited[MAX], src;
void dijkstra(int n);
void print_path(int node);
int main() {
int num_nodes, i, j;
printf("Enter number of nodes: ");
scanf("%d", &num_nodes);
printf("Enter the source node: ");
scanf("%d", &src);
printf("Enter cost matrix (999 for INF):\n");
for (i = 0; i < num_nodes; i++)
for (j = 0; j < num_nodes; j++)
scanf("%d", &cost[i][j]);
dijkstra(num_nodes);
return 0;}
void dijkstra(int n) {
int i, j, u, min_dist;
for (i = 0; i < n; i++) {
dist[i] = cost[src][i];
visited[i] = 0;
prev[i] = (dist[i] < INF) ? src : -1;}
dist[src] = 0;
visited[src] = 1;
for (i = 1; i < n; i++) {
min_dist = INF;
u = -1;
for (j = 0; j < n; j++) {
if (!visited[j] && dist[j] < min_dist) {
min_dist = dist[j];
u = j;}}
if (u == -1) break;
visited[u] = 1;
for (j = 0; j < n; j++) {
if (!visited[j] && (dist[u] + cost[u][j] < dist[j])) {
dist[j] = dist[u] + cost[u][j];
prev[j] = u;}}}
printf("\nResults:\n");
for (i = 0; i < n; i++) {
if (i != src) {
printf("To %d: ", i);
if (dist[i] == INF) printf("No Path");
else { print_path(i); printf(" [Dist: %d]", dist[i]); }
printf("\n");}}}
void print_path(int node) {
if (node == src) { printf("%d", src); return; }
if (prev[node] == -1) { printf("No path"); return; }
print_path(prev[node]);
printf("->%d", node);}
