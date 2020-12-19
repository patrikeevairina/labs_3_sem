#include <iostream>
#include <memory>

using namespace std;

typedef shared_ptr<class Node> nodePtr;
typedef weak_ptr<Node> parentPtr;

class Node
{
public:
    int key;
    nodePtr left;
    nodePtr right;
    Node(int val)
    {
        left = nullptr;
        right = nullptr;
        key = val;
    }

};

class BST
{
private:
    nodePtr _root;
    int _size;
    void printTree(nodePtr);


public:
    BST();
    ~BST();

    void print();
    bool find(int);
    void insert(int);
    void erase(int);
    int size()
    {
        return _size;
    }
};

BST::BST()
    :_root(nullptr), _size(0)
{

}

void BST::print()
{
    // cout << "use " << _root.use_count() << endl;
    printTree(_root);
    cout << endl;

}

void BST::printTree(nodePtr curr)
{
    if (curr) // Проверка на ненулевой указатель
    {
        printTree(curr->left);
        cout << curr->key << " ";
        printTree(curr->right);
    }


}

bool BST::find(int key)
{
    nodePtr curr = _root;
    while (curr && curr->key != key)
    {
        if (curr->key > key)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return curr != nullptr;
}

void BST::insert(int key)
{
    //cout << endl << "size " << _size << endl;
    _size++;
    if (_root == nullptr)
    {
        _root = nodePtr(new Node(key));
        return;
    }
    else
    {
        nodePtr node = _root;
        for (int k = 0; k != -1; k++)
        {
            if (key < node->key)
            {
                if (node->left == nullptr)
                {
                    node->left = nodePtr(new Node(key));

                    return;
                }
                else
                {
                    node = node->left;
                }
            }
            else if (key >= node->key)
            {
                if (node->right == 0)
                {
                    node->right = nodePtr(new Node(key));

                    return;
                }
                else
                {
                    node = node->right;
                }
            }
        }
    }
}

void BST::erase(int key)
{
    nodePtr curr = _root;
    parentPtr parent;
    while (curr && curr->key != key)
    {
        parent = curr;
        if (curr->key > key)
        {
            curr = curr->left;
        }
        else
        {
            curr = curr->right;
        }
    }
    if (!curr)
        return;
    if (curr->left == nullptr)
    {
        // Вместо curr подвешивается его правое поддерево
        if (parent.lock() && parent.lock()->left == curr)
            parent.lock()->left = curr->right;
        if (parent.lock() && parent.lock()->right == curr)
            parent.lock()->right = curr->right;
        --_size;
        // delete curr;
        return;
    }
    if (curr->right == nullptr)
    {
        // Вместо curr подвешивается его левое поддерево
        if (parent.lock() && parent.lock()->left == curr)
            parent.lock()->left = curr->left;
        if (parent.lock() && parent.lock()->right == curr)
            parent.lock()->right = curr->left;
        --_size;
        //   delete curr;
        return;
    }
    // У элемента есть два потомка, тогда на место элемента поставим
    // наименьший элемент из его правого поддерева
    nodePtr replace = curr->right;
    while (replace->left)
        replace = replace->left;
    int replace_value = replace->key;
    erase(replace_value);
    curr->key = replace_value;
}

BST::~BST()
{
    
}

int main()
{
    BST *tree = new BST();
    for (int i = 0; i < 10; ++i)
    {
        tree->insert(i);
    }
    tree->print();
    cout << tree->size() << endl;

    for (int i = 8; i > 2; --i)
    {
        tree->erase(i);
    }

    tree->print();
    cout << tree->size();

    return 0;
}
