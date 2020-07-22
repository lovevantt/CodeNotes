## 问题：剑指 Offer 11. 旋转数组的最小数字

把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。输入一个递增排序的数组的一个旋转，输出旋转数组的最小元素。例如，数组 [3,4,5,1,2] 为 [1,2,3,4,5] 的一个旋转，该数组的最小值为1。  

**示例1：**

    输入：[3,4,5,1,2]
    输出：1

**示例2：**

    输入：[2,2,2,0,1]
    输出：0

注意：本题与主站 154 题相同：https://leetcode-cn.com/problems/find-minimum-in-rotated-sorted-array-ii/

## 思路

关键词：二分法、遍历

## 代码

c++：

本质是把有序数组的前n项放到了队尾，假设最小值为`nums[0]`，若从后遍历，只需找到数值由小变大的突变。

```c++
class Solution {
public:
    int minArray(vector<int>& numbers) {
        int minnumber=numbers[0];
        for(int i=numbers.size()-1;i>=0;i--)
        {
            //minnumber=minnumber<=numbers[i]?minnumber:numbers[i];
            if(minnumber>=numbers[i])
            {
                minnumber=numbers[i];
            }
            else
            {
                break;
            }
        }
        return minnumber;
    }
};
```