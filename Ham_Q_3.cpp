#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* Global Variables */

int **_graph_; /** hold the graph */
int * _ham_path_so_far_; /** Hold the length of path discovered so far */
int _total_vertex_in_graph_; /** Hold the total number of vertex in the graph*/
int _total_edge_in_graph_; /** Hold the total number of edges given in the graph*/
int _k_; /** Hold the maximum number of edges given to be included in the graph*/


int _total_edge_possible_; /** Hold the total number of edges possible in a graph*/
int* _edge_list_; /** Hold the edges, which has selected to be added to the graph*/

/**
*	Input: int Array = Edge List to be removed from the graph
*	output: None.
*	Description: This function removed edges from the graph.
*/
void remove_edges(int a[]){
	for(int i=0; i<_k_; i++){
		int r = int(a[i]/_total_vertex_in_graph_);
		int c = a[i]%_total_vertex_in_graph_;
		_graph_[r][c]=0;
		_graph_[c][r]=0;
	}
}
/**
*	Input: Int Array = Edge List array to be added to the Graph
*	Output: None.
*	Description: This function adds edges to the graph.
*/
void add_edges(int a[]){
	for(int i=0; i<_k_; i++){
		int r = int(a[i]/_total_vertex_in_graph_);
		int c = a[i]%_total_vertex_in_graph_;
		_graph_[r][c]=1;
		_graph_[c][r]=1;
	}
}
/**
*	Input: 	int edge_number = The edge number which is considering to be added in the graph
*			int l = the number edges added so far
*	Output:	Boolean. True, if edge is safe to be added.	
*					 False, if edge is not safe to be added.
*/

bool is_edge_safe(int edge_number,int l){
	int r = int(edge_number / _total_vertex_in_graph_); 
	int c = edge_number % _total_vertex_in_graph_;
	
	if(c >= r)
		return false;
	
	if(_graph_[r][c] == 1)
		return false;
	
	// Check wether adding edges create increase a vertex degree by 3
	int count1=0, count2=0;
	for(int i=0; i<l;i++){
		int r1 = int(_edge_list_[i]/_total_vertex_in_graph_);
		int c1 = int(_edge_list_[i]%_total_vertex_in_graph_);
		if(edge_number==_edge_list_[i])
			break;
		if(r==r1)
			count1++;
		if(c==c1)
			count2++;
		if(count1==2 || count2==2)
			return false;
	}
	return true;
}

/**
*	Input: 	int v= current possible vertex visited to be added in the hamiltonian path
*			int path_so_far= The length of path already discovered
*	Output: Bollean.	True, If v is safe to be added in the currently discovered path so far.
*						False, otherwise.									
*/
bool check_v_is_safe(int v, int path_so_far){

	for(int i=0; i<path_so_far;i++){
		if(_ham_path_so_far_[i]==v)
			return false;
	}

	int u = _ham_path_so_far_[path_so_far-1];
	return _graph_[u][v]==1 ? true:false;
	
	if(path_so_far == _ham_path_so_far_[0] && v > _ham_path_so_far_[0]){
		int flag = 0;
		for(int i=1; i< _ham_path_so_far_[0]; i++ ){
			if(_ham_path_so_far_[i] < _ham_path_so_far_[0])
			{
				flag=1;
				break;
			}
		}
		if(flag==0)
		return false;
	}
}
/**
*	Input: Integer. Number of edges discovered so far to get a hamiltonian path.
*	Output: Bollean.	True, if current graph has a hamiltonian path
*						False, if current graph has no hamiltonian path
*/
bool find_hamilton_path(int path_so_far){

	for(int v=0; v<_total_vertex_in_graph_; v++){
		if(path_so_far!=0){
			if(!check_v_is_safe(v,path_so_far))
				continue;
		}if(path_so_far==0 && v==_total_vertex_in_graph_-1 && v!=0){
			continue;
		}
		_ham_path_so_far_[path_so_far]=v;

		if(path_so_far == _total_vertex_in_graph_-1){
			return true;	/*full path has been constructed*/
		}
		if(find_hamilton_path(path_so_far+1)){ /*check wether hamilton path already found or not*/
			return true;
		}
	}
	return false;
}
/**
*	Input: None
*	Output: Boolean.	True, if current graph has a hamiltonian path
*						False, if current graph has no hamiltonian path
*/
bool hamilton(){
	return find_hamilton_path(0);
}
/**
*	Input: Integer. 	The number of edges added to the graph so far.
*	Output: Boolean.	True if, Graph has a Hamiltonian Path for adding maximum K edges.
*						False, otherwise.
*	Dependency: It calls hamilton() function. 
*/
bool find_hamilton_cover(int edge_so_far){
	if(edge_so_far>=_k_){
		bool flag=false;
		add_edges(_edge_list_);
		if(hamilton()){
			flag=true;
		}
		remove_edges(_edge_list_);
		return flag;
	}
	if(edge_so_far==0){
		for(int i=0; i<_total_edge_possible_; i++){
			/* Ignoring diagnoal element from hamilton_cover */
			if(!is_edge_safe(i,edge_so_far))
				continue;			
			_edge_list_[edge_so_far]=i;
			if(find_hamilton_cover(edge_so_far+1))
				return true;	
		}
	}
	else{
		for(int i=_edge_list_[edge_so_far-1]+1;i<_total_edge_possible_;i++){
			//printf("%d\n",i);
			if(!is_edge_safe(i,edge_so_far))
				continue;
			_edge_list_[edge_so_far]=i;
			if(find_hamilton_cover(edge_so_far+1))
				return true;
		}
	}
	return hamilton();
}
/**
*	Input: File name
*	Output: None
*	Description: It reads all the input parameters
*/
void read_input_file(const char * file_name){

	FILE *fp;
	fp=fopen(file_name, "r");
	
 	fscanf (fp, "%d", &_total_vertex_in_graph_);   
	fscanf (fp, "%d", &_total_edge_in_graph_);
	fscanf (fp, "%d", &_k_);
	if(_k_ >= _total_vertex_in_graph_){
		_k_ = _total_vertex_in_graph_-1;
	}

	_total_edge_possible_ = _total_vertex_in_graph_ * _total_vertex_in_graph_;

 /* Allocating space to store _graph_'s adjacency matrix */
	_graph_ = new int*[ _total_vertex_in_graph_ ];
	for(int i=0; i< _total_vertex_in_graph_; i++){
		_graph_[i] = new int[_total_vertex_in_graph_];
	}
 /* Initializing Adjacency Matrix of _graph_ */	
	for(int i=0; i< _total_vertex_in_graph_ ;i++){
		for(int j=0; j<_total_vertex_in_graph_;j++){
			_graph_[i][j]=0;
		}
	}
 /* Putting Adjacency Matrix of _graph_ */
	int v1,v2;
	for(int i=0; i<_total_edge_in_graph_; i++){
		fscanf(fp,"%d %d",&v1,&v2);
		_graph_[v1-1][v2-1]=1;
		_graph_[v2-1][v1-1]=1;
	}
	fclose(fp);
}

bool hamilton_cover(int k){
	_k_ = k;
	find_hamilton_cover(0);
}
/**
* 	Input: None
*	Output: Integer value, The minimum number of edges are required to add
*			to have a hamiltonian path in the graph.
*   Dependency: It calls hamilton_cover(int) function
*/
int min_hamilton_cover(){

	int l=0;
	int h=_total_vertex_in_graph_-1;
	int m=0;
	_k_ = 0;

	while(l<h){
		m = l+ ceil((double)(h-l)/2);
		free(_edge_list_);
		_edge_list_ = new int[m];
		if(hamilton_cover(m)){
			h = m-1;
		}
		else{
			l = m+1;
		}		
	}
	if(hamilton_cover(h)){
		return h;
	}else{
		return h+1;
	}
}
/**
* Input: k = Maximum edges can be added
* Output: If Graph has K path cover
* Dependecy: It calls hamilton_cover(int) function
*/
bool path_cover(int k){
	if(k > _total_vertex_in_graph_){
		return false;
	}
	if(hamilton_cover(k-1)){
		return true;
	}
	else{
		return false;
	}
}
int main(int argc, char **argv){
	if(argc < 2){
		printf("Enter File Name and Try Again\n");
		return 0;
	}

	const char * file_name = argv[1];

	read_input_file(file_name);
	_ham_path_so_far_ = new int[_total_vertex_in_graph_];
	_edge_list_ =  new int[_k_];	
	int k1 = _k_;

	printf("%s\n",path_cover(k1)?"YES":"NO");

	return 0;
}