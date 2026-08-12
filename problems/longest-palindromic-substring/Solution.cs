// Console, Math와 String 같은 기본 .NET 형식을 사용하기 위해 System 네임스페이스를 가져온다.
using System;
// 재할당을 줄이며 문자열을 조립하는 StringBuilder를 사용하기 위해 Text 네임스페이스를 가져온다.
using System.Text;

// LeetCode가 요구하는 풀이 클래스를 선언한다.
public class Solution
{
    // 홀수와 짝수 팰린드롬을 같은 방식으로 처리할 변환 문자열을 만든다.
    private static string BuildTransformedString(string s)
    {
        // 최종 크기를 지정해 내부 문자 버퍼의 불필요한 재할당을 피한다.
        StringBuilder builder = new StringBuilder(s.Length * 2 + 3);
        // 입력에 없는 시작 감시 문자로 확장 중 왼쪽 경계 검사를 없앤다.
        builder.Append('^');
        // foreach로 원본 문자열의 각 문자를 순서대로 방문한다.
        foreach (char character in s)
        {
            // 문자 사이 구분자를 넣어 모든 팰린드롬의 길이를 홀수로 통일한다.
            builder.Append('#');
            // 현재 원본 문자를 구분자 뒤에 추가한다.
            builder.Append(character);
        // 원본 문자를 변환 문자열에 추가하는 반복 블록을 끝낸다.
        }
        // 마지막 구분자와 입력에 없는 끝 감시 문자를 붙인다.
        builder.Append("#$");
        // 가변 버퍼를 불변 string으로 변환해 반환한다.
        return builder.ToString();
    // 변환 문자열 생성 메서드를 끝낸다.
    }

    // Manacher 알고리즘으로 가장 긴 팰린드롬 부분 문자열을 반환한다.
    public string LongestPalindrome(string s)
    {
        // 원래 제약 밖의 null이나 빈 문자열도 안전하게 처리한다.
        if (string.IsNullOrEmpty(s))
        {
            // 팰린드롬 부분 문자열이 없으므로 빈 문자열을 반환한다.
            return string.Empty;
        // 빈 문자열 처리 블록을 끝낸다.
        }

        // 변환 문자열은 이후 수정하지 않는 지역 변수로 유지한다.
        string transformed = BuildTransformedString(s);
        // 각 중심의 팰린드롬 반지름을 저장하는 고정 크기 연속 배열을 만든다.
        int[] radius = new int[transformed.Length];
        // 지금까지 가장 오른쪽까지 도달한 팰린드롬의 중심을 저장한다.
        int center = 0;
        // 해당 팰린드롬이 포함하는 가장 오른쪽 인덱스를 저장한다.
        int rightBoundary = 0;
        // 동률일 때 가장 왼쪽 답을 유지하도록 최적 시작점을 0으로 둔다.
        int bestStart = 0;
        // 문자 하나는 항상 팰린드롬이므로 최적 길이를 1로 둔다.
        int bestLength = 1;

        // 양끝 감시 문자를 제외한 모든 위치를 팰린드롬 중심으로 검사한다.
        for (int index = 1; index + 1 < transformed.Length; index++)
        {
            // 현재 중심을 기준으로 index와 대칭인 위치를 계산한다.
            int mirror = 2 * center - index;
            // index가 기존 오른쪽 경계 안이면 대칭 위치의 계산 결과를 재사용한다.
            if (index < rightBoundary)
            {
                // 경계를 넘지 않는 범위에서 대칭 반지름과 남은 거리 중 작은 값을 취한다.
                radius[index] = Math.Min(rightBoundary - index, radius[mirror]);
            // 대칭 정보 재사용 조건 블록을 끝낸다.
            }

            // 감시 문자 덕분에 별도 경계 검사 없이 같은 문자인 동안 확장할 수 있다.
            while (transformed[index + radius[index] + 1]
                == transformed[index - radius[index] - 1])
            {
                // 현재 중심의 팰린드롬 반지름을 한 칸 늘린다.
                radius[index]++;
            // 중심 확장 반복 블록을 끝낸다.
            }

            // 이번 팰린드롬이 기존에 알려진 오른쪽 경계를 넘어서는지 확인한다.
            if (index + radius[index] > rightBoundary)
            {
                // 가장 오른쪽 팰린드롬의 중심을 현재 위치로 갱신한다.
                center = index;
                // 새 팰린드롬의 오른쪽 끝을 재사용 경계로 기록한다.
                rightBoundary = index + radius[index];
            // 오른쪽 경계 갱신 조건 블록을 끝낸다.
            }

            // 변환 문자열의 반지름은 원본 팰린드롬의 길이와 같다.
            int candidateLength = radius[index];
            // 구분자를 제거했을 때 원본 문자열에서의 시작 인덱스를 계산한다.
            int candidateStart = (index - candidateLength) / 2;
            // 더 길거나, 길이가 같으면서 더 왼쪽인 후보만 최적 답으로 선택한다.
            if (candidateLength > bestLength
                || (candidateLength == bestLength && candidateStart < bestStart))
            {
                // 최적 답의 시작점을 현재 후보로 갱신한다.
                bestStart = candidateStart;
                // 최적 답의 길이를 현재 후보로 갱신한다.
                bestLength = candidateLength;
            // 최적 답 갱신 조건 블록을 끝낸다.
            }
        // 모든 중심을 검사하는 반복 블록을 끝낸다.
        }

        // 기록된 원본 구간을 새 문자열로 만들어 반환한다.
        return s.Substring(bestStart, bestLength);
    // LeetCode 풀이 메서드를 끝낸다.
    }
// Solution 클래스 정의를 끝낸다.
}

// 로컬에서 표준 입력으로 풀이를 실행하기 위한 진입점 클래스를 선언한다.
public static class Program
{
    // .NET 프로그램이 시작되는 Main 메서드를 선언한다.
    public static void Main()
    {
        // 한 줄을 읽고 입력이 없으면 null 병합 연산자로 빈 문자열을 사용한다.
        string s = Console.ReadLine() ?? string.Empty;
        // 풀이 객체를 생성한다.
        Solution solution = new Solution();
        // 계산한 가장 긴 팰린드롬을 표준 출력에 쓴다.
        Console.WriteLine(solution.LongestPalindrome(s));
    // Main 메서드를 끝낸다.
    }
// Program 클래스 정의를 끝낸다.
}
