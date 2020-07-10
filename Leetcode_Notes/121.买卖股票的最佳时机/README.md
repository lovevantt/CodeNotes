## 问题：121. 买卖股票的最佳时机

给定一个数组，它的第 i 个元素是一支给定股票第 i 天的价格。

如果你最多只允许完成一笔交易（即买入和卖出一支股票一次），设计一个算法来计算你所能获取的最大利润。

**注意**：你不能在买入股票前卖出股票。

### 示例 1:

    输入: [7,1,5,3,6,4]
    输出: 5
    解释: 在第 2 天（股票价格 = 1）的时候买入，在第 5 天（股票价格 = 6）的时候卖出，最大利润 = 6-1 = 5 。
        注意利润不能是 7-1 = 6, 因为卖出价格需要大于买入价格；同时，你不能在买入前卖出股票。

### 示例 2:

    输入: [7,6,4,3,1]
    输出: 0
    解释: 在这种情况下, 没有交易完成, 所以最大利润为 0。

## 思路

### 动态规划

#### 动态规划做题步骤：

1. 明确 `dp(i)` 应该表示什么（二维情况：`dp(i)(j)`）；
2. 根据 `dp(i)` 和 `dp(i-1)` 的关系得出状态转移方程；
3. 确定初始条件，如 `dp(0)`。

#### 本题思路:

`dp[i]` 表示前 `i` 天的最大利润，因为我们始终要使利润最大化，则：

`dp[i] = max(dp[i-1], prices[i]-minprice)`

### 一次遍历

![avator](https://pic.leetcode-cn.com/4eaadab491f2bf88639d66c9d51bb0115e694ae08d637841ac18172b631cb21f-0121.gif)

遍历一遍数组，计算每次 **到当天为止** 的最小股票价格`minprice`和最大利润`maxvalue`。

对数组进行一次遍历，利润等于高价减低价，每此遍历到下一个元素，将该值于存储的历史最小值进行比较，并且计算一遍利润，即当前最大利润（当前值-历史最小值）与历史最大利润进行对比，取大值。
特殊情况，先讲最小值初始化为`prices[0]`。

## 代码

### 动态规划

c++：

```c++
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        if (n == 0) return 0; // 边界条件
        int minprice = prices[0];
        vector<int> dp (n, 0);

        for (int i = 1; i < n; i++){
            minprice = min(minprice, prices[i]);
            dp[i] = max(dp[i - 1], prices[i] - minprice);
        }
        return dp[n - 1];
    }
};
```

### 一次遍历

c++：

```c++
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if(prices.size()<=0)
        {
            return 0;
        }
        int maxvalue=0;
        int minprice=prices[0];
        for(int i=0;i<prices.size();i++)
        {
            minprice=min(minprice,prices[i]);
            maxvalue=max(maxvalue,prices[i]-minprice);
        }
        return maxvalue;
    }
};
```