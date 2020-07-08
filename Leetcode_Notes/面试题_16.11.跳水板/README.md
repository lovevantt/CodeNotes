## 问题：面试题 16.11. 跳水板

你正在使用一堆木板建造跳水板。有两种类型的木板，其中长度较短的木板长度为shorter，长度较长的木板长度为longer。你必须正好使用k块木板。编写一个方法，生成跳水板所有可能的长度。

返回的长度需要从小到大排列。

### 示例：

    输入：
    shorter = 1
    longer = 2
    k = 3
    输出： {3,4,5,6}

### 提示：

- 0 < shorter <= longer
- 0 <= k <= 100000

## 思路

用一个vector记录结果，用一个set存储每次计算的结果。

特殊情况有两种：

- k=0,即零块木板，返回空`[]`
- shorter=longer,长短木板一样长，返回`k*shorter`

最短的情况是使用k块短木板，然后每次减一块短木板加一块长木板，计算出长度后判断set中是否有此值，没有的话将此值同时间存入set和vector，有的话跳过进行下一循环，直到使用k块长木板进行计算。

## 代码

**c++**:

```c++
class Solution {
public:
    vector<int> divingBoard(int shorter, int longer, int k) {
        vector<int> result;//存储所有值
        set<int> value;//存关键字（长度）
        if(k==0)
        {
            return result;
        }
        else if(shorter==longer)
        {
            int length=shorter*k;
            result.push_back(length);
            return result;
        }
        for(int i=0;i<=k;i++)
        {
            int length=(k-i)*shorter+i*longer;
            if(value.count(length)==0)
            {
                value.insert(length);
                result.push_back(length); 
            }
        }
        return result;
    }
};
```

**python**:

```python
class Solution:
    def divingBoard(self, shorter: int, longer: int, k: int) -> List[int]:
        value=set()
        result=[]
        if k==0:
            return []
        elif shorter==longer:
            return [shorter*k]
        for i in range(k+1):
            if((i*longer+(k-i)*shorter) not in value):
                value.add(i*longer+(k-i)*shorter)
                result.append(i*longer+(k-i)*shorter)
        #result=list(value)        
        return result
```