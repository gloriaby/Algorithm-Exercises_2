## Week 1. Strongly Connected Component
Related files:

* 1\_kosaraju\_nonrecursiveDFS.c,
* 1\_SCC.txt,
* 1\_countleader.txt

### Task
The directed graph (tail---->head) is given in file 1\_SCC.txt with tails in the first column and heads in the second column. Computing the [Strongly Connected Component (SCC)](https://en.wikipedia.org/wiki/Strongly_connected_component) on this graph and find the size of each SCC.


### Kosaraju's algorithm
According to the course *Graph Search, Shortest Paths, and Data Structures* in Coursera, the algorithm can be described as

1. Get the reversed graph G_r
2. Run Depth First Search Loop on the reversed graph G_r, rename each name with its finish time t.
3. Run Depth First Search Loop on the original graph G (with updated node name in last step) and get the leader of each node.
4. Group each SCC (with the same leader) and count the number of nodes in each SCC.

### My Implementation
I implement the DFS algorithm in a [non-recursive](https://en.wikipedia.org/wiki/Depth-first_search#Pseudocode) way. The stack is created to store the name of each node that is waiting to be explored. Every time when we pop a node (actually it is its name), I will check whether it has already been explored. As long as it's not been explored, I mark it as explored and push all nodes in the corresponding row of the adjacent list. Although the nodes will be checked when we pop it, I could only push the unexplored node.

I have an array `leader[]` to record

* whether a node is been explored or not
* who is its leader

Initially each entry is 0, which means no node is been explored. As long as node i is explored, then `leader[i-1]` could be assigned a non-zero value, representing that this node is been explored. The value of `leader[i-1]` has no meaning (as long as it's non-zero) in the first call of `DFS_Loop()`. However, in the second call of `DFS_Loop()`, s in `leader[i-1]=s` is node i's leader.

I also have an array `finishtime[]` to record

* the finishing time of each node (during the first call of `DFS_Loop()`)

In the first call of `DFS_Loop()`, all nodes are renamed based on their finishing time. This is easy to implement in recursive DFS algorithm but there is no function return to let me record the finishing time in non-recursive method. (In order to get the finishing time in non-recursive method) comparing with the recursive method, in `DFS_Loop()` every time it start with a new unexplored node to go into `DFS_nonrecursion()`, the node could be explored in the same order as the recursive method. Therefore, I could increase the value of `t` by 1 when I mark a node as explored and temporarily stored it in `leader[]`. After finish this `DFS_nonrecursion()` leading by a specific node, reverse the order (and update the value in `leader[]`) of all node in the call of that `DFS_nonrecursion()`. Since all nodes being explored in this call of `DFS_nonrecursion()` should have finishing time greater than those of the nodes being explored in the last call of `DFS_nonrecursion()`, I have another variable `t_max` store the total number of nodes being explored before this call of `DFS_nonrecursion()` and the finshing time should be added with `t_max` during each call.	

After call `DFS_Loop` twice, the SCCs are computed and then I have another array `count[]` to record

* the number of nodes in a SCC

For example, count[0]=3 means there are three nodes in a SCC whose leader is node 1. We should also note that the node name "1" in this example is modified during the first call of `DFS_Loop()`. I do not record the original name of each node, that node 1 is the node with finishing time equals 1.

### Run My Code
You may need to modify the stack size if you are trying to compute the directed graph given by 1\SCC.txt since it has a large number of nodes. In my case, run

	ulimit -s 32767
	
to increase the stack size to 32M Bytes before running my code. After running my code, the the number of nodes in each SCC will be output to the file 1\_countleader.txt.

## Week 2.
Related Files:

* 2_dijkstra.c
* 2_dijkstraData.txt

### Task

Given an adjacent list representing an undirected weighted graph in file 2_dijkastraData.txt. For each line, the first colomn represent the node number (from 1 to 200) and other columns are tuples of (node, weight) that connected with the node in the first column. Set node 1 as the source node, compute the shortest paths to all other nodes from the source. (If there is no path between the source and the destination, set the shortest paths to be 1000000.)

### Dijkstra's Algorithm

According to the same course, the algorithm can be described as:

* Initialize the set X (nodes that are explored): put the source node into that set.
* Set the shortest path to source node equals to 0.

* while we could found nodes to be put into set X:
	* among all **edges** (v,w) with v in set X and w not in set X, pick the one that minimize **the shortest paths to node v plus distance between v and w**. 
	* add w to set X
	* set the shortest path to w equals **the shortest paths to node v plus distance between v and w**

### My Implementation

About the `while` condition, I have an int variable `X_size` to record the times we go through the while loop. If for any node `w`, there always exists a shortest path from souce node to node `w`, then for each iteration, we will always found a new node `w` and set its shortest path and increase the `X_size` by 1. For this case `X_size` means the number of nodes in set X. If for a node `w`, there is actually no path between the source to it, we could check whether `w_old==w` where `w_old` is the variable I set equals to `w` in the last iteration. If `w_old==w` then we know that we didn't find any node in this iteration. We will not put node `w` into set X (element `X[w-1]` will not be set to 1 in my code) and we will set the shortest path between the sorce node to node `w` equal to 1000000. Note that no matter whether we could find any node to be added into set X in each iteration, X_size will always increase by 1 for each iteration. If there exist some nodes that can not be reached from the source, `X_size` will not represent the size of set X. Instead, it just used to record the number of iterations we have experienced so far.

To check amoung all edges, I simply go through all edges which meet the requirement (one vertex in in set X and another one not) so that the complexity would be O(mn) where m is the number of edges and n is the number of vertexes.

### Run My Code

The output will gives the shortest paths from source node 1 to all the nodes.

## Week 3.

## Week 4.
























