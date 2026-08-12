// 표준 입력과 출력을 사용하기 위해 iostream 헤더를 포함한다.
#include <iostream>
// 두 정수를 한 번에 반환하는 std::pair를 사용하기 위해 utility 헤더를 포함한다.
#include <utility>
// 가변 길이 문자열인 std::string을 사용하기 위해 string 헤더를 포함한다.
#include <string>

// LeetCode가 요구하는 풀이 클래스를 선언한다.
class Solution {
    // 중심 확장 도우미는 클래스 내부에서만 사용하도록 비공개 영역에 둔다.
private:
    // 주어진 두 위치를 중심으로 확장해 팰린드롬의 시작점과 길이를 구한다.
    static std::pair<int, int> expandFromCenter(const std::string& s, int left, int right) {
        // 두 인덱스가 범위 안에 있고 문자가 같은 동안 팰린드롬을 바깥으로 넓힌다.
        while (left >= 0 && right < static_cast<int>(s.size()) && s[left] == s[right]) {
            // 왼쪽 경계를 한 칸 바깥으로 옮긴다.
            --left;
            // 오른쪽 경계를 한 칸 바깥으로 옮긴다.
            ++right;
        // 중심 확장 반복 블록을 끝낸다.
        }
        // 마지막 확장은 실패했으므로 실제 시작점 left + 1과 길이 right - left - 1을 반환한다.
        return {left + 1, right - left - 1};
    // 중심 확장 함수를 끝낸다.
    }

    // 채점기가 호출할 풀이 함수는 클래스 외부에서 접근하도록 공개 영역에 둔다.
public:
    // LeetCode가 호출하는 함수로 가장 긴 팰린드롬 부분 문자열을 반환한다.
    std::string longestPalindrome(const std::string& s) {
        // 원래 제약 밖이지만 빈 문자열에서도 안전하게 빈 결과를 반환한다.
        if (s.empty()) {
            // 만들 수 있는 부분 문자열이 없으므로 빈 문자열을 반환한다.
            return "";
        // 빈 문자열 예외 처리 블록을 끝낸다.
        }

        // 지금까지 찾은 최장 팰린드롬의 시작 인덱스를 첫 문자로 초기화한다.
        int bestStart = 0;
        // 문자 하나는 항상 팰린드롬이므로 최장 길이를 1로 초기화한다.
        int bestLength = 1;

        // 모든 문자 위치를 홀수 및 짝수 팰린드롬의 중심 후보로 검사한다.
        for (int center = 0; center < static_cast<int>(s.size()); ++center) {
            // 같은 위치에서 시작해 홀수 길이 팰린드롬을 구한다.
            const auto [oddStart, oddLength] = expandFromCenter(s, center, center);
            // 더 긴 홀수 팰린드롬을 찾았는지 확인한다.
            if (oddLength > bestLength) {
                // 최장 답의 시작 위치를 새 구간으로 갱신한다.
                bestStart = oddStart;
                // 최장 답의 길이를 새 구간의 길이로 갱신한다.
                bestLength = oddLength;
            // 홀수 길이 갱신 조건 블록을 끝낸다.
            }

            // 인접한 두 위치에서 시작해 짝수 길이 팰린드롬을 구한다.
            const auto [evenStart, evenLength] = expandFromCenter(s, center, center + 1);
            // 더 긴 짝수 팰린드롬을 찾았는지 확인한다.
            if (evenLength > bestLength) {
                // 최장 답의 시작 위치를 새 구간으로 갱신한다.
                bestStart = evenStart;
                // 최장 답의 길이를 새 구간의 길이로 갱신한다.
                bestLength = evenLength;
            // 짝수 길이 갱신 조건 블록을 끝낸다.
            }
        // 모든 중심을 검사하는 반복 블록을 끝낸다.
        }

        // 기록한 시작점과 길이를 이용해 정답 부분 문자열을 만들어 반환한다.
        return s.substr(static_cast<std::size_t>(bestStart), static_cast<std::size_t>(bestLength));
    // LeetCode 풀이 함수를 끝낸다.
    }
// Solution 클래스 정의를 끝낸다.
};

// 저장소의 make run으로 풀이를 직접 시험하기 위한 프로그램 진입점이다.
int main() {
    // 표준 입력 한 줄을 저장할 문자열을 선언한다.
    std::string s;
    // 공백 가능성까지 포함해 첫째 줄 전체를 입력 문자열로 읽는다.
    std::getline(std::cin, s);
    // Windows 줄 끝에서 남을 수 있는 캐리지 리턴 문자를 제거한다.
    if (!s.empty() && s.back() == '\r') {
        // 마지막 캐리지 리턴 한 글자를 삭제한다.
        s.pop_back();
    // 줄 끝 정리 조건 블록을 끝낸다.
    }
    // 풀이 객체를 생성한다.
    Solution solution;
    // 가장 긴 팰린드롬을 계산해 한 줄로 출력한다.
    std::cout << solution.longestPalindrome(s) << '\n';
    // 프로그램이 정상적으로 끝났음을 운영체제에 알린다.
    return 0;
// main 함수 블록을 끝낸다.
}
