/*
暴力做法：可以在遍历数组的时候，当i-k+1>=0（表示当前滑动窗口的大小达到了k），就遍历i-k+1 ~ i的所有元素，寻找最大值，
然后把最大值压入到结果数组res中，显然这种做法需要两重循环，时间复杂度为O(n*k)。

我们考虑一下优化，暴力做法在遍历数组的时候还需要在内层循环遍历来计算当前窗口内的最大值，而计算当前窗口的最大值，我们可以用
单调队列进行优化。

考虑这样一种情况：在长度为k的滑动窗口内，我们要寻找最大值：我们可以从前往后寻找，如果有这样一种情况，下标i < j，且nums[i] <= nums[j]，
则nums[i]肯定不是答案，因为nums[j]比nums[i]大。
不过在下一次遍历数组的时候，又（可能）在下一个滑动窗口内比较nums[i]和nums[j]了，虽然我们在之前就知道了只要有nums[j]在，nums[i]就肯定
不是答案了。
所以我们可以用单调队列来记录某个滑动窗口内的元素的大小关系，把当前窗口内的元素入队（其实是把下标入队），每个元素入队之前都会和队尾元素进行比较，
如果当前元素比队尾元素大，则队尾元素肯定不是当前滑动窗口内的最大值（当然也不会是后面的滑动窗口的最大值），我们就一直删除队尾元素，直到队列
为空或者队尾元素比当前元素大。这时再把当前元素（的下标）入队，这样，我们的队列里的元素（下标）的值（nums[下标])就是单调递减的，所以队头元素
就是当前滑动窗口内的最大值。  
我们只需要注意当滑动窗口移动的时候判断窗口左端(i - k + 1)和队头元素的关系，如果i - k + 1大于q.front(),说明当前
窗口滑动到下一个位置，之前记录的队头元素下标已经过期了，那就要出队。
通过这种方式，我们就维护了一个能够快速找到当前滑动窗口内元素最大值的双端队列。

时间复杂度分析：由于遍历一遍数组，每个元素最多入队和出队一次，所以时间复杂度为O(n)。
*/

class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector<int> res;
        deque<int> q;                                          //双端队列里存的是下标，不是值
        for(int i = 0; i < nums.size(); ++i) {
            if(q.size() && q.front() < i - k + 1) {                  //窗口滑动，之前的队头元素过期了，出队
                q.pop_front();
            }
            while(q.size() && nums[q.back()] <= nums[i]) {           //当前元素比之前队列记录的队尾元素大，则队尾元素出队，因为他肯定不是答案
                q.pop_back();
            }
            q.push_back(i);
            if(i - k + 1 >= 0) {                                    //如果窗口大小已经为k，则肯定有一个最大值，记录到res数组中
                res.push_back(nums[q.front()]);
            }
        }
        return res;
    }
};
