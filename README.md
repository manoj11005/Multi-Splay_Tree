# Multi-Splay_Tree

This project implements a MultiSplay Tree in C++, a specialized binary search tree structure that combines splay tree and multi-splay tree concepts to optimize the efficiency of tree operations. A MultiSplay Tree is designed to perform splay operations on multiple nodes simultaneously, thus enhancing the performance of search operations and dynamically maintaining a balanced tree structure.

FEATURES:

FUNCTIONS IMPLEMENTED IN THIS CODE:

1. BuildTree(): This function creates the entire tree. The Tree T stores the key value, minDepth, depth, and information on whether it is a root.
2. Rotate(): This function performs the rotation operation on a node.
3. Splay(): This function performs the splay operation on a node, bringing it closer to the root.
4. SwitchPath(): Similar to a splay tree, this self-adjusting update operation swaps the preferred child.
5. Search(): This function accesses a particular element using the Multi-Splay Algorithm in the Multi-Splay Tree.
6. Display(): This function displays the entire tree, showing the key value, root status, and parent information for each node.
