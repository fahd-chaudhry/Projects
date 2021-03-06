
#ifdef _REDBLACKTREE_H_
#include <cstdlib>
#include <stdexcept>
#include <string>
using namespace std;

template <class T>
RedBlackTree<T>::RedBlackTree()
 {
    root=NULL;
    size=0;
}
template <class T>
RedBlackTree<T>::~RedBlackTree()
 {
    RemoveAll();
}

template <class T>
RedBlackTree<T>::RedBlackTree(const RedBlackTree& rbtree)
 {
    root=NULL;
    size=0;
    CopyTree( rbtree.GetRoot(), rbtree.GetRoot());
    size = rbtree.Size();
}
template <class T>
 int RedBlackTree<T>::Height() const
 {
    int HeightOfTree = CalculateHeight(root);
    if (HeightOfTree > 0)
    {
        --HeightOfTree;
    }
    return HeightOfTree;
}

template <class T>
 int RedBlackTree<T>::Size() const
 {
    return size;
}
template <class T>
RedBlackTree<T>& RedBlackTree<T>::operator=(const RedBlackTree& rbtree)
{
    if (this != &rbtree)
     {
        RemoveAll();
        CopyTree( rbtree.GetRoot(), rbtree.GetRoot());
    }
    size = rbtree.Size();
    return *this;
}

template <class T>
void RedBlackTree<T>::RemoveAll()
 {
    RemoveAll(root);
    size = 0;
}
template <class T>
bool RedBlackTree<T>::Insert(T item) {
    if (Search(item) == true)
     {
        return false;
    }

    Node<T>* x = BSTInsert(item);
    ++size;
    if (x != NULL)
    {
        x->is_black = false;
        Node<T>* y = NULL;
        while (x != root && x->p != NULL && x->p->is_black == false)
         {
            if (x->p->p != NULL && x->p == x->p->p->left)
            {
                if (x->p != NULL && x->p->p != NULL)
                {
                    y = x->p->p->right;
                }

                if (y != NULL && y->is_black == false)
                {
                    x->p->is_black = true;
                    y->is_black = true;
                    x->p->p->is_black = false;
                    x = x->p->p;
                }
                else
                 {
                    if (x->p != NULL && x == x->p->right)
                     {
                        x = x->p;
                        LeftRotate(x);
                    }
                    if (x->p != NULL && x->p->p != NULL)
                    {
                        x->p->is_black = true;
                        x->p->p->is_black = false;
                        RightRotate(x->p->p);
                    }
                }
            }
            else
            {
                if (x->p != NULL && x->p->p != NULL)
                {
                    y = x->p->p->left;
                }

                if (y != NULL && y->is_black == false)
                {
                    x->p->is_black = true;
                    y->is_black = true;
                    x->p->p->is_black = false;
                    x = x->p->p;
                }
                else
                {
                    if (x->p != NULL && x == x->p->left)
                    {
                        x = x->p;
                        RightRotate(x);
                    }
                    if (x->p != NULL && x->p->p != NULL) {
                        x->p->is_black = true;
                        x->p->p->is_black = false;
                        LeftRotate(x->p->p);
                    }
                }
            }
        }
    }

    root->is_black = true;
    return true;
}

template <class T>
 int RedBlackTree<T>::CalculateHeight(Node<T>* node) const
 {
    if (node != NULL)
     {
        int leftHeight = 1 + CalculateHeight(node->left);
        int rightHeight = 1 + CalculateHeight(node->right);
        if (leftHeight > rightHeight)
        {
            return leftHeight;
        } else
         {
            return rightHeight;
        }
    }
     else
      {
        return 0;
    }
}

template <class T>
void RedBlackTree<T>::RemoveAll(Node<T>* node)
{
   if (node == NULL)
  {
    return;
  }
  RemoveAll(node->left);
  RemoveAll(node->right);

}

template <class T>
Node<T>* RedBlackTree<T>::CopyTree(Node<T>* sourcenode, Node<T>* parentnode)
 {
    Node<T>* nd = NULL;
    if (sourcenode != NULL)
    {
        nd = BSTInsert(sourcenode->data);
        ++size;
        nd->is_black = sourcenode->is_black;
        CopyTree(sourcenode->left, parentnode);
        CopyTree( sourcenode->right, parentnode);
    }
    if (sourcenode == parentnode)
    {
        root = nd;
    }
    return NULL;
}


template <class T>
bool RedBlackTree<T>::Remove(T item)
{
  Node<T>* x;
  Node<T>* y;
  Node<T>* z = root;
  bool isLeftChild;

  while (z != NULL)
  {
    if (item == z->data)
      break;
    else if (item < z->data)
      z = z->left;
    else
      z = z->right;
  }

  if (z->left == NULL || z->right == NULL)
  {
    y = z;
  }
  else
  {
    y = Predecessor(z);
  }

  if (y->left != NULL)
  {
    x = y->left;
  }
  else
  {
    x = y->right;
  }
  if (y->left != NULL || y->right != NULL)
  {
    x->p = y->p;
  }

  if (y->p == NULL)
  {
    root = x;
  }
  else
  {
    if (y == y->p->left)
    {
      y->p->left = x;
      isLeftChild = true;
    }
    else
    {
      y->p->right = x;
      isLeftChild = false;
    }
  }

  if (y != z)
  {
    z->data = y->data;
  }

  if (x != NULL && y->is_black == true)
  {
    RBDeleteFixUp(x, x->p, isLeftChild);
  }
  delete x;
  size--;
  return true;
}




template <class T>
void RedBlackTree<T>::RBDeleteFixUp(Node<T>* x, Node<T>* xparent, bool xisleftchild)
{
    Node<T>* y = NULL;
    while (x != root && x != NULL && x->is_black == true)
    {
        if (xisleftchild == true)
        {
            if (xparent != NULL)
             {
                y = xparent->right; // y is x's sibling.
            }

            if (y != NULL && y->is_black == false)
             {
                y->is_black = true;
                if (xparent != NULL)
                 {
                    xparent->is_black = false; // x's parent must have been black since y is red.
                }
                LeftRotate(xparent);
                if (xparent != NULL)
                {
                    y = xparent->right;
                }
            }

            if (y != NULL && y->left != NULL && y->right != NULL && y->left->is_black == true && y->right->is_black == true)
             {
                y->is_black = false;
                x = xparent; // Back to the loop again.
                if (x != NULL)
                 {
                    xparent = x->p; // Set the new xparent after x being modified.
                }
                xisleftchild = (xparent != NULL && x == xparent->left);
            }
            else
             {
                if (y != NULL && y->right != NULL && y->right->is_black == true)
                 {
                    if (y->left != NULL)
                    {
                        y->left->is_black = true;
                    }
                    y->is_black = false;
                    RightRotate(y);
                    if (xparent != NULL)
                    {
                        y = xparent->right;
                    }
                }
                if (y != NULL && xparent != NULL)
                 {
                    y->is_black = xparent->is_black;
                    xparent->is_black = true;
                }

                if (y != NULL && y->right != NULL)
                 {
                    y->right->is_black = true;
                }

                LeftRotate(xparent);
                x = root;
                xparent = NULL;
            }
        }
         else
          {
            if (xparent != NULL)
            {
                y = xparent->left;
            }

            if (y != NULL && y->is_black == false)
             {
                y->is_black = true;
                if (xparent != NULL)
                {
                    xparent->is_black = false;
                }
                RightRotate(xparent);
                if (xparent != NULL)
                 {
                    y = xparent->left;
                }
            }

            if (y != NULL && y->right != NULL && y->left != NULL && y->right->is_black == true && y->left->is_black == true)
             {
                y->is_black = false;
                x = xparent;
                if (x != NULL)
                {
                    xparent = x->p;
                }
                xisleftchild = (xparent != NULL && x == xparent->left);
            }
            else
            {
                if (y != NULL && y->left != NULL && y->left->is_black == true)
                {
                    if (y->right != NULL)
                    {
                        y->right->is_black = true;
                    }
                    y->is_black = false;
                    LeftRotate(y);
                    if (xparent != NULL)
                    {
                        y = xparent->left;
                    }
                }
                if (y != NULL && xparent != NULL)
                {
                    y->is_black = xparent->is_black;
                    xparent->is_black = true;
                }

                if (y != NULL && y->left != NULL)
                {
                    y->left->is_black = true;
                }

                RightRotate(xparent);
                x = root;
                xparent = NULL;
            }
        }
    }
    if (x != NULL)
    {
        x->is_black = true;
    }
}




#endif
