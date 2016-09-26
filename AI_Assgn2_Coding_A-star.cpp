#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

vector<vector<int> > goal_puzzle(3,vector<int> (3));
map<vector<vector<int> > , vector<vector<int> > > parent;
map<vector<vector<int> > , bool> visited;	// puzzle is visited or not
int expand_nodes = 0;

map<vector<vector<int> > , int > f_n;
map<vector<vector<int> > , int > g_n;

// typedef <vector<int> > state;	// Current puzzle , State 

int dx[]={-1,+1,0,0};
int dy[]={0,0,-1,+1};

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

bool visit(vector<vector<int> > puzzle)
{
	if(visited[puzzle]==true)
		return true;
	else
		return false;
}

bool safe(int i,int j)
{
	if(i>=0 && i<=2 && j>=0 && j<=2)
		return true;
	else
		return false;
}

vector<vector<vector<int> > > neighbours(vector<vector<int> > puzzle) {
	pair<int,int> pos;
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			if(puzzle[i][j]==0) {
				pos.first=i;
				pos.second=j;
				break;
			}
		}
	}
	vector<vector<vector<int> > > ans;
	for(int k=0;k<4;k++) {
		int cx = pos.first;
		int cy = pos.second;
		vector<vector<int> > n = puzzle;
		if(safe(cx+dx[k],cy+dy[k]))
		{
			swap(n[cx+dx[k]][cy+dy[k]],n[cx][cy]);
			ans.push_back(n);
		}
	}

	return ans; 
}

void print(vector<vector<int> > puzzle) {		// Print_purpose
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			cout<<puzzle[i][j]<<" ";
		}
		cout<<"\n";
	}
	cout<<"\n\n";
}

void print_path(vector<vector<int> > puzzle) {
   if(parent.count(puzzle)) {
   		print_path(parent[puzzle]);
   }

   for(int i=0;i<3;i++) {
   		for(int j=0;j<3;j++) {
   			cout << puzzle[i][j] <<" ";
   		}
   		cout<<endl;
   	}
	cout <<endl; 
	return;
}

bool is_equal(vector<vector<int> > puzzle) {
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			if(puzzle[i][j] != goal_puzzle[i][j])
				return 0;
		}
	}
	return 1;
}

int heuristic_last(vector<vector<int> > puzzle)
{
	int dist = 0;
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			if(puzzle[i][j]!=0) {
				for(int k=0;k<3;k++) {
					for(int l=0;l<3;l++) {
						if(puzzle[i][j]==goal_puzzle[k][l])
							dist+=abs(i-k)+abs(j-l);
					}
				}
			}
		}
	}
	return dist;
}

int heuristic_first(vector<vector<int> > puzzle)
{
	return 0;
}

int heuristic_middle(vector<vector<int> > puzzle)
{
	int dist = 0;
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			if(puzzle[i][j]!=0) {
				if(i==0) {
					if(puzzle[i][j]!=j+1)
						dist+=1;
				}
				if(i==1) {
					if(puzzle[i][j]!=j+4)
						dist+=1;
				}
				if(i==2) {
					if(puzzle[i][j]!=j+7)
						dist+=1;
				}
			}
		}
	}
	return dist;
}

struct compare {		// Compare Function
	bool operator() (vector<vector<int> > &a, vector<vector<int> > &b) {
		int am = f_n[a];
		int bm = f_n[b];
		return am>bm;
	}
};

void solve_puzzle(vector<vector<int> > initial_puzzle) {

	priority_queue <vector<vector<int> >, vector<vector<vector<int> > >, compare > prio_queue;		// Kis basis pr compare kr rhe
	g_n.insert(pair<vector<vector<int> > , int > (initial_puzzle,0));
	f_n.insert(pair<vector<vector<int> > , int > (initial_puzzle,g_n[initial_puzzle]+heuristic_last(initial_puzzle)));

	prio_queue.push(initial_puzzle);

	bool isSol = false;
	while(!prio_queue.empty())
	{
		vector<vector<int> > current_puzzle = prio_queue.top();
		prio_queue.pop();
		
		visited[current_puzzle] = true;
		
		if(is_equal(current_puzzle) == 1) {
			isSol = true;
			printf("Solution Found ---> \n");
			print_path(current_puzzle);
			break;
		}

		expand_nodes++;
		vector<vector<vector<int> > > node_expand = neighbours(current_puzzle);
		vector<vector<vector<int> > >::iterator it;
		for(it=node_expand.begin();it!=node_expand.end();it++)
		{
			vector<vector<int> > temp = *it;
			if(!visit(temp))
			{
				g_n.insert(pair<vector<vector<int> > , int > (temp,g_n[current_puzzle]+1));
				f_n.insert(pair<vector<vector<int> > , int > (temp,heuristic_last(temp)+g_n[temp]));
				parent.insert(pair<vector<vector<int> > , vector<vector<int> > >(temp,current_puzzle));
				prio_queue.push(temp);
			}
		}
	}
	
	if(isSol == false)
		printf("Solution not found\n");
	printf("Nodes Expand ---> %d\n", expand_nodes);
	return;
}

int main() {

	srand(time(0));
  	struct timeval stop, start;
  	float elapsed;
  	double diff;

	int count = 0;
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			goal_puzzle[i][j] = (++count)%9;
		}
	}

	vector<int> nos;
	for (int i = 0; i < 9; ++i){
		nos.push_back(i);
	}

	int t = 100;
	while(t--) {
		printf("Iterations left --> %d\n",t);
		parent.clear();
		visited.clear();
		vector<vector<int> > initial_puzzle(3,vector<int> (3));
		expand_nodes = 0;
		random_shuffle(nos.begin(),nos.end());
		
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				initial_puzzle[i][j] = nos[3*j+i];	
			}
		}

		cout << "\n------------A* heuristic_last-----------------\n"<< endl;
		gettimeofday(&start, NULL);

		print(initial_puzzle);
		solve_puzzle(initial_puzzle);

		gettimeofday(&stop, NULL);
	    elapsed = timedifference_msec(start, stop);
	    printf("Time taken (%f)ms\n", elapsed);
	    cout << "-------------------------------------\n"<< endl;
	}
}