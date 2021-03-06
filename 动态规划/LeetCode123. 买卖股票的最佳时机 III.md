# LeetCode123. 买卖股票的最佳时机 III

![](https://img2020.cnblogs.com/blog/2078361/202008/2078361-20200808153940669-717902440.png)

由于只能进行两次交易，且交易之间不能有交集，所以可以视为两段独立的交易时间，因此我们可以枚举所有的两次交易的“分界点”，比如我们以第二次交易的开始时间作为分界点，枚举从开头到当前这个点的所有时间内，如果只进行一次交易所能获得的最大收益（这就是第一笔交易所能获得的最大收益）。然后再从后往前枚举，更新从分界点之后进行一笔交易所能获得的最大收益（这就是第二笔交易所能获得的最大收益）。

两次收益相加就能获得总的最大收益。

我们可以用一个数组f表示第一笔交易的最大收益，即f[i]表示从第0~i-1中的某天买入，第i天卖出，进行一笔交易的最大收益。
这个数组可以用O(n)的时间预处理，方法和[LeetCode121. 买卖股票的最佳时机](https://www.cnblogs.com/linrj/p/13457252.html)相同。

预处理第一笔交易的最大收益之后，我们就可以从后往前枚举第二笔交易的起点，这个步骤和f数组是相似的，只不过是倒着计算的，时间复杂度也是O(n)，
然后把两笔交易的收益加到一起，更新总的收益的最大值。

代码如下：

```
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        vector<int> f(n);
        for(int i = 0, lowestPrice = INT_MAX; i < n; ++i) {
            if(i == 0) {                              //递推边界，特殊处理
                f[i] = 0;                             //第0天之前无法交易，所以截止到第0天，可获得的最大收益为0
                lowestPrice = prices[0];              //更新最低价格，方便后面的计算
                continue;
            }
            f[i] = max(f[i - 1], prices[i] - lowestPrice);      //截止到第i天，进行一笔交易的最大收益为 截止到第i-1天可获得的最大收益和在价格最低点买入、第i天卖出两种方案收益的较大值
            lowestPrice = min(lowestPrice, prices[i]);          //更新最低价格
        }
        int res = 0;
        for(int i = n - 1, highestPrice = 0; i >= 0; --i) {      //从后往前枚举第二笔交易的买入时间
            res = max(res,highestPrice - prices[i] + f[i]);      //highestPrice - prices[i]表示第i天买入，在之后的一个价格较高点highestPrice卖出。 加上f[i]表示加上第一笔交易的收益
            highestPrice = max(highestPrice, prices[i]);           //更新最高价格
        }
        return res;
    }
};
```


这题还可以用状态机dp来做，可以参考[LeetCode188. 买卖股票的最佳时机 IV](https://www.cnblogs.com/linrj/p/13456892.html).
我们用一个三维的数组dp表示某个状态所能获得的最大收益： vector<vector<vector<int>>> dp(n, vector<vector<int>>(3, vector<int>(2)));
其中第一维n是prices数组的大小，表示总的天数。
第二维表示当前允许进行的最大交易次数，这题限制了可交易次数最多为2，所以截止到某一天的最高交易次数只有0，1，2三种情况。
这里的交易次数，我们认为是买入股票的次数。
第三维我们用0表示持有股票，1表示不持有股票。
所以**状态dp[i][j][0]的意思就是当前为第i天（i从0开始），截止到当前的最高交易次数为j，当前不持有股票 的最高收益**。
**状态dp[i][j][1]的意思就是当前为第i天，截止到当前的最高交易次数为j，当前持有股票 的最高收益**。

**对于一个不持有股票的状态dp[i][j][0]**，它是由两种状态转移而来，即（1）前一天就不持有股票，然后今天什么也没做，这种情况下有:**dp[i][j][0] = dp[i - 1][j][0];**
（2）前一天持有股票，今天把股票买了，这种情况下有:**dp[i][j][0] = dp[i - 1][j][1] + prices[i];** 
加上prices[i]表示卖出股票获得了prices[i]的收益。
由于我们的dp表示的是当前状态能获得的最大收益，因此我们需要对这两种状态取一个max: **dp[i][j][0] = max(dp[i - 1][j][0], dp[i - 1][j][1] + prices[i]);**

**对于一个持有股票的状态dp[i][j][1]**，它也是由两种状态转移而来，即（1）前一天就持有股票了，今天什么也没做，这种情况下有：**dp[i][j][1] = dp[i - 1][j][1];**
（2）前一天不持有股票，今天买了一支股票，这种情况下有:**dp[i][j][1] = dp[i - 1][j - 1][0] - prices[i];** 
注意：我们的状态定义里，买入一只股票就等于交易次数加一，所以这里是从dp[i - 1][j - 1][0]转移来的，是j - 1不是j!
减去prices[i]表示我们花了prices[i]的价格（成本）买了只股票。
由于我们的dp表示的是当前状态能够获得的最大收益，因此我们需要对这两种状态取一个max: **dp[i][j][1] = max(dp[i - 1][j][1], dp[i - 1][j - 1][0] - prices[i];**

有了状态转移方程之后，我们要考虑一下递推边界。
因为我们的状态转移方程里dp[i][][]的状态依赖于dp[i - 1][][]的值，在i为0（第0天）的时候会数组越界，所以我们需要特殊处理一下第0天的状态。

由于j的所有取值只有三种：0，1，2（当前的最大交易次数），第三维只有0，1两种取值，所以我们可以直接把这六个状态都给出来。
dp[0][0][0] = dp[0][1][0] = dp[0][2][0] = 0; 表示第0天，不持有股票的最大收益都是0。
dp[0][0][1]表示第0天，没进行交易，持有股票的状态，由于第0天没进行交易（买入股票）是不可能持有股票的，所以我们用INT_MIN表示这种状态：dp[0][0][1] = INT_MIN;
dp[0][1][1]表示第0天，进行了一次交易，持有股票的状态，也就是第0天花了prices[i]的价格买了一只股票，我们有:dp[0][1][1] = -prices[i];
dp[0][2][1]表示第0天，进行了两次交易，持有股票的状态，虽然第0天只可能进行一次交易，但是为了方便后面的计算，我们还是要处理第二维为2的情况，这个状态我们可以理解为第
0天买了两次股票，然后当天就卖了一次股票，然后还剩一次股票的情况，所以等价于dp[0][1][1]，我们有: dp[0][2][1] = -prices[0];

递推边界有了，状态转移方程也有了，我们就可以开始状态转移了。

递推之后，**最后的答案就是dp[n - 1][2][0], 表示最后一天，最多进行两次交易，不持有股票的收益。**
为什么不是dp[n - 1][2][1]呢？因为第三维的1表示持有股票，而买股票需要花钱的，只有把股票卖了才能获得收益。
所以dp[n - 1][2][0]的值肯定大于dp[n - 1][2][1]。

代码如下：

```
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        if(n == 0) {
            return 0;
        }
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(3, vector<int>(2)));
        dp[0][0][0] = 0, dp[0][1][0] = 0, dp[0][2][0] = 0;
        dp[0][0][1] = INT_MIN, dp[0][1][1] = -prices[0], dp[0][2][1] = -prices[0];
        for(int i = 1; i < n; ++i) {
            for(int j = 1; j <= 2; ++j) {            //枚举最大交易次数为1和2能获得的最大收益，0就不用枚举了，因为没交易就没收益。而且会导致数组越界
                dp[i][j][0] = max(dp[i - 1][j][0], dp[i - 1][j][1] + prices[i]);
                dp[i][j][1] = max(dp[i - 1][j][1], dp[i - 1][j - 1][0] - prices[i]);
            }
        }
        return dp[n - 1][2][0];
    }
};
```