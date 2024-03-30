class Solution {
public:
    void add_word(string& i)
    {
        _m[i] = ++idx;
        int idx_temp1 = idx;
        for (auto& j : i)
        {
            auto temp = j;
            j = '?';
            if (_m.find(i) != _m.end())
            {
                int idx_temp2 = _m[i];
                j = temp;
                _v[idx_temp1].emplace_back(idx_temp2);
                _v[idx_temp2].emplace_back(idx_temp1);
                continue;
            }
            _m[i] = ++idx;
            int idx_temp2 = idx;
            _v[idx_temp1].emplace_back(idx_temp2);
            _v[idx_temp2].emplace_back(idx_temp1);
            j = temp;
        }
    }

    int ladderLength(string s1, string s2, vector<string>& s3) {
        _v.resize((s3.size() + 1) * (s3[0].length() + 1));
        _f.resize((s3.size() + 1) * (s3[0].length() + 1));
        
        for (auto& i : s3) { 
            add_word(i); }
        add_word(s1);

        if (_m.find(s2) == _m.end()) { return 0; }
        _que.emplace(_m[s1]);
        _f[_m[s1]] = 1;
        int aim_idx = _m[s2];
        while (!_que.empty()) 
        {
            ++result;
            int len = _que.size();
            for (int z = 0; z < len; ++z)
            {
                int idx = _que.front();
                if (idx == aim_idx) { return _f[aim_idx]/2+1; }
                _que.pop();
                for (auto& i : _v[idx])
                {
                    if (_f[i] == 0) { _f[i] = result; _que.push(i); }
                }
            }
        }
        return 0;
    }
private:
    unordered_map<string, int> _m;
    vector<vector<int>> _v;
    vector<int> _f;
    queue<int> _que;
    int idx = -1;
    int result = 0;
};
