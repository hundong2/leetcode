// Console과 String 같은 기본 .NET 형식을 사용하기 위해 System 네임스페이스를 가져온다.
using System;

// LeetCode가 요구하는 풀이 클래스를 선언한다.
public class Solution
{
    // 주어진 중심에서 확장한 구간의 시작점과 길이를 값 튜플로 반환한다.
    private static (int Start, int Length) ExpandFromCenter(string s, int left, int right)
    {
        // 좌우 인덱스가 유효하고 두 문자가 같은 동안 범위를 확장한다.
        while (left >= 0 && right < s.Length && s[left] == s[right])
        {
            // 왼쪽 경계를 바깥쪽으로 한 칸 옮긴다.
            left--;
            // 오른쪽 경계를 바깥쪽으로 한 칸 옮긴다.
            right++;
        // 중심 확장 while 블록을 끝낸다.
        }
        // 실패한 마지막 확장을 제외한 실제 시작점과 길이를 반환한다.
        return (left + 1, right - left - 1);
    // 중심 확장 메서드를 끝낸다.
    }

    // LeetCode가 호출하는 메서드로 가장 긴 팰린드롬을 반환한다.
    public string LongestPalindrome(string s)
    {
        // 제약 밖의 빈 문자열도 안전하게 처리한다.
        if (string.IsNullOrEmpty(s))
        {
            // 팰린드롬 부분 문자열이 없으므로 빈 문자열을 반환한다.
            return string.Empty;
        // 빈 문자열 처리 블록을 끝낸다.
        }

        // 현재 최장 구간의 시작점을 첫 문자로 초기화한다.
        int bestStart = 0;
        // 문자 하나는 항상 팰린드롬이므로 길이를 1로 초기화한다.
        int bestLength = 1;

        // 각 문자 위치를 가능한 중심으로 차례대로 검사한다.
        for (int center = 0; center < s.Length; center++)
        {
            // 한 문자를 중심으로 하는 홀수 길이 구간을 찾는다.
            (int oddStart, int oddLength) = ExpandFromCenter(s, center, center);
            // 새 홀수 구간이 기존 답보다 긴지 확인한다.
            if (oddLength > bestLength)
            {
                // 정답 시작점을 새 구간의 시작점으로 바꾼다.
                bestStart = oddStart;
                // 정답 길이를 새 구간의 길이로 바꾼다.
                bestLength = oddLength;
            // 홀수 구간 갱신 블록을 끝낸다.
            }

            // 두 문자 사이를 중심으로 하는 짝수 길이 구간을 찾는다.
            (int evenStart, int evenLength) = ExpandFromCenter(s, center, center + 1);
            // 새 짝수 구간이 기존 답보다 긴지 확인한다.
            if (evenLength > bestLength)
            {
                // 정답 시작점을 새 구간의 시작점으로 바꾼다.
                bestStart = evenStart;
                // 정답 길이를 새 구간의 길이로 바꾼다.
                bestLength = evenLength;
            // 짝수 구간 갱신 블록을 끝낸다.
            }
        // 모든 중심을 확인하는 for 블록을 끝낸다.
        }

        // 기록된 구간을 새 문자열로 만들어 반환한다.
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
        // 한 줄을 읽고 입력이 없으면 빈 문자열을 사용한다.
        string s = Console.ReadLine() ?? string.Empty;
        // LeetCode 풀이 객체를 생성한다.
        Solution solution = new Solution();
        // 계산한 가장 긴 팰린드롬을 표준 출력에 쓴다.
        Console.WriteLine(solution.LongestPalindrome(s));
    // Main 메서드를 끝낸다.
    }
// Program 클래스 정의를 끝낸다.
}
