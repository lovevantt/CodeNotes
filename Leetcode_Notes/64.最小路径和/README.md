## 问题：64. 最小路径和

给定一个包含非负整数的 m x n 网格，请找出一条从左上角到右下角的路径，使得路径上的数字总和为最小。

**说明：**每次只能向下或者向右移动一步。

    输入:
    [
      [1,3,1],
      [1,5,1],
      [4,2,1]
    ]
    输出: 7
    解释: 因为路径 1→3→1→1→1 的总和最小。

## 思路

关键词：动态规划（dp）

用`dp[i][j]`表示第i行第j列的最小和,`grid[i][j]`表示某一位置的值，由于路径只能从左上到右下，所以每个地方只能接受它上方或左方传来的量，所以可以分为三种情况：
- 在最上面一行，他们只能接收从左面传递的值，所以`dp[0][i]=dp[0][i-1]+grid[0][i]`
- 在最左面一列，他们只能接收从上面传递的值，所以`dp[j][0]=dp[j-1][0]+grid[j][0]`
- 剩余部分：`dp[i][j]=min(dp[i-1][j],dp[i][j-1])+grid[i][j]`

## 代码

c++：

```c++
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int row=grid.size();
        int colunm=grid[0].size();
        vector<vector<int>> dp(row,vector<int>(colunm));
        dp[0][0]=grid[0][0];
        for(int i=1;i<row;i++)
        {
            dp[i][0]=dp[i-1][0]+grid[i][0];
        }
        for(int j=1;j<colunm;j++)
        {
            dp[0][j]=dp[0][j-1]+grid[0][j];
        }
        for(int i=1;i<row;i++)
        {
            for(int j=1;j<colunm;j++)
            {
                dp[i][j]=min(dp[i-1][j],dp[i][j-1])+grid[i][j];
            }
        }
        return dp[row-1][colunm-1];
    }
};
```