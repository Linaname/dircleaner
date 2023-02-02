#include "wildcard_matcher.h"

#include <vector>

bool IsMatch(const std::string& text, const std::string& pattern) {
    int text_size = text.size();
    int pattern_size = pattern.size();
    std::vector<std::vector<bool>> dp(text_size + 1, std::vector(pattern_size + 1, false));
    dp[0][0] = true;
    for (int j = 0; j < pattern_size && pattern[j] == '*'; j++) {
        dp[0][j + 1] = true;
    }

    for (int text_idx = 1; text_idx <= text_size; ++text_idx) {
        for (int pattern_idx = 1; pattern_idx <= pattern_size; ++pattern_idx) {
            if (pattern[pattern_idx - 1] == '*') {
                dp[text_idx][pattern_idx] = dp[text_idx - 1][pattern_idx] || dp[text_idx][pattern_idx - 1];
            } else if(text[text_idx - 1] == pattern[pattern_idx - 1]) {
                dp[text_idx][pattern_idx] = dp[text_idx - 1][pattern_idx - 1];
            }
        }
    }
    return dp[text_size][pattern_size];
}