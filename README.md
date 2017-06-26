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

## Week 3.

## Week 4.
