// 두 정수 중 작은 값을 고르는 std::min을 사용하기 위해 algorithm 헤더를 포함한다.
#include <algorithm>
// 표준 입력과 출력을 사용하기 위해 iostream 헤더를 포함한다.
#include <iostream>
// 가변 길이 문자열인 std::string을 사용하기 위해 string 헤더를 포함한다.
#include <string>
// 연속 메모리 동적 배열인 std::vector를 사용하기 위해 vector 헤더를 포함한다.
#include <vector>

// LeetCode가 요구하는 풀이 클래스를 선언한다.
class Solution {
    // 내부 구현 도우미는 클래스 밖에서 호출할 필요가 없으므로 비공개 영역에 둔다.
private:
    // 홀수와 짝수 팰린드롬을 같은 방식으로 처리할 변환 문자열을 만든다.
    static std::string buildTransformedString(const std::string& s) {
        // 입력을 복사하지 않고 읽기 전용 참조로 받아 새 변환 문자열만 만든다.
        std::string transformed;
        // 필요한 크기를 미리 확보해 문자열이 커질 때 발생할 재할당을 피한다.
        transformed.reserve(s.size() * 2 + 3);
        // 입력에 없는 시작 감시 문자로 확장 중 왼쪽 경계 검사를 없앤다.
        transformed.push_back('^');
        // 범위 기반 반복문으로 원본의 각 문자를 차례대로 방문한다.
        for (const char character : s) {
            // 문자 사이 구분자를 넣어 모든 팰린드롬의 길이를 홀수로 통일한다.
            transformed.push_back('#');
            // 현재 원본 문자를 구분자 뒤에 추가한다.
            transformed.push_back(character);
        // 원본 문자를 변환 문자열에 추가하는 반복 블록을 끝낸다.
        }
        // 마지막 구분자와 입력에 없는 끝 감시 문자를 한 번에 붙인다.
        transformed.append("#$");
        // 완성된 문자열의 소유권을 호출자에게 반환한다.
        return transformed;
    // 변환 문자열 생성 함수를 끝낸다.
    }

    // 채점기가 호출할 풀이 함수는 클래스 외부에서 접근하도록 공개 영역에 둔다.
public:
    // Manacher 알고리즘으로 가장 긴 팰린드롬 부분 문자열을 반환한다.
    std::string longestPalindrome(const std::string& s) {
        // 원래 제약 밖이지만 빈 문자열에서도 안전하게 빈 결과를 반환한다.
        if (s.empty()) {
            // 만들 수 있는 부분 문자열이 없으므로 빈 문자열을 반환한다.
            return "";
        // 빈 문자열 예외 처리 블록을 끝낸다.
        }

        // const로 선언해 알고리즘 중 변환 문자열이 바뀌지 않음을 보장한다.
        const std::string transformed = buildTransformedString(s);
        // 각 중심에서의 팰린드롬 반지름을 0으로 초기화한 연속 동적 배열을 만든다.
        std::vector<int> radius(transformed.size(), 0);
        // 지금까지 가장 오른쪽까지 도달한 팰린드롬의 중심을 저장한다.
        int center = 0;
        // 해당 팰린드롬이 포함하는 가장 오른쪽 인덱스를 저장한다.
        int rightBoundary = 0;
        // 동률일 때 가장 왼쪽 답을 유지하도록 최적 시작점을 0으로 둔다.
        int bestStart = 0;
        // 문자 하나는 항상 팰린드롬이므로 최적 길이를 1로 둔다.
        int bestLength = 1;

        // 양끝 감시 문자를 제외한 모든 위치를 팰린드롬 중심으로 검사한다.
        for (int index = 1; index + 1 < static_cast<int>(transformed.size()); ++index) {
            // 현재 중심을 기준으로 index와 대칭인 위치를 계산한다.
            const int mirror = 2 * center - index;
            // index가 기존 오른쪽 경계 안이면 대칭 위치의 계산 결과를 재사용한다.
            if (index < rightBoundary) {
                // 경계를 넘지 않는 범위에서 대칭 반지름과 남은 거리 중 작은 값을 취한다.
                radius[index] = std::min(rightBoundary - index, radius[mirror]);
            // 대칭 정보 재사용 조건 블록을 끝낸다.
            }

            // 다음 좌우 문자가 같은 동안 아직 확인하지 않은 범위를 확장한다.
            while (transformed[index + radius[index] + 1]
                   == transformed[index - radius[index] - 1]) {
                // 현재 중심의 팰린드롬 반지름을 한 칸 늘린다.
                ++radius[index];
            // 중심 확장 반복 블록을 끝낸다.
            }

            // 이번 팰린드롬이 기존에 알려진 오른쪽 경계를 넘어서는지 확인한다.
            if (index + radius[index] > rightBoundary) {
                // 가장 오른쪽 팰린드롬의 중심을 현재 위치로 갱신한다.
                center = index;
                // 새 팰린드롬의 오른쪽 끝을 재사용 경계로 기록한다.
                rightBoundary = index + radius[index];
            // 오른쪽 경계 갱신 조건 블록을 끝낸다.
            }

            // 변환 문자열의 반지름은 원본 팰린드롬의 길이와 같다.
            const int candidateLength = radius[index];
            // 구분자를 제거했을 때 원본 문자열에서의 시작 인덱스를 계산한다.
            const int candidateStart = (index - candidateLength) / 2;
            // 더 길거나, 길이가 같으면서 더 왼쪽인 후보만 최적 답으로 선택한다.
            if (candidateLength > bestLength
                || (candidateLength == bestLength && candidateStart < bestStart)) {
                // 최적 답의 시작점을 현재 후보로 갱신한다.
                bestStart = candidateStart;
                // 최적 답의 길이를 현재 후보로 갱신한다.
                bestLength = candidateLength;
            // 최적 답 갱신 조건 블록을 끝낸다.
            }
        // 모든 중심을 검사하는 반복 블록을 끝낸다.
        }

        // 기록한 원본 구간으로 새 문자열을 만들어 반환한다.
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
