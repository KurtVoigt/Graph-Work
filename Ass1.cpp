// CSCI 411 - Fall 2020
// Assignment 1
// Author:Kurt Voigtritter
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/**************************************************************************
 * A simple Node struct                                                   *
 * id - int - the id or name of the node                                  *
 * SCC - int - the strongly connected component that this node belongs to *
 * visited - bool - whether or not this node has been visited             *
 * ************************************************************************/
struct Node {
    int id;
    int SCC;
    bool visited;
};

/**************************************************************************************************
 * A simple struct of strongly connected component (SCC) graph nodes                              *
 * id - int - the id or name of the node (this corresponds to the SCC id)                         *
 * size - int - the number of nodes from the original graph that belong to this SCC               *
 * hasInEdges - bool - true if there are edges with end points in this SCC and false otherwise    *
 * hasOutEdges - bool - true if there are edges with start points in this SCC and false otherwise *
 * ************************************************************************************************/
struct SCCNode {
    int id;
    int size;
    bool hasInEdges;
    bool hasOutEdges;
};

/*Clears a graph of visited flags
 *
 *
 */

void unvisit(vector<vector<Node*> > A){
	for(size_t i = 1; i < A[0].size(); i++){
	      A[0][i]->visited = false;
	    }
}

/*
 * A version of DFS which keeps track of all nodes reachable
 * by a source node and pts these nodes into a list
 *
 */
void NodeVisit(vector<vector<Node*> > A, Node* v,vector<int> &list){
    v->visited = true;
    list.push_back(v->id);
	for(size_t i = 0; i < A[v->id].size(); i++){
    	if(!A[v->id][i]->visited){
    		NodeVisit(A, A[v->id][i], list);
    	}
    }

}
/*
 * BFS which will mark true all nodes that were visited on the way to a target node
 */
void PathExist(vector<vector<Node*> > A, Node* target, Node* source, bool * pathList ){
	source->visited = true;
	pathList[source->id] = true;
	for(size_t i = 0; i < A[source->id].size(); i++){
		if(!A[source->id][i]->visited){
			 PathExist(A, target, A[source->id][i], pathList);
		}
	}


}

/*
 * BFS which builds a list of all nodes which can reach a target node
 */
void NodeSearch(vector<vector<Node*> > A, Node* v,vector<int> &list){
	list.push_back(v->id);
	for(size_t i = 1; i < A[0].size(); i++ ){
		unvisit(A);
		if(i != v->id){//search edges
			bool hitVertices[A[0].size()]; //= {};
			for(int f = 0; f < A[0].size(); f++){
				hitVertices[f] = false;
			}
		    PathExist(A, v, A[0][i], hitVertices);
            if(hitVertices[v->id]){
            	list.push_back(A[0][i]->id);
            }
		}
	}


}

/********************************************************************************************************
 * A variation of DFS for the first pass over a graph looking for strongly connected components.        *
 * The goal is to fill the vector L with nodes in decreasing order with respect to finishing time       *
 * A - vector<vector<Node*>> - an adjacency list                                                        *
 * &SSCList, a vector of int vectors which will contain the indices of SCCs stored in A
 * ******************************************************************************************************/
//find all the nodes from which a node has a path to
//find all nodes which have a path to that node
//take intersection of these two sets, this intersection is a set of SCCs
void DFSSCC(vector<vector<Node*> > A, vector<vector<int> > &SSCList){
	vector<vector<int> > groups;
    for(size_t i = 1; i < A[0].size(); i++){
      A[0][i]->visited = false;
      A[0][i]->SCC = -1;
    }
    for(size_t i = 1; i < A.size(); i++){
    	vector<int> pathFromi;
    	vector<int> pathToi;
    	vector<int> intersectionI;
    	unvisit(A);
    	//all the nodes which i has a path to
    	if(A[0][i]->SCC == -1) {
    		NodeVisit(A, A[0][i], pathFromi);
        	//all the nodes which have a path to i
    	    NodeSearch(A, A[0][i], pathToi);
    	//take intersection of the two and store them in L
    	    sort(pathFromi.begin(), pathFromi.end());
    	    sort(pathToi.begin(), pathToi.end());
    	    vector<int> intersection(pathFromi.size()+pathToi.size());
    	    vector<int>::iterator it, st;
    	    it = set_intersection(pathFromi.begin(),
    	                          pathFromi.end(),
    	                          pathToi.begin(),
    	                          pathToi.end(),
    	                          intersection.begin());
    	    for (st = intersection.begin(); st != it; ++st){

    		  intersectionI.push_back(A[0][*st]->id);

    	    }
    	    SSCList.push_back(intersectionI);
    	    for(size_t r = 0; r < SSCList[SSCList.size()-1].size(); r++){
    	        		 A[0][SSCList[SSCList.size()-1][r]]->SCC = (SSCList.size()-1 );
    	        	}
    	}
    }

}

/*
 * Checks for outgoing edges from a single Vertex to another
 * SSC group
 */
bool outgoingCheck(Node* x,vector<vector<Node*> > Adj){
//Search for edges that aren't in SSC
	for(size_t i = 0; i < Adj[x->id].size(); i++){
		if(Adj[x->id][i]->SCC != x->SCC){//if the edge isn't going to a SSC it is a outgoing edge
			return true;
		}
	}
	return false;
}
/*
 * Checks for an incoming edge on a node from another SSC group
 */
bool incomingCheck(Node* x, vector<vector<Node*> > Adj){
	for(size_t i = 1; i < Adj[0].size(); i++){
		if(Adj[0][i]->SCC != x->SCC){
			//cout << "Adj[0][i] SCC is " << Adj[0][i]->SCC << " and " << x->id << " is " <<x->SCC;
			//check whether node is connected to a node in x's SCC
			for(size_t j = 0; j < Adj[i].size(); j++){
				if(Adj[i][j]->SCC == x->SCC) return true;
			}
		}
	}
	return false;
}
/************************************************************************************************
 * Given the adjacency list representation of a graph, print the number of nodes that belong to *
 * the three sets A, B, and C as described in assignment 1                                      *
 * A - vector<vector<Node*>> - an adjacency list
 * SSCList - A vector of integer vectors which hold indices of the adjacency list which
 * belong to a single SSC group                                                                 *
 * **********************************************************************************************/
void printSetSizes(vector<vector<Node*> > Adj,  vector<vector<int> > SCCList){
    vector<SCCNode*> vertexGroups;
    int A = 0;
    int B = 0;
    int C = 0;
    for(size_t i = 0; i < SCCList.size(); i++){
    	SCCNode* v = new SCCNode;
    	v->id = i;
    	v->hasInEdges = false;
    	v->hasOutEdges = false;
    	v->size = SCCList[i].size();
    	vertexGroups.push_back(v);
    }

    for(size_t i = 0; i < SCCList.size(); i++){
    	for(size_t j = 0; j < SCCList[i].size(); j++){
    		if(outgoingCheck(Adj[0][SCCList[i][j]], Adj)){
    			vertexGroups[Adj[0][SCCList[i][j]]->SCC]->hasOutEdges = true;
    		}
    		if(incomingCheck(Adj[0][SCCList[i][j]], Adj)){
    			vertexGroups[Adj[0][SCCList[i][j]]->SCC]->hasInEdges = true;
    		}

    	}
    }

    for(size_t i = 0; i < vertexGroups.size(); i++){
    	if(vertexGroups[i]->hasInEdges && !vertexGroups[i]->hasOutEdges){// group B
    		B = B + vertexGroups[i]->size;
    	}
    	else if(!vertexGroups[i]->hasInEdges && vertexGroups[i]->hasOutEdges){//group A
    		A = A + vertexGroups[i]->size;
    	}
    	else{
    		C = C + vertexGroups[i]->size;
    	}
    }
    cout<<A <<" " <<B << " " <<C;
}



int main(){
    //get the number of nodes and number of edges from cin separated by a space
    int n = -1, m = -1;
    cin >> n >> m;

    //add the nodes to an adjacency list
    //note that A[0] is a list of nodes with a dummy node in A[0][0]
    //this means that A[i] is the node with id i where ids start at 1
    vector<vector<Node*> > A(n+1);
    A[0].push_back(new Node());
    for (int i=1; i<n+1; i++){
        Node* v = new Node();
        v->id = i;
        v->SCC = -1;
        v->visited = false;
        A[0].push_back(v);
    }

    //get edges from cin and add them to the adjacency list
    //the start and end of a single edge are on the same line separated by a space
    int u = -1, v = -1;
    for (int i=0; i<m; i++){
        cin >> u >> v;
        A[u].push_back(A[0][v]);
    }


    vector<vector<int> >pathy;
    //modifies vector to have groups of all strongly connected vertices
    DFSSCC(A,  pathy);

    //call printSetSizes to determine the size of the sets A, B, and C and print the results
    printSetSizes(A, pathy);

    return 0;
}


/*for(int i = 0; i < pathy.size(); i++){
   	for(int j = 0; j < pathy[i].size(); j++){
   		cout << "Node " << pathy[i][j] << " SCC is " << A[0][pathy[i][j]]->SCC<<endl;
   	}
   }*/
