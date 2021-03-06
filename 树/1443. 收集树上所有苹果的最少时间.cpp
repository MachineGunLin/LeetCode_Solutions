class Solution {
public:
    int minTime(int n, vector<vector<int>>& edges, vector<bool>& hasApple) {
        int res = 0;
        for(int i = edges.size() - 1; i >= 0; i--) {
            if(hasApple[edges[i][1]] == true) {
                hasApple[edges[i][0]] = true;
            }
        }
        for(int i = 0; i < edges.size(); i++) {
            if(hasApple[edges[i][1]] == true) {
                res += 2;
            }
        }
        return res;
    }
};
