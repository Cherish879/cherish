class Solution {
public:
    int maxBottlesDrunk(int s1, int s2) {
        int result = s1;
        int empty = s1;
        int full = 0;
        while (1) 
        {
            if (empty < s2)
                return result;
            while (empty >= s2)
            {
                ++full;
                empty -= s2++;
            }
            result += full;
            empty += full;
            full = 0;
        }
    }
};
