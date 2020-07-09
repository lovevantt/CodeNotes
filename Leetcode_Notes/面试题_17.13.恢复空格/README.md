## 问题：面试题 17.13. 恢复空格

哦，不！你不小心把一个长篇文章中的空格、标点都删掉了，并且大写也弄成了小写。像句子`"I reset the computer. It still didn’t boot!"`已经变成了`"iresetthecomputeritstilldidntboot"`。在处理标点符号和大小写之前，你得先把它断成词语。当然了，你有一本厚厚的词典`dictionary`，不过，有些词没在词典里。假设文章用`sentence`表示，设计一个算法，把文章断开，要求未识别的字符最少，返回未识别的字符数。

**注意**：本题相对原题稍作改动，只需返回未识别的字符数。

### 示例：

    输入：
    dictionary = ["looked","just","like","her","brother"]
    sentence = "jesslookedjustliketimherbrother"
    输出： 7
    解释： 断句后为"jess looked just like tim her brother"，共7个未识别字符。

### 提示：

- `0 <= len(sentence) <= 1000`
- `dictionary` 中总字符数不超过 `150000`。
- 你可以认为 `dictionary` 和 `sentence` 中只包含小写字母。

## 思路

关键字：动态规划（dp）、字典树Trie

### 1.状态定义：

`dp[i]` 表示字符串的前 `i` 个字符的最少未匹配数。

### 2、状态转移：

假设当前我们已经考虑完了前 `i` 个字符了，对于前 `i + 1` 个字符对应的最少未匹配数：

1. 第 `i + 1` 个字符未匹配，则 `dp[i + 1] = dp[i] + 1`，即不匹配数加 1;
2. 遍历前 `i` 个字符，若以其中某一个下标 `idx` 为开头、以第 `i + 1` 个字符为结尾的字符串正好在词典里，则 `dp[i] = min(dp[i], dp[idx])` 更新 `dp[i]`。

### 3.初步解法：

```c++
class Solution {
    public int respace(String[] dictionary, String sentence) {
        Set<String> dict = new HashSet<>(Arrays.asList(dictionary));
        int n = sentence.length();
        int[] dp = new int[n + 1];
        for (int i = 1; i <= n; i++) {
            dp[i] = dp[i - 1] + 1;
            for (int idx = 0; idx < i; idx++) {
                if (dict.contains(sentence.substring(idx, i))) {
                    dp[i] = Math.min(dp[i], dp[idx]);
                }
            }
        }
        return dp[n];
    }
}
```

### 4.使用 Trie 进行优化：

对于上述解法，计算 `dp[i + 1]` 时，我们需要用 `idx` 来遍历前 `i` 个字符，逐个判断以 `idx` 为开头，以第 `i + 1` 个字符为结尾的字符串是否在字典里。  
这一步可以利用字典树来加速，通过字典树我们可以查询以第 `i + 1` 个字符为结尾的单词有哪些（构建字典树时将单词逆序插入即可）。  
关于字典树的学习，可以看下 [这篇题解](https://leetcode-cn.com/problems/short-encoding-of-words/solution/99-java-trie-tu-xie-gong-lue-bao-jiao-bao-hui-by-s/)，这就是解法二。  
时间复杂度是 `O(m + n^2)`，`m` 是字典长度，`n` 为待匹配字符串的长度。为什么还是 `n^2` 呢？因为有可能状态转移的时候，每个位置都需要转移，这是最坏情况，绝大多数情况下远小于 `n` ，所以解法二最终耗时会远小于解法一。

## 代码：

c++:
```c++
class Trie {
public:
    Trie* next[26] = {nullptr};
    bool isEnd;
    
    Trie() {
        isEnd = false;
    }

    void insert(string s) {
        Trie* curPos = this;

        for (int i = s.length() - 1; i >= 0; --i) {
            int t = s[i] - 'a';
            if (curPos->next[t] == nullptr) {
                curPos->next[t] = new Trie();
            }
            curPos = curPos->next[t];
        }
        curPos->isEnd = true;
    }
};

class Solution {
public:
    int respace(vector<string>& dictionary, string sentence) {
        int n = sentence.length(), inf = 0x3f3f3f3f;

        Trie* root = new Trie();
        for (auto& word: dictionary) {
            root->insert(word);
        }

        vector<int> dp(n + 1, inf);
        dp[0] = 0;
        for (int i = 1; i <= n; ++i) {
            dp[i] = dp[i - 1] + 1;

            Trie* curPos = root;
            for (int j = i; j >= 1; --j) {
                int t = sentence[j - 1] - 'a';
                if (curPos->next[t] == nullptr) {
                    break;
                } else if (curPos->next[t]->isEnd) {
                    dp[i] = min(dp[i], dp[j - 1]);
                }
                if (dp[i] == 0) {
                    break;
                }
                curPos = curPos->next[t];
            }
        }
        return dp[n];
    }
};
```

java:
```java
class Solution {
    public int respace(String[] dictionary, String sentence) {
        // 构建字典树
        Trie trie = new Trie();
        for (String word: dictionary) {
            trie.insert(word);
        }
        // 状态转移，dp[i] 表示字符串的前 i 个字符的最少未匹配数
        int n = sentence.length();
        int[] dp = new int[n + 1];
        for (int i = 1; i <= n; i++) {
            dp[i] = dp[i - 1] + 1;
            for (int idx: trie.search(sentence, i - 1)) {
                dp[i] = Math.min(dp[i], dp[idx]);
            }
        }
        return dp[n];
    }
}

class Trie {
    TrieNode root;

    public Trie() {
        root = new TrieNode();
    }

    // 将单词倒序插入字典树
    public void insert(String word) {
        TrieNode cur = root;
        for (int i = word.length() - 1; i >= 0; i--) {
            int c = word.charAt(i) - 'a';
            if (cur.children[c] == null) {
                cur.children[c] = new TrieNode();
            }
            cur = cur.children[c];
        }
        cur.isWord = true;
    }

    // 找到 sentence 中以 endPos 为结尾的单词，返回这些单词的开头下标。
    public List<Integer> search(String sentence, int endPos) {
        List<Integer> indices = new ArrayList<>(); 
        TrieNode cur = root;
        for (int i = endPos; i >= 0; i--) {
            int c = sentence.charAt(i) - 'a';
            if (cur.children[c] == null) {
                break;
            }
            cur = cur.children[c];
            if (cur.isWord) {
                indices.add(i);
            }  
        }
        return indices;
    }
}

class TrieNode {
    boolean isWord;
    TrieNode[] children = new TrieNode[26];

    public TrieNode() {}
}
```