include <bits/stdc++.h>

using namespace std;

//-RECIPE-START-//
// Answer range based sum queries in O((N+Q)*sqrt(N))
// Can be adapted to deal with any range based queries where
// The numbers in the input array don't change
// The value for [l, r+1) and similar can be computed quickly from [l, r)
// Queries are known in advance
struct Mo {
    vector<int> nums;
    int curr_value;
    int l, r; // Current range [l, r)
    // Change the following functions to change the algorithm.
    void left_inc() { curr_value -= nums[l++]; }
    void left_dec() { curr_value += nums[--l]; }
    void right_inc() { curr_value += nums[r++]; }
    void right_dec() { curr_value -= nums[--r]; }
    Mo(const vector<int> &nums) : nums(nums) {
        curr_value = 0, l = 0, r = 0;
    }
    // Assumes the queries are [left, right)
    vector<int> process_queries(const vector<pair<int, int>> &Qranges) {
        const int BLOCK_SIZE = sqrt(nums.size());
        vector<tuple<int, int, int>> Qs; // left, right, index
        for (int i = 0; i < Qranges.size(); ++i) 
            Qs.push_back(make_tuple(Qranges[i].first, Qranges[i].second, i));
        // Magic sort order that ensures we only do O(sqrt(N)) window movements per query.
        sort(Qs.begin(), Qs.end(), [BLOCK_SIZE](const tuple<int, int, int> &x,
            const tuple<int, int, int> &y) {
            int block_x = std::get<0>(x) / BLOCK_SIZE;
            int block_y = std::get<0>(y) / BLOCK_SIZE;
            if(block_x != block_y)
                return block_x < block_y;
            return std::get<1>(x) < std::get<1>(y);
        });
        vector<int> answers(Qs.size());
        for(auto &q: Qs) {
            int left, right, answer_idx;
            tie(left, right, answer_idx) = q;
            while(r < right) right_inc();
            while(r > right) right_dec();
            while(l < left) left_inc();
            while(l > left) left_dec();
            answers[answer_idx] = curr_value;
        }
        return answers;
    }
};
// Answers queries by breaking an array into sqrt sized blocks.
struct SQRTdecomp {
    vector<int> blocks, nums;
    int block_size;
    SQRTdecomp(const vector<int> &nums) : nums(nums) {
        block_size = sqrt(nums.size());
        int i = 0;
        while (i < nums.size()) {
            int sum = 0;
            for (int j = 0; j < block_size; ++j) {
                sum += nums[i];
                ++i;
            }
            blocks.push_back(sum);
        }
    }
    int query(int l, int r) { // [l, r)
        --r; // To make the range inclusive
        int lblock = l/block_size, rblock = r/block_size;
		int ans = 0;
		if (lblock == rblock) {
			for (int i = l; i <= r; ++i)
				ans += nums[i];
		} else {
			for (int i = l; i < (lblock+1)*block_size; ++i)
				ans += nums[i];
			for (int i = rblock*block_size; i <= r; ++i)
				ans += nums[i];
			for (int i = lblock+1; i < rblock; ++i)
				ans += blocks[i];
		}
		return ans;
    }
};
//-RECIPE-END-//

void test() {
    default_random_engine re;
    for (int t = 0; t < 10000; ++t) {
        vector<int> nums(50);
        for (int &n : nums) n = re()%20-5;
        vector<pair<int, int>> Qs(50);
        for (auto &p : Qs) {
            p.second = re()%nums.size()+1;
            p.first = re()%p.second;
        }
        Mo solver(nums);
        auto moAns = solver.process_queries(Qs);
        SQRTdecomp decomp(nums);
        vector<int> naive_ans, sqrt_ans;
        for (auto &p : Qs) {
            int sum = 0;
            for (int i = p.first; i < p.second; ++i) {
                sum += nums[i];
            }
            naive_ans.push_back(sum);
            sqrt_ans.push_back(decomp.query(p.first, p.second));
        }
        for (int i : nums) cout << i << " ";
        cout << endl;
        for (int i : naive_ans) cout << i << " ";
        cout << endl;
        for (int i : moAns) cout << i << " ";
        cout << endl;
        for (int i : sqrt_ans) cout << i << " ";
        cout << endl;
        cout << endl;
        assert(naive_ans == moAns);
        assert(naive_ans == sqrt_ans);
    }
    cout << "All tests passed" << endl;
}

int main() {
    test();
}
