// LeetCode가 요구하는 풀이 클래스를 선언한다.
class Solution {
    // 주어진 두 위치를 중심으로 확장해 시작점과 길이를 Pair로 반환한다.
    private fun expandFromCenter(s: String, initialLeft: Int, initialRight: Int): Pair<Int, Int> {
        // 확장하면서 값이 바뀌므로 왼쪽 경계를 가변 변수 var로 복사한다.
        var left = initialLeft
        // 확장하면서 값이 바뀌므로 오른쪽 경계를 가변 변수 var로 복사한다.
        var right = initialRight
        // 인덱스가 범위 안에 있고 두 문자가 같은 동안 팰린드롬을 넓힌다.
        while (left >= 0 && right < s.length && s[left] == s[right]) {
            // 왼쪽 경계를 바깥으로 한 칸 옮긴다.
            left--
            // 오른쪽 경계를 바깥으로 한 칸 옮긴다.
            right++
        // 중심 확장 while 블록을 끝낸다.
        }
        // 실패한 마지막 위치를 제외한 시작점과 길이를 불변 Pair로 묶어 반환한다.
        return Pair(left + 1, right - left - 1)
    // 중심 확장 함수를 끝낸다.
    }

    // LeetCode가 호출하는 함수로 가장 긴 팰린드롬 부분 문자열을 반환한다.
    fun longestPalindrome(s: String): String {
        // 제약 밖의 빈 문자열도 방어적으로 처리한다.
        if (s.isEmpty()) {
            // 만들 수 있는 부분 문자열이 없으므로 빈 문자열을 반환한다.
            return ""
        // 빈 문자열 처리 블록을 끝낸다.
        }

        // 최장 구간을 찾을 때 갱신하므로 시작점을 가변 변수로 둔다.
        var bestStart = 0
        // 문자 하나는 팰린드롬이므로 최장 길이를 1로 둔다.
        var bestLength = 1

        // 문자열의 각 유효 인덱스를 가능한 중심으로 차례대로 검사한다.
        for (center in s.indices) {
            // 한 문자를 중심으로 하는 홀수 길이 구간을 찾는다.
            val (oddStart, oddLength) = expandFromCenter(s, center, center)
            // 홀수 구간이 지금까지의 답보다 긴지 확인한다.
            if (oddLength > bestLength) {
                // 정답 시작점을 새 홀수 구간으로 갱신한다.
                bestStart = oddStart
                // 정답 길이를 새 홀수 구간 길이로 갱신한다.
                bestLength = oddLength
            // 홀수 구간 갱신 블록을 끝낸다.
            }

            // 두 문자 사이를 중심으로 하는 짝수 길이 구간을 찾는다.
            val (evenStart, evenLength) = expandFromCenter(s, center, center + 1)
            // 짝수 구간이 지금까지의 답보다 긴지 확인한다.
            if (evenLength > bestLength) {
                // 정답 시작점을 새 짝수 구간으로 갱신한다.
                bestStart = evenStart
                // 정답 길이를 새 짝수 구간 길이로 갱신한다.
                bestLength = evenLength
            // 짝수 구간 갱신 블록을 끝낸다.
            }
        // 모든 중심을 확인하는 for 블록을 끝낸다.
        }

        // 시작 인덱스를 포함하고 끝 인덱스를 제외하는 substring으로 정답을 반환한다.
        return s.substring(bestStart, bestStart + bestLength)
    // LeetCode 풀이 함수를 끝낸다.
    }
// Solution 클래스 정의를 끝낸다.
}

// 저장소에서 표준 입력으로 풀이를 직접 실행하기 위한 프로그램 진입점이다.
fun main() {
    // readlnOrNull의 nullable 결과가 null이면 Elvis 연산자 ?:로 빈 문자열을 사용한다.
    val s = readlnOrNull() ?: ""
    // 풀이 객체를 만들고 계산한 결과를 한 줄 출력한다.
    println(Solution().longestPalindrome(s))
// main 함수 블록을 끝낸다.
}
