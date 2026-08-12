# 표준 입력 스트림을 사용하기 위해 파이썬 표준 모듈 sys를 가져온다.
import sys


# LeetCode가 요구하는 풀이 클래스를 정의한다.
class Solution:
    # 객체 상태를 사용하지 않는 도우미이므로 인스턴스 없이 호출 가능한 정적 메서드로 만든다.
    @staticmethod
    # 주어진 두 인덱스를 중심으로 확장한 구간의 시작점과 길이를 반환한다.
    def _expand_from_center(s: str, left: int, right: int) -> tuple[int, int]:
        # 인덱스가 범위 안에 있고 좌우 문자가 같은 동안 팰린드롬을 넓힌다.
        while left >= 0 and right < len(s) and s[left] == s[right]:
            # 왼쪽 경계를 한 칸 바깥으로 옮긴다.
            left -= 1
            # 오른쪽 경계를 한 칸 바깥으로 옮긴다.
            right += 1
        # 마지막 실패 위치를 제외한 실제 시작점과 길이를 튜플로 반환한다.
        return left + 1, right - left - 1

    # LeetCode가 호출하는 함수로 가장 긴 팰린드롬 부분 문자열을 반환한다.
    def longestPalindrome(self, s: str) -> str:
        # 원래 제약 밖인 빈 문자열을 방어적으로 확인한다.
        if not s:
            # 만들 수 있는 부분 문자열이 없으므로 빈 문자열을 반환한다.
            return ""

        # 지금까지 찾은 최장 팰린드롬의 시작점을 첫 문자로 둔다.
        best_start = 0
        # 문자 하나는 팰린드롬이므로 최장 길이를 1로 둔다.
        best_length = 1

        # range가 만드는 0 이상 len(s) 미만의 각 인덱스를 중심으로 검사한다.
        for center in range(len(s)):
            # 한 문자를 중심으로 하는 홀수 길이 팰린드롬을 찾는다.
            odd_start, odd_length = self._expand_from_center(s, center, center)
            # 홀수 구간이 지금까지의 답보다 긴지 검사한다.
            if odd_length > best_length:
                # 정답의 시작점을 새 홀수 구간으로 갱신한다.
                best_start = odd_start
                # 정답의 길이를 새 홀수 구간 길이로 갱신한다.
                best_length = odd_length

            # 두 문자 사이를 중심으로 하는 짝수 길이 팰린드롬을 찾는다.
            even_start, even_length = self._expand_from_center(s, center, center + 1)
            # 짝수 구간이 지금까지의 답보다 긴지 검사한다.
            if even_length > best_length:
                # 정답의 시작점을 새 짝수 구간으로 갱신한다.
                best_start = even_start
                # 정답의 길이를 새 짝수 구간 길이로 갱신한다.
                best_length = even_length

        # 슬라이스로 최장 구간의 새 문자열을 만들어 반환한다.
        return s[best_start : best_start + best_length]


# 이 파일이 모듈로 불린 때가 아니라 직접 실행된 때만 로컬 하네스를 실행한다.
if __name__ == "__main__":
    # 첫째 줄을 읽고 줄 끝 개행 문자만 제거한다.
    input_string = sys.stdin.readline().rstrip("\r\n")
    # 풀이 객체를 만들고 계산 결과를 출력한다.
    print(Solution().longestPalindrome(input_string))
