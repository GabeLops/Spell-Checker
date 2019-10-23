// AVLSet.hpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"


template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet() noexcept;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.

    bool shouldBalance;

    class TreeNode {
    public:
        ElementType element;
        TreeNode* leftChild;
        TreeNode* rightChild;
        int height;

        TreeNode(const ElementType& e) : element(e), leftChild(NULL), rightChild(NULL), height(-1) { }

        ~TreeNode() noexcept {
            delete leftChild;
            delete rightChild;
        }

        TreeNode(const TreeNode* s) {
            this->element = s->element;
            if(s->leftChild != NULL) {
                this->leftChild = new TreeNode(s->leftChild);
            }
            else {
                this->leftChild = NULL;
            }

            if(s->rightChild != NULL) {
                this->rightChild = new TreeNode(s->rightChild);
            }
            else {
                this->rightChild = NULL;
            }
            this->height = s->height;
        }
    };

    TreeNode* avlTree;

    void recursiveAdd(const ElementType& element, TreeNode *&root);

    bool containsRecursive(const ElementType& element, TreeNode* root) const;

    unsigned int recursiveSize(TreeNode* root) const noexcept;

    int recursiveHeight(TreeNode* root) const;

    void recursivePreorder(VisitFunction visit, TreeNode* root) const;

    void recursiveInorder(VisitFunction visit, TreeNode* root) const;

    void recursivePostorder(VisitFunction visit, TreeNode* root) const;

    // rotate methods
    void rotateLeft(TreeNode *&node);
    void rotateRight(TreeNode *&node);
    void rotateRightLeft(TreeNode *&node);
    void rotateLeftRight(TreeNode *&node);
    void balance(TreeNode *&node);
    int balanceFactor(TreeNode* node);
};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
{
    this->shouldBalance = shouldBalance;
    avlTree = NULL;
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    delete avlTree;
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
    this->shouldBalance = s.shouldBalance;
    this->avlTree = NULL;

    s.inorder([&](const ElementType& element) { this->add(element); });
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
    this->shouldBalance = s.shouldBalance;
    this->avlTree = NULL;

    s.inorder([&](const ElementType& element) { this->add(element); });
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    this->shouldBalance = s.shouldBalance;
    // check if we need to delete something first
    if(this->avlTree != NULL) {
        delete this->avlTree;
    }
    this->avlTree = NULL;

    s.inorder([&](const ElementType& element) { this->add(element); });

    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    this->shouldBalance = s.shouldBalance;
    // check if we need to delete something first
    if(this->avlTree != NULL) {
        delete this->avlTree;
    }
    this->avlTree = NULL;

    s.inorder([&](const ElementType& element) { this->add(element); });
       
    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    if(avlTree == NULL) {
        avlTree = new TreeNode(element);
    }
    else {
        recursiveAdd(element, avlTree);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::recursiveAdd(const ElementType& element, TreeNode *&root) {
    if(root->element > element) {
        if(root->leftChild != NULL) {
            recursiveAdd(element, root->leftChild);
            if(shouldBalance)
                balance(root);
        }
        else {
            root->leftChild = new TreeNode(element);
        }
    }
    else {
        if(root->rightChild != NULL) {
            recursiveAdd(element, root->rightChild);
            if(shouldBalance)
                balance(root);
        }
        else {
            root->rightChild = new TreeNode(element);
        }    
    }
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    return containsRecursive(element, avlTree);
}

template <typename ElementType>
bool AVLSet<ElementType>::containsRecursive(const ElementType& element, TreeNode* root) const
{
    // first check if root is null
    if(root == NULL) {
        return false;
   }
   if(root->element == element) {
        return true;
   }
   // othersie do recursive calls
   if(root->element > element) {
        return containsRecursive(element, root->leftChild);
   }
   else {
        return containsRecursive(element, root->rightChild); 
   }
}

template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return recursiveSize(avlTree);
}

template <typename ElementType>
unsigned int AVLSet<ElementType>::recursiveSize(TreeNode* root) const noexcept
{
    if(root == NULL) {
        return 0;
    }
    return 1 + recursiveSize(root->leftChild) + recursiveSize(root->rightChild);
}

template <typename ElementType>
int AVLSet<ElementType>::height() const
{
    return recursiveHeight(avlTree);
}


template <typename ElementType>
int AVLSet<ElementType>::recursiveHeight(TreeNode* root) const
{
    if(root == NULL) {
        return -1;
    }

    int leftChildHeight = recursiveHeight(root->leftChild);
    int rightChildHeight = recursiveHeight(root->rightChild);
    root->height = 1 + (leftChildHeight < rightChildHeight ? rightChildHeight : leftChildHeight);

    return root->height;
}

template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    recursivePreorder(visit, avlTree);
}

template <typename ElementType>
void AVLSet<ElementType>::recursivePreorder(VisitFunction visit, TreeNode* root) const
{
    if(root == NULL) {
        return;
    }
    visit(root->element);
    recursivePreorder(visit, root->leftChild);
    recursivePreorder(visit, root->rightChild);
}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    recursiveInorder(visit, avlTree);
}

template <typename ElementType>
void AVLSet<ElementType>::recursiveInorder(VisitFunction visit, TreeNode* root) const
{
    if(root == NULL) {
        return;
    }

    recursiveInorder(visit, root->leftChild);
    visit(root->element);
    recursiveInorder(visit, root->rightChild);
}

template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    recursivePostorder(visit, avlTree);
}


template <typename ElementType>
void AVLSet<ElementType>::recursivePostorder(VisitFunction visit, TreeNode* root) const
{
    if(root == NULL) {
        return;
    }

    recursivePostorder(visit, root->leftChild);
    recursivePostorder(visit, root->rightChild);
    visit(root->element);
}

template <typename ElementType>
void AVLSet<ElementType>::rotateLeft(TreeNode *&node) 
{
    TreeNode* tmpNode = node->rightChild;
    node->rightChild = tmpNode->leftChild;
    tmpNode->leftChild = node;

    node = tmpNode;
}

template <typename ElementType>
void AVLSet<ElementType>::rotateRight(TreeNode *&node) 
{
    TreeNode* tmpNode = node->leftChild;
    node->leftChild = tmpNode->rightChild;
    tmpNode->rightChild = node;

    node = tmpNode;   
}

template <typename ElementType>
void AVLSet<ElementType>::rotateRightLeft(TreeNode *&node)
{
    TreeNode* tmpNode = node->rightChild;
    rotateRight(tmpNode);
    node->rightChild = tmpNode;
    rotateLeft(node);
}

template <typename ElementType>
void AVLSet<ElementType>::rotateLeftRight(TreeNode *&node) 
{
    TreeNode* tmpNode = node->leftChild;
    rotateLeft(tmpNode);
    node->leftChild = tmpNode;
    rotateRight(node);
}

template <typename ElementType>   
void AVLSet<ElementType>::balance(TreeNode *&node)
{
    int factor = balanceFactor(node);
    if(factor > 1) {
        if(balanceFactor(node->leftChild) > 0)
            rotateRight(node);
        else
            rotateLeftRight(node);
    }
    if(factor < -1) {
        if(balanceFactor(node->rightChild) < 0) 
            rotateLeft(node);
        else
            rotateRightLeft(node);
    }
}

template <typename ElementType>   
int AVLSet<ElementType>::balanceFactor(TreeNode *node)
{
    int leftHeight = recursiveHeight(node->leftChild);
    int rightHeight = recursiveHeight(node->rightChild);

    if(node->leftChild == NULL)
        return -(rightHeight + 1);
    if(node->rightChild == NULL)
        return (leftHeight + 1);

    return leftHeight - rightHeight;
}

#endif // AVLSET_HPP

