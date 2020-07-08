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