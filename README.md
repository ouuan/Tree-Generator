# Tree-Generator
帮助出题人造各种各样的树（开发中）

## 预期效果

```cpp
Chain chain(10); // Create a chain consisting of 10 nodes
Flower flower(20); // Create a flower consisting of 20 nodes
Tree tree = connect(chain.bottom, flower.centre, chain.top); // Create a tree consisting of the chain and the flower above, choose the top of the chain as the root
tree.renumberNodes(DFS); // renumber the nodes in DFS order
tree.reweightNodes(1, 10, MAXHEAP); // give each node a weight in [1, 10], each node's weight <= its father
tree.print(WEIGHTED, UNWEIGHTED, BFS, RANDOM); // print the tree, nodes weighted, edges unweighted, edges in BFS order, print either "u v" or "v u" randomly
```

可能大约长这样（随便想了点 feature.. ~~估计会变成 bug 而永远没有这些 feature~~

只不过我还没开始写，然而想把它实际应用于 cf 出题，所以应该不会咕太久。

