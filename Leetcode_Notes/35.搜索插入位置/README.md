## 问题：35. 搜索插入位置

给定一个排序数组和一个目标值，在数组中找到目标值，并返回其索引。如果目标值不存在于数组中，返回它将会被按顺序插入的位置。

你可以假设数组中无重复元素。

**示例1：**

    输入: [1,3,5,6], 5
    输出: 2

**示例2：**

    输入: [1,3,5,6], 2
    输出: 1

**示例3：**

    输入: [1,3,5,6], 7
    输出: 4

**示例4：**

    输入: [1,3,5,6], 0
    输出: 0

## 解题思路

关键词：**二分法**

## 代码

遍历：
```c++
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        for(int i=0;i<nums.size();i++)
        {
            if(nums[i]>=target)
            {
                return i;
            }
        }
        return nums.size();
    }
};
```

二分法(python3)：
```python
class Solution:
    def searchInsert(self, nums: List[int], target: int) -> int:
        low=0
        high=len(nums)
        mid=0
        while(low<high):
            mid=low+(high-low)//2
            if(nums[mid]<target):
                low=mid+1
            elif(nums[mid]>=target):
                high=mid
        return low
```