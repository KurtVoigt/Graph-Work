# Graph-Work
C++, Splits a Graph into groups of strongly Connected components, then splits these groups into groups A, B, C based on properties

Group A is a group of strongly Connected components which only have outgoing edges.
Group B is a group of strongly connected components which only have incoming edges
Group C is a set of all strongly connected components which do not satisfy A or B

The User constructs the graph himself. The first two entries are two numbers n,m, seperated by a space, where n is the number of vertices and m the number of 
edges.

the following entries are all the edges which the graph has, sample input:

15 24
1 2
2 1
1 3
3 2
3 6
2 6
6 12
6 13
13 12
12 14
14 13
3 7
7 13
7 8
8 9
8 11
9 10
11 10
10 7
4 5
5 4
4 11
5 11
10 15
