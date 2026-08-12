// 두 정수 중 작은 값을 고르는 minOf 함수를 사용해 Manacher 알고리즘을 구현한다.

// LeetCode가 요구하는 풀이 클래스를 선언한다.
class Solution {
    // 홀수와 짝수 팰린드롬을 같은 방식으로 처리할 변환 문자열을 만든다.
    private fun buildTransformedString(s: String): String {
        // 최종 크기를 지정한 StringBuilder로 불필요한 내부 버퍼 재할당을 피한다.
        val builder = StringBuilder(s.length * 2 + 3)
        // 입력에 없는 시작 감시 문자로 확장 중 왼쪽 경계 검사를 없앤다.
        builder.append('^')
        // 원본 문자열의 각 문자를 순서대로 방문한다.
        for (character in s) {
            // 문자 사이 구분자를 넣어 모든 팰린드롬의 길이를 홀수로 통일한다.
            builder.append('#')
            // 현재 원본 문자를 구분자 뒤에 추가한다.
            builder.append(character)
        // 원본 문자를 변환 문자열에 추가하는 반복 블록을 끝낸다.
        }
        // 마지막 구분자와 입력에 없는 끝 감시 문자를 붙인다.
        builder.append("#$")
        // 가변 빌더의 내용을 불변 String으로 변환해 반환한다.
        return builder.toString()
    // 변환 문자열 생성 함수를 끝낸다.
    }

    // Manacher 알고리즘으로 가장 긴 팰린드롬 부분 문자열을 반환한다.
    fun longestPalindrome(s: String): String {
        // 원래 제약 밖의 빈 문자열도 방어적으로 처리한다.
        if (s.isEmpty()) {
            // 만들 수 있는 부분 문자열이 없으므로 빈 문자열을 반환한다.
            return ""
        // 빈 문자열 처리 블록을 끝낸다.
        }

        // val로 선언해 알고리즘 중 변환 문자열 참조가 바뀌지 않도록 한다.
        val transformed = buildTransformedString(s)
        // 각 중심의 팰린드롬 반지름을 0으로 초기화한 기본형 IntArray를 만든다.
        val radius = IntArray(transformed.length)
        // 이후 값이 갱신되므로 가장 오른쪽 팰린드롬의 중심을 var로 선언한다.
        var center = 0
        // 해당 팰린드롬이 포함하는 가장 오른쪽 인덱스를 저장한다.
        var rightBoundary = 0
        // 동률일 때 가장 왼쪽 답을 유지하도록 최적 시작점을 0으로 둔다.
        var bestStart = 0
        // 문자 하나는 항상 팰린드롬이므로 최적 길이를 1로 둔다.
        var bestLength = 1

        // 끝 감시 문자를 제외하도록 until로 상한 미포함 인덱스 범위를 만든다.
        for (index in 1 until transformed.lastIndex) {
            // index가 기존 오른쪽 경계 안이면 대칭 위치의 계산 결과를 재사용한다.
            if (index < rightBoundary) {
                // 현재 중심을 기준으로 index와 대칭인 위치를 계산한다.
                val mirror = 2 * center - index
                // 경계를 넘지 않는 범위에서 대칭 반지름과 남은 거리 중 작은 값을 취한다.
                radius[index] = minOf(rightBoundary - index, radius[mirror])
            // 대칭 정보 재사용 조건 블록을 끝낸다.
            }

            // 감시 문자 덕분에 별도 경계 검사 없이 같은 문자인 동안 확장한다.
            while (
                transformed[index + radius[index] + 1] ==
                    transformed[index - radius[index] - 1]
            ) {
                // 현재 중심의 팰린드롬 반지름을 한 칸 늘린다.
                radius[index]++
            // 중심 확장 반복 블록을 끝낸다.
            }

            // 이번 팰린드롬이 기존에 알려진 오른쪽 경계를 넘어서는지 확인한다.
            if (index + radius[index] > rightBoundary) {
                // 가장 오른쪽 팰린드롬의 중심을 현재 위치로 갱신한다.
                center = index
                // 새 팰린드롬의 오른쪽 끝을 재사용 경계로 기록한다.
                rightBoundary = index + radius[index]
            // 오른쪽 경계 갱신 조건 블록을 끝낸다.
            }

            // 변환 문자열의 반지름은 원본 팰린드롬의 길이와 같다.
            val candidateLength = radius[index]
            // 구분자를 제거했을 때 원본 문자열에서의 시작 인덱스를 계산한다.
            val candidateStart = (index - candidateLength) / 2
            // 더 길거나, 길이가 같으면서 더 왼쪽인 후보만 최적 답으로 선택한다.
            if (
                candidateLength > bestLength ||
                    (candidateLength == bestLength && candidateStart < bestStart)
            ) {
                // 최적 답의 시작점을 현재 후보로 갱신한다.
                bestStart = candidateStart
                // 최적 답의 길이를 현재 후보로 갱신한다.
                bestLength = candidateLength
            // 최적 답 갱신 조건 블록을 끝낸다.
            }
        // 모든 중심을 검사하는 반복 블록을 끝낸다.
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
