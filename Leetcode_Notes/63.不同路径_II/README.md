## 问题：63. 不同路径 II

一个机器人位于一个 m(列数) x n(行数) 网格的左上角 （起始点在下图中标记为“Start” ）。

机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为“Finish”）。

现在考虑网格中有障碍物。那么从左上角到右下角将会有多少条不同的路径？

![示例图](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/10/22/robot_maze.png)

网格中的障碍物和空位置分别用 1 和 0 来表示。

**说明**：m 和 n 的值均不超过 100。

### 示例

    输入:
    [
      [0,0,0],
      [0,1,0],
      [0,0,0]
    ]
    输出: 2
    解释:
    3x3 网格的正中间有一个障碍物。
    从左上角到右下角一共有 2 条不同的路径：
    1. 向右 -> 向右 -> 向下 -> 向下
    2. 向下 -> 向下 -> 向右 -> 向右

## 解题思路

**关键词**(key word) : dp(动态规划)问题、滚动数组思想

### 方法基础

#### 动态规划

动态规划：Dynamic Programming，DP。是一种在数学、计算机科学和经济学中使用的，通过把原问题分解为相对简单的子问题的方式求解复杂问题的方法。

动态规划常常适用于有重叠子问题和最优子结构性质的问题，动态规划方法所耗时间往往远少于朴素解法。

动态规划背后的基本思想非常简单。大致上，若要解一个给定问题，我们需要解其不同部分（即子问题），再合并子问题的解以得出原问题的解。 通常许多子问题非常相似，为此动态规划法试图仅仅解决每个子问题一次，从而减少计算量：一旦某个给定子问题的解已经算出，则将其记忆化存储，以便下次需要同一个子问题解之时直接查表。这种做法在重复子问题的数目关于输入的规模呈指数增长时特别有用。

动态规划问题满足三大重要性质：

- 最优子结构性质：如果问题的最优解所包含的子问题的解也是最优的，我们就称该问题具有最优子结构性质（即满足最优化原理）。最优子结构性质为动态规划算法解决问题提供了重要线索。

- 子问题重叠性质：子问题重叠性质是指在用递归算法自顶向下对问题进行求解时，每次产生的子问题并不总是新问题，有些子问题会被重复计算多次。动态规划算法正是利用了这种子问题的重叠性质，对每一个子问题只计算一次，然后将其计算结果保存在一个表格中，当再次需要计算已经计算过的子问题时，只是在表格中简单地查看一下结果，从而获得较高的效率。

- 无后效性：将各阶段按照一定的次序排列好之后，对于某个给定的阶段状态，它以前各阶段的状态无法直接影响它未来的决策，而只能通过当前的这个状态。换句话说，每个状态都是过去历史的一个完整总结。这就是无后向性，又称为无后效性。

总结：

- 动态规划： 动态规划表面上很难，其实存在很简单的套路：当求解的问题满足以下两个条件时， 就应该使用动态规划：
    1. 主问题的答案 包含了 可分解的子问题答案 （也就是说，问题可以被递归的思想求解）
    2. 递归求解时， 很多子问题的答案会被多次重复利用

- 动态规划的本质思想就是递归， 但如果直接应用递归方法， 子问题的答案会被重复计算产生浪费， 同时递归更加耗费栈内存， 所以通常用一个二维矩阵（表格）来表示不同子问题的答案， 以实现更加高效的求解。 

#### 滚动数组思想

滚动数组思想是DP问题中的一个常见思想。简单的理解就是让数组滚动起来，每次都使用固定的几个存储空间，来达到压缩，节省存储空间的作用。因为DP题目是一个自底向上的扩展过程，我们常常需要用到的是连续的解，前面的解往往可以舍去。所以用滚动数组优化是很有效的。

##### 示例：斐波那契数列普通方法

```c++
    int d[80];
    d[0]=1;d[1]=2;
    for(int i=2;i<90;i++)
    {
        d[i]=d[i-1]+d[i-2];
    }
    printf("%d\n",d[79]);
```

##### 示例：斐波那契数列滚动数组法

```c++
//普通版本,相当于一直在算d[i]=d[i-1]+d[i-2]
    int d[3];
    d[1]=1;d[2]=2;//此时初始化和前面不一样
    for(int i=2;i<80;i++)
    {
        //每循环一次就要把后两个的值赋给前两个
        d[0]=d[1];
        d[1]=d[2];
        d[2]=d[0]+d[1];
    }
    printf("%d\n",d[2]);
```

```c++
//进阶版本,相当于一直在算d[i]=d[i-1]+d[i-2]
    int d[3];
    d[0]=1;d[1]=2;//根据迭代公式初始化
    for(int i=2;i<80;i++)
    {
        //方法精华，滚动向前
        d[i%3]=d[(i-1)%3]+d[(i-2)%3];
    }
    printf("%d\n",d[79%3]);//根据迭代公式
```

### 思路

用 `f(i,j)` 来表示从坐标 `(0,0)` 到坐标 `(i,j)` 的路径总数，`u(i,j)` 表示坐标 `(i,j)` 是否可行，如果坐标 `(i,j)` 有障碍物，`u(i,j)=0`，否则 `u(i,j)=1`。

- 假设我们走到 `(i, j)` ，要么是从左边`(i-1, j)`的点走来，要么是从上边`(i, j-1)`的点走来。  
所以到达`(i, j)`的路径数等于到达`(i-1, j)`和`(i, j-1)`的路径之和。
- 递归公式表示为`f(i,j)=f(i-1,j)+f(i,j-1)`
- 障碍物处理：遇到障碍物`(l, k)`，则该点能提供的路径为0，表示为`f(l,k)=0`
- 初始条件：由于和最左一列和最上一行都只有一种走法：
    1. 第 1 列的格子只有从其上边格子走过去这一种走法，因此初始化 `f(i,0)=1`，存在障碍物时为 0；
    2. 第 1 行的格子只有从其左边格子走过去这一种走法，因此初始化 `f(0,j)=1`，存在障碍物时为 0。

### 代码

#### C++

```c++
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        if(obstacleGrid[0][0]==1)
        {
            return 0;
        }
        int m = obstacleGrid.size(),n = obstacleGrid.at(0).size();
        vector<vector<int> > dp(m,vector<int>(n));
        for(int i=0;i<m&&obstacleGrid[i][0]==0;i++)
        {
            dp[i][0]=1;
        }
        for(int j=0;(j<n)&&(obstacleGrid[0][j])==0;j++)
        {
            dp[0][j]=1;
        }
        for(int i=1;i<m;i++)
        {
            for(int j=1;j<n;j++)
            {
                if(obstacleGrid[i][j]==0)
                {
                    dp[i][j]=dp[i-1][j]+dp[i][j-1];
                }
            }
        }
        return dp[m-1][n-1];
    }
};
```

#### Python

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import numpy
mm = 4  # m列
nn = 3  # n行
input_data = numpy.zeros((nn, mm), dtype=numpy.int16)
# 另一种列表初始化方法：obstacleGrid=[[0 for i in range(m)] for i in range(n)]
# 障碍物初始化：置1
input_data[1][1] = 1


def uniquePathsWithObstacles(obstacleGrid):
    # 假装不知道原数据的大小,获取输入数据的列数(m)和行数(n)
    m = obstacleGrid.shape[1]
    n = obstacleGrid.shape[0]
    dp = numpy.zeros((n, m), dtype=numpy.int16)
    if(obstacleGrid[0][0] == 1):
        return dp[0][0]
    for i in range(m):
        if obstacleGrid[0][i] == 0:
            dp[0][i] = 1
        else:
            break
    for j in range(n):
        if obstacleGrid[j][0] == 0:
            dp[j][0] = 1
        else:
            break
    for i in range(1, m):
        for j in range(1, n):
            if obstacleGrid[j][i] == 0:
                dp[j][i] = dp[j-1][i]+dp[j][i-1]
    return dp[j][i]


x = uniquePathsWithObstacles(input_data)
print(x)
```
