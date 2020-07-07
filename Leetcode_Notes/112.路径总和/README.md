# 问题：112. 路径总和

给定一个二叉树和一个目标和，判断该树中是否存在根节点到叶子节点的路径，这条路径上所有节点值相加等于目标和。

**说明**: 叶子节点是指没有子节点的节点。

## 示例: 
给定如下二叉树，以及目标和 `sum = 22`，

              5
             / \
            4   8
           /   / \
          11  13  4
         /  \      \
        7    2      1

返回 `true`, 因为存在目标和为 `22` 的根节点到叶子节点的路径 `5->4->11->2`。

# 解题思路

**关键词**：*二叉树*、*递归*

## 基础知识

### 二叉树

**二叉树**（Binary tree）是树形结构的一个重要类型，是n(n>=0)个结点的有限集合，该集合或者为空集（称为空二叉树），或者由一个根结点和两棵互不相交的、分别称为根结点的左子树和右子树组成。

**特点**：
1. 每个结点最多有两颗子树，所以二叉树中不存在度大于2的结点；
2. 左子树和右子树是有顺序的，次序不能任意颠倒；
3. 即使树中某结点只有一棵子树，也要区分它是左子树还是右子树。

#### 特殊二叉树

**满二叉树**：在一棵二叉树中。如果所有分支结点都存在左子树和右子树，并且所有叶子都在同一层上，这样的二叉树称为满二叉树。

**完全二叉树**：除了最底层的叶子结点之外,其余层全满,而且叶子层集中在左端。

#### 代码表示

##### 定义结点

一个树结点包含储存的数据、左孩子指针、右孩子指针。

```c++
struct BiTree {
    int data;
    BiTree *left;
    BiTree *right;
    BiTree(int x) : data(x), left(NULL), right(NULL) {}
};
```

##### 构造

##### 遍历

二叉树的遍历是指从二叉树的根结点出发，按照某种次序依次访问二叉树中的所有结点，使得每个结点被访问一次，且仅被访问一次。

二叉树的访问次序可以分为四种：前序遍历、中序遍历、后序遍历、层序遍历。

###### 前序遍历

> 前序遍历通俗的说就是从二叉树的根结点出发，当**第一次**到达结点时就输出结点数据，按照先向左在向右的方向访问。  
对任意二叉树树结构，先打印节点本身，再打印左子节点，最后打印右子结点。即：中、左、右。

```c++
/*二叉树的前序遍历递归算法*/
void PreOrderTraverse(BiTree T)
{
    if(T==NULL)
    return;
    printf("%c", T->data);  /*显示结点数据，可以更改为其他对结点操作*/
    PreOrderTraverse(T->left);    /*再先序遍历左子树*/
    PreOrderTraverse(T->right);    /*最后先序遍历右子树*/
}
```

###### 中序遍历

> 中序遍历就是从二叉树的根结点出发，当**第二次**到达结点时就输出结点数据，按照先向左在向右的方向访问。  
对任意二叉树树结构，先打印左子节点，再打印节点本身，最后打印右子结点。即：左、中、右。

```c++
/*二叉树的中序遍历递归算法*/
void InOrderTraverse(BiTree T)
{
    if(T==NULL)
    return;
    InOrderTraverse(T->left); /*中序遍历左子树*/
    printf("%c", T->data);  /*显示结点数据，可以更改为其他对结点操作*/
    InOrderTraverse(T->right); /*最后中序遍历右子树*/
}
```

###### 后序遍历

>后序遍历就是从二叉树的根结点出发，当**第三次**到达结点时就输出结点数据，按照先向左在向右的方向访问。  
对任意二叉树树结构，先打印左子节点，再打印右子节点，最后打印结点本身。即：左、右、中。

```c++
/*二叉树的后序遍历递归算法*/
void PostOrderTraverse(BiTree T)
{
    if(T==NULL)
    return;
    PostOrderTraverse(T->left);   /*先后序遍历左子树*/
    PostOrderTraverse(T->right);   /*再后续遍历右子树*/
    printf("%c", T->data);  /*显示结点数据，可以更改为其他对结点操作*/
}
```

## 思路

1. 递归

```c++
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    bool hasPathSum(TreeNode* root, int sum) {
        if (root==NULL)
        {
            return false;
        }
        if (root->left==NULL&&root->right==NULL&&root->val==sum)
        {
            return true;
        }
        return hasPathSum(root->left,sum-root->val)||hasPathSum(root->right,sum-root->val);
    }
};
```

2.