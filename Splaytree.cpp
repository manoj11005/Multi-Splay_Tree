#include <iostream>
#include <cstdlib>
using namespace std;

/* A node is created */
struct Node
{
    int key; // Value
    int depth;
    int mindepth;
    bool isRoot;      // boolean variable that indicates if the edge to its parent is dashed
    Node *leftChild;  // points to left child of the node
    Node *rightChild; // points to right child of the node
    Node *parent;     // points to parent of the node

    // Constructor
    Node(int k, int d, int md, bool ir) : key(k), depth(d), mindepth(md), isRoot(ir), leftChild(nullptr), rightChild(nullptr), parent(nullptr) {}
};

// Function to find the minimum of 2 numbers
int min(int a, int b)
{
    return (a < b) ? a : b;
}

// Creating a root node as a global variable
Node *root = nullptr;

// Function which displays all the information about the nodes in the tree
void Display(Node *root)
{
    if (!root)
        return;

    // Prints value of key at that node
    cout << "Value: " << root->key << " \nisRoot: ";

    // Tells whether the node is a root or not
    if (root->isRoot)
    {
        cout << "True \n";
    }
    else
    {
        cout << "False \n";
    }

    // Displays the key of the parent node
    if (root->parent == nullptr)
    {
        cout << "Parent: NULL \n\n";
    }
    else
    {
        cout << "Parent: " << root->parent->key << " \n\n";
    }

    // We Recursively call display function for the left and right child of the tree
    if (root->leftChild != nullptr)
    {
        Display(root->leftChild);
    }
    if (root->rightChild != nullptr)
    {
        Display(root->rightChild);
    }
}

// CreateNode function which creates a new node
Node *CreateNode(int key, int depth, int minDepth, bool isRoot)
{
    // Allocates memory to the node and assigns value to various parameters of the node
    return new Node(key, depth, minDepth, isRoot);
}

// BuildTree Function which creates the tree via recursion
Node *BuildTree(int start, int end, int depth, bool isRoot)
{
    // Terminating condition for this recursive function
    if (start == end)
    {
        return nullptr;
    }

    // We Create a mid node, and then we recursively call this Function to create its left subtree and its right subtree
    int mid = (start + end) / 2;
    Node *temp = CreateNode(mid, depth, depth, isRoot);
    temp->leftChild = BuildTree(start, mid, depth + 1, false);
    temp->rightChild = BuildTree(mid + 1, end, depth + 1, true);
    if (temp->leftChild != nullptr)
    {
        temp->leftChild->parent = temp;
    }
    if (temp->rightChild != nullptr)
    {
        temp->rightChild->parent = temp;
    }
    return temp;
}

// Rotate function which first finds out what kind of rotation to make on a certain node (zig-zig, zig-zag, zag-zag, zag-zig) and then performs that rotation
void Rotate(Node *ptr)
{
    Node *temp = ptr->parent;
    if (temp->isRoot)
    {
        temp->isRoot = false;
        ptr->isRoot = true;
    }
    if (root == temp)
    {
        root = ptr;
    }
    if (temp->parent != nullptr)
    {
        if (temp->parent->leftChild == temp)
        {
            temp->parent->leftChild = ptr;
        }
        else
        {
            temp->parent->rightChild = ptr;
        }
    }
    ptr->parent = temp->parent;
    if (temp->rightChild == ptr)
    {
        temp->rightChild = ptr->leftChild;
        if (ptr->leftChild != nullptr)
        {
            ptr->leftChild->parent = temp;
        }
        ptr->leftChild = temp;
        temp->parent = ptr;
    }
    else
    {
        temp->leftChild = ptr->rightChild;
        if (ptr->rightChild != nullptr)
        {
            ptr->rightChild->parent = temp;
        }
        ptr->rightChild = temp;
        temp->parent = ptr;
    }
    if (temp->leftChild != nullptr)
    {
        temp->mindepth = min(temp->mindepth, temp->leftChild->mindepth);
    }
    if (temp->rightChild != nullptr)
    {
        temp->mindepth = min(temp->mindepth, temp->rightChild->mindepth);
    }
}

void splay(Node *ptr, Node *top)
{
    Node *temp;
    Node *temp2;
    // this function splays the required element and makes it reach the root or the top element as provided in question
    while (!(ptr->isRoot || ptr->parent == top))
    {
        // this checks and uses the rotate function to shift some pointers and help the required element reach the top
        temp = ptr->parent;
        if (!(temp->isRoot || temp->parent == top))
        {
            temp2 = temp->parent;
            if ((temp2->leftChild == temp && temp->leftChild == ptr) || (temp2->rightChild == temp && temp->rightChild == ptr))
            {
                Rotate(temp);
            }
            else
            {
                Rotate(ptr);
            }
        }
        Rotate(ptr);
    }
}

Node *refParent(Node *ptr, bool flag)
{
    Node *temp = nullptr;
    if (!flag)
    {
        temp = ptr->leftChild;
    }
    else
    {
        temp = ptr->rightChild;
    }
    while (1)
    {
        if (!flag)
        {
            if (temp->rightChild != nullptr && temp->rightChild->mindepth < ptr->depth)
            {
                temp = temp->rightChild;
            }
            else if (temp->leftChild != nullptr && temp->leftChild->mindepth < ptr->depth)
            {
                temp = temp->leftChild;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (temp->leftChild && temp->leftChild->mindepth < ptr->depth)
            {
                temp = temp->leftChild;
            }
            else if (temp->rightChild && temp->rightChild->mindepth < ptr->depth)
            {
                temp = temp->rightChild;
            }
            else
            {
                break;
            }
        }
    }
    return temp;
}

void SwitchPath(Node *ptr)
{
    // This Function just changes the preferred child to unpreferred and vice versa, it helps in bringing the Splay-Subtree up towards the root
    Node *temp;
    if (ptr->leftChild != nullptr)
    {
        if (ptr->leftChild->mindepth > ptr->depth)
        {
            ptr->leftChild->isRoot = !(ptr->leftChild->isRoot);
        }
        else
        {
            temp = refParent(ptr, false);
            splay(temp, ptr);
            if (temp->rightChild != nullptr)
            {
                temp->rightChild->isRoot = !(temp->rightChild->isRoot);
            }
        }
    }
    if (ptr->rightChild != nullptr)
    {
        if (ptr->rightChild->mindepth > ptr->depth)
        {
            ptr->rightChild->isRoot = !(ptr->rightChild->isRoot);
        }
        else
        {
            temp = refParent(ptr, true);
            splay(temp, ptr);
            if (temp->leftChild != nullptr)
            {
                temp->leftChild->isRoot = !(temp->leftChild->isRoot);
            }
        }
    }
}

void MultiSplayAlgo(Node *ptr)
{
    // this is the main function that uses the above coded splay function and the switch path function

    // this function basically splays the node to top of the MST as well as splays the whole SubTree to the top of the MST

    Node *temp = ptr;
    while (temp->parent != nullptr)
    {
        Node *temp2 = temp->parent;
        if (temp->isRoot)
        {
            splay(temp2, nullptr);
            SwitchPath(temp2);
        }
        temp = temp2;
    }
    splay(ptr, nullptr);
}

// Function which searches a key in the tree
void Search(int key)
{
    // Create 2 nodes equal to root node
    Node *ptr = root;
    Node *prev = root;

    // Running loop till ptr becomes NULL or the key is found
    while (ptr != nullptr && ptr->key != key)
    {
        prev = ptr;

        // Searching for key as per Binary search tree
        if (key < ptr->key)
        {
            ptr = ptr->leftChild;
        }
        else
        {
            ptr = ptr->rightChild;
        }
    }
    if (ptr == nullptr)
    {
        // If key is not found in the tree we apply MultiSplayAlgo to the nearest node of the tree where key would have been present
        MultiSplayAlgo(prev);
    }
    else
    {
        // If the key is found we apply MultiSplayAlgo to that node (ptr) where key is found
        MultiSplayAlgo(ptr);
    }
}

int main()
{
    cout << "Enter the number of nodes you want in the tree: ";
    int n;
    cin >> n;
    cout << "\nMultiSplayTree is Created !!!!\n\n";
    root = BuildTree(1, n + 1, 0, true);
    int s = 0;
    while (s != 3)
    {
        cout << "1 -> Display the Tree\n2 -> Search an Element\n3 -> Exit the Code\n\nEnter Your Choice: ";
        cin >> s;
        if (s == 1)
        {
            Display(root);
        }
        else if (s == 2)
        {
            int d;
            cout << "Enter element to Search: \n";
            cin >> d;
            Search(d);
        }
        else if (s == 3)
        {
            break;
        }
        else
        {
            cout << "Enter a valid Number Choice\n";
        }
    }

    return 0;
}
