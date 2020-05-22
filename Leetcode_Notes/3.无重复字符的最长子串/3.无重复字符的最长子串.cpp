#include<string>
using namespace std;
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int start,end;
        start=end=0;
        int maxlength=1;
        if(s.size()==0)
        {
            return 0;
        }
        for(int i=0;i<s.size();i++)
        {
            for(int j=start;j<end;j++)
            {
                if(s[j]==s[end])//若首尾相等就把首字母跳过
                {
                    start=j+1;
                }
            }
            end++;
            maxlength=max(maxlength,end-start);
        }
        return maxlength;
    }
    int max(int a,int b)
    {
        if(a>b)
        {
            return a;
        }
        else
        {
            return b;
        }
    }
};