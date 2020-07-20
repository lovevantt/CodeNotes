## 问题：167. 两数之和 II - 输入有序数组

给定一个已按照升序排列 的有序数组，找到两个数使得它们相加之和等于目标数。

函数应该返回这两个下标值 index1 和 index2，其中 index1 必须小于 index2。

**说明：**

- 返回的下标值（index1 和 index2）不是从零开始的。
- 你可以假设每个输入只对应唯一的答案，而且你不可以重复使用相同的元素。

**示例:**

    输入: numbers = [2, 7, 11, 15], target = 9
    输出: [1,2]
    解释: 2 与 7 之和等于目标数 9 。因此 index1 = 1, index2 = 2 。

## 思路

双指针：首尾各一个指针，并比较`numbers[i]+numbers[j]`与`target`的大小：

- 当`numbers[i]+numbers[j]>target`时，j=j-1
- 当`numbers[i]+numbers[j]>target`时，i=i+1
- 当`numbers[i]+numbers[j]=target`时，返回`[i+1,j+1]`

## 代码

python：

```python
def twosums(numbers,target):
    i=0
    j=len(numbers)-1
    while i<j:
        if numbers[i]+numbers[j]<target:
            i=i+1
        elif numbers[i]+numbers[j]>target:
            j=j-1
        else:
            return [i+1,j+1]
numbers = [2, 7, 11, 15]
target = 9
print(twosums(numbers,target))
```