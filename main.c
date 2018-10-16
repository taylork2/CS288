/******************************************************************************
CS288 HW7 three search strategies: depth, breadth, and intelligent
command example: command 16 numbers and search strategy

fifteen 1 2 3 4 5 6 7 8 9 10 11 12 13 14 0 15 {dfs|bfs|astar}
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BF 4			/* Branching factor of the search tree */
#define N 4
#define NxN 16
#define DFS 1                   /* depth first search */
#define BFS 2                   /* breadth first search */
#define BEST 3                  /* best first search */
#define BB 4                    /* branch and bound */
#define ASTAR 5                 /* A* search */
#define UNKNOWN 9		/* unknown search strategy */

#define MAX_PATH 1000

#define DN 0			/* for finding path */
#define RT 1
#define UP 2
#define LT 3
#define UK 9

#define FVAL 0			/* f=g+h, keep it simple for now */
#define GVAL 1
#define HVAL 2
#define PATH 3			/* array index, how it got to this state */

#define TRUE 1
#define FALSE 0

int level, strategy, path_idx;


struct node {
	int board[N + 1][N];
	struct node *next;
};


void print_a_node(struct node *np);
void find_path(struct node *cp, struct node *opnp, struct node *cldp);
void print_path(int n, int *path);

int toggle_dir(int dir);
int solvable(struct node *cp);
int find_h(int current[N + 1][N], int goalp[N + 1][N]);
int path[MAX_PATH], path_buf[MAX_PATH];
int count(struct node *cp);
void prep_dir_to_str();
char *dir_to_sdir[4], strategy_s[10]; /* four directions */

typedef struct node Node;

struct node *start, *goal;
struct node *initialize(int argc, char **argv);
struct node *prepend(struct node *tp, struct node *sp);

int nodes_same(struct node *xp, struct node *yp);
struct node *expand(struct node *cp);
struct node *merge(struct node *succ, struct node *open, int flg);
struct node *filter(struct node *succ, struct node *hp);
struct node *goal_found(struct node *cp, struct node *gp);
struct node *append(struct node *tp, struct node *sp);
struct node *prepend(struct node *tp, struct node *sp);
struct node *move(struct node *cp, int a, int b, int x, int y, int dir);

int main(int argc, char **argv)
{
	int iter, cnt = 0, total = 1, ocnt = 0, ccnt = 0;
	int is_solvable;
	struct node *cp, *open, *closed, *succ, *tp;

	cp = open = closed = succ = NULL;
	start = initialize(argc, argv);	/* init initial and goal states */
	is_solvable = solvable(start);		/* check if solvable permutation */
	printf("Solvable: %s\n", is_solvable ? "TRUE" : "FALSE");
	if (!is_solvable)	
		return 1;	

	//if (perm & 1) return 1; /*if odd, return false*/

	open = start;
	iter = 0;

	prep_dir_to_str();
	if (cp = goal_found(start, goal))
	{
		printf("Starting board is goal state\nDone\n");
		return 0 ;
	}
	

	while (open) {
		ocnt = count(open);
		ccnt = count(closed);
		printf("%d: open=%d + clsd=%d = total=%d\n", iter, ocnt, ccnt, ocnt + ccnt);

		cp = open;
		open = open->next; 
		cp->next = NULL; /* get the first node from open */
		//print_a_node(cp);

		succ = expand(cp);			     /* Find new successors */
		succ = filter(succ, open);		     /* New succ list */
		succ = filter(succ, closed);		     /* New succ list */

		cnt = count(succ);
		total = total + cnt;

		if (succ) 
			open = merge(succ, open, strategy); /* New open list */

		closed = append(cp, closed);		      /* New closed */

		if (succ && (cp = goal_found(succ, goal)))
			break;
		iter++;
	}
	find_path(cp, open, closed);

	printf("\nDone!");
	system("pause");
	//printf("%s strategy: %d iterations %d nodes\n", strategy_s, iter + 1, total);
	//find_path(cp, open, closed);

	return 0;
}


int toggle_dir(int dir) {
	int opp_dir = -1;
	switch (dir)
	{	
		case DN:
			opp_dir = UP;
			break;
		case RT:
			opp_dir =  LT;
			break;
		case LT:
			opp_dir = RT;
			break;
		case UP:
			opp_dir = DN;
			break;
		default:
			break;
	}
	return opp_dir;;
}

void print_a_node(struct node *np) {
	int i, j;
	for (i = 0; i<N + 1; i++) {
		for (j = 0; j<N; j++) printf("%2d ", np->board[i][j]);
		printf("\n");
	}
	printf("\n");
}

int count(struct node *cp) {
	int cnt = 0;

	while (cp)
	{
		cnt++;
		cp = cp->next;		
	}

	return cnt;
}

void print_path(int n, int *path) {
	int i, p;
	//  for (i=0;i<n;i++) path[i] = path_buf[path_cnt-i-1];
	//  for (i=0;i<path_cnt;i++) printf("%d ",path[i]); printf("\n");
	//  printf("entering print_path: n=%d\n",n);

	////  for (i=n-1;i>=0;i--) printf("%d ",*(path+i)); printf("\n");
	for (i = n - 1; i >= 0; i--) {
		p = *(path + i);
		if (i>0) printf("%s -> ", dir_to_sdir[p]);
		else printf("%s", dir_to_sdir[p]);
	}
	printf("\n");
	//  printf("exiting print_path\n");
}

//char **dir_to_sdir={"DN","RT","UP","LT"}; //[4];
void prep_dir_to_str() {
	dir_to_sdir[0] = "DN";
	dir_to_sdir[1] = "RT";
	dir_to_sdir[2] = "UP";
	dir_to_sdir[3] = "LT";
}

void * swap(struct node *cp, int i, int j, int k, int l) {
	int tmp;

	tmp = cp->board[i][j];
	cp->board[i][j] = cp->board[k][l];
	cp->board[k][l] = tmp;	
}

void find_parent(struct node *cp, int prev_dir, Node *closed) {
	int i, j, k, cnt, row = 0, col = 0;
	int flag;
	Node * closed_p = closed;

	for (i = 0; i <= N + 1; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (cp->board[i][j] == 0)
			{
				row = i; col = j;
				break;
			}
		}
		if (j < N) break;
	}

	switch (prev_dir)
	{
	case DN:
		swap(cp, row, col, row - 1, col); // swap with above tile
		break;
	case RT:
		swap(cp, row, col, row, col - 1); // swap with left tile
		break;
	case UP:
		swap(cp, row, col, row + 1, col); // swap with below tile
		break;
	case LT:
		swap(cp, row, col, row, col + 1); // swap with right tile
		break;
	}

	while (closed_p)
	{
		if (!nodes_same(closed_p, cp))
			closed_p = closed_p->next;
		else
			break;
	}

	for (i = N; i < N + 1; i++)
	{
		for (j = 0; j < N; j++)
		{
			cp->board[i][j] = closed_p->board[i][j];
		}
	}

}
void find_path(struct node *cp, struct node *opnp, struct node *cldp) {
	int i, j, dir, opp_dir;	
	int path_cnt = 0, p;
	struct node *tp = cp;

	dir = tp->board[N][3]; //direction
	opp_dir = toggle_dir(dir);

	while (!nodes_same(tp, start))
	{
		dir = tp->board[N][3];
		find_parent(tp, dir, cldp);
		path[path_cnt++] = dir;
		if (path_cnt >= MAX_PATH)
			break;
	}
	print_path(path_cnt, path);
}

struct node *expand(struct node *cp) {
	int i, j, k, cnt;
	Node *succ, *tp;

	succ = NULL;
	tp = NULL;

	/* check where 0 is. find indices i,j */
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++)
			if (cp->board[i][j] == 0) break;
		if (j<N) break;		/* found it */
	}

	if ((i + 1) < N) {		/* DOWN */
		tp = move(cp, i, j, i + 1, j, DN);
		succ = append(tp, succ);
	}
	if ((j + 1) < N) {		/* RIGHT */
		tp = move(cp, i, j, i, j + 1, RT);
		succ = append(tp, succ);
	}
	if ((i - 1) < N) {		/* UP */
		tp = move(cp, i, j, i - 1, j, UP);
		succ = append(tp, succ);
	}
	if ((j - 1) < N) {		/* LEFT */
		tp = move(cp, i, j, i, j - 1, LT);
		succ = append(tp, succ);
	}
	return succ;
}

/* attach tp to beginning of sp */
struct node *prepend(struct node *tp, struct node *sp) {
	//.....
	Node * cp = tp;
	if (!sp)
	{
		return tp;
	}
	while (cp->next)	
		cp = cp->next;
	cp->next = sp;
	
	return tp;
}

/* attach tp to the end of sp */
struct node *append(struct node *tp, struct node *sp) {
	Node * cp;
	cp = sp;

	if (!sp)
	{
		return tp;
	}
	while (cp->next) cp = cp->next;
	cp->next = tp;
	//.....
	return sp;
}


struct node *merge_smart(Node *succ, Node * open, int strategy)
{
	Node *csucc, *copen;	
	Node *temp_open;
	Node *prev;
	Node * insert; // node with lowest h
	Node * insert_prev; // node previous to insert

	csucc = prev = succ;
	copen = open;
	
	while (TRUE)
	{
		int max_f = -1; //prepend nodes in order of max to lowest f, so lowest is head

		csucc = succ;
		prev = csucc;
		insert = NULL;
		insert_prev = NULL;

		while (csucc)
		{
			int f;
			if(strategy == ASTAR)
				f = csucc->board[N][0];//index of f value
			else
				f = csucc->board[N][2];//index of h value

			if (f >= max_f)
			{
				max_f = f;
				insert = csucc;
				insert_prev = prev;
			}

			if (prev != csucc)
				prev = prev->next;

			csucc = csucc->next;
		}
		if (!insert)
			break;
		if (insert == insert_prev) // if insert node is head pointer, move hp to next node and insert
			succ = succ->next;
		else
			insert_prev->next = insert->next;

		if (copen)
		{			
			insert->next = NULL;
			copen = prepend(insert, copen);			
		}
		else
		{
			insert->next = NULL;
			copen = insert;			
		}
	}

	open = copen;
	return open;
}


struct node *merge(struct node *succ, struct node *open, int flg) {
	struct node *csucc, *copen;
	Node * next;

	csucc = succ;
	copen = open;

	if (flg == DFS) {	/* attach in the front: succ -> ... -> open */
		
		copen = prepend(csucc, copen);
		open = copen;
	}
	else if (flg == BFS) {	  /* attach at the end: open -> ... -> succ */		
		open = append(csucc, copen);		
	}
	else if (flg == BEST) {	/* Best first: sort on h value */		
		open = merge_smart(succ, open, BEST);
	}

	else {			/* A* search: sort on f=g+h value */
		open = merge_smart(succ, open, ASTAR);
	}
	return open;
}

/******************************************************************************
Check succ against open and closed. Remove those succ nodes that are in open or closed.
******************************************************************************/
struct node *filter(struct node *succ, struct node *hp) {
	struct node *lsp, *rsp;	/* lsp=left succ p, rsp=right succ p */
	struct node *tp;		/* temp ptr */
	
	Node *sp; /*succ ptr*/
	
	lsp = rsp = sp = succ;	//set all succ pointers to succ	

	while (sp)
	{		
		tp = hp;
		while (tp && !nodes_same(sp,tp))
		{
			tp = tp->next;
		}

		//if nodes are same, tp terminates before becoming null
		if (tp)
		{
			if (lsp == rsp) // if first iteration			
			{				
				lsp = sp->next; /*inc lsp to next node*/
				succ = lsp; /*head of succ is left pointer*/
			}
			else			
				lsp->next = rsp->next;

			sp = sp->next;
			free(rsp);			 /*free node*/			
			rsp = sp;
		}
		else // successful
		{
			sp = sp->next;

			if (lsp == rsp)
				rsp = sp;
			else
			{
				lsp = rsp;
				rsp = sp;
			}
		}				
	}

	return succ;
}

/* a=b=x[N][N] */
int nodes_same(struct node *xp, struct node *yp) {
	int i, j;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (xp->board[i][j] != yp->board[i][j]) 
				return FALSE;
		}
	}
	return TRUE;
}
/*takes in position of new move*/
int move_valid(int row, int col, int dir)
{
	switch (dir)
	{
	case DN:
		if (row >= N) return 0;
		break;
	case RT:
		if (col >= N) return 0;
		break;
	case UP:
		if (row < 0) return 0;
		break;
	case LT:
		if (col < 0) return 0;
		break;
	default:
		break;
	}
	return 1;
}
/*move value board[a][b] to [x][y]*/

struct node *move(struct node *cp, int a, int b, int x, int y, int dir) {
	struct node *newp;
	int i, j, k, l, tmp;
	int f, g, h;
	
	if (!move_valid(x, y, dir)) return NULL;

	newp = (Node*)malloc(sizeof(Node));
	newp->next = NULL;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			newp->board[i][j] = cp->board[i][j];
		}
	}
	tmp = newp->board[a][b];
	newp->board[a][b] = newp->board[x][y];
	newp->board[x][y] = tmp;
	
	if ((strategy == ASTAR) || (strategy == BEST))
	{
		h = find_h(newp->board, goal->board);
		g = cp->board[N][1] + 1;
		f = g + h;

		newp->board[N][0] = f;
		newp->board[N][1] = g;
		newp->board[N][2] = h;
		newp->board[N][3] = dir;
	}
	else
	{
		newp->board[N][0] = 0;
		newp->board[N][1] = 0;
		newp->board[N][2] = 0;
		newp->board[N][3] = dir;
	}
	// insert the direction that resulted in this node, used for printing path
	return newp;
}

struct node *goal_found(struct node *cp, struct node *gp) {	
	int i, j;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (cp->board[i][j] != gp->board[i][j]) break;
		}
		if (j < N)return NULL;
	}
	// check if succ list has goal
	// if found, return that for finding path else return NULL
	printf("Goal Found: \n");
	print_a_node(cp);
	return cp;
}

int find_h(int current[N + 1][N], int goalp[N + 1][N]) {
	int h = 0, i, j, k, l, done;

	for (i = 0; i < N; i++)
	{				
		for (j = 0; j < N; j++)
		{
			int val = current[i][j];
			int distx, disty;
			for (k = 0; k < N; k++)
			{
				for (l = 0; l < N; l++)
				{
					int goalval = goalp[k][l];
					if (goalval == val) 
						break;
				}
				if (l != N)
					break;
			}
			distx = (k - i) < 0 ? (i - k) : (k - i);		
			disty = (l - j) < 0 ? (j - l) : (l - j);

			h += distx + disty;
		}
	}
	// ...
	return h;
}

//cnt=odd -> no solution, cnt=even=solvable
int solvable(struct node *cp) {
	int i, j, k = 0, lst[NxN], cnt = 0, total = 0;
	int idx = 0;
	int blank_row;//row of blank tile
	
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			lst[idx] = cp->board[i][j];

			if (lst[idx] == 0)
				blank_row = i;
			idx++;
		}
	}

	for (i = 0; i < NxN - 1; i++)
	{
		cnt = 0;
		for (j = i + 1; j < NxN; j++)
		{
			if (lst[i] > lst[j] )
			{
				if (lst[j] == 0) continue;
					cnt++;
			}
		}
		total += cnt;
	}

	printf("blank row is %i\n", blank_row);
	printf("total is %i\n", total);
	/*Return True if
	  Blank tile is on an odd row and inversion is even
	  Blank tile is on an even row and inversion is odd	  
	*/

	if ((total & 1) && !(blank_row & 1))
		return TRUE;
	else if (!(total & 1) && (blank_row & 1))
		return TRUE;

	return FALSE;


	//return total;	/* return the number we discussed in class today 3/31/2015*/
}

/* fif 0 1 2 4 5 6 3 8 9 10 7 12 13 14 11 15 astar */
struct node *initialize(int argc, char **argv) {
	int i, j, k, npe, n, idx, gidx, inv;
	struct node *tp;
	
	tp = (struct node *) malloc(sizeof(struct node));
	idx = 1;
	for (j = 0; j<N; j++)
		for (k = 0; k<N; k++) tp->board[j][k] = atoi(argv[idx++]);
	for (k = 0; k<N; k++) tp->board[N][k] = 0;	/* set f,g,h of initial state to 0 */
	tp->next = NULL;
	start = tp;

	printf("init state: \n");
	print_a_node(start);

	tp = (struct node *) malloc(sizeof(struct node));
	gidx = 1;
	for (j = 0; j<N; j++)
		for (k = 0; k<N; k++) tp->board[j][k] = gidx++;
	tp->board[N - 1][N - 1] = 0;		/* empty tile=0 */
	for (k = 0; k<N; k++) tp->board[N][k] = 0;	/* set f,g,h of goal state to 0 */
	tp->next = NULL;
	goal = tp;

	/*printf("goal state: \n");
	print_a_node(goal);*/

	strcpy(strategy_s, argv[idx]);
	if (strcmp(strategy_s, "dfs") == 0) strategy = DFS;
	else if (strcmp(strategy_s, "bfs") == 0) strategy = BFS;
	else if (strcmp(strategy_s, "best") == 0) strategy = BEST;
	else if (strcmp(strategy_s, "bb") == 0) strategy = BB;
	else if (strcmp(strategy_s, "astar") == 0) strategy = ASTAR;
	else strategy = UNKNOWN;
	printf("strategy=%s\n", strategy_s);

	return start;
}