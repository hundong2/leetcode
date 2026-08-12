# 표준 입력 스트림을 사용하기 위해 파이썬 표준 모듈 sys를 가져온다.
import sys


# LeetCode가 요구하는 풀이 클래스를 정의한다.
class Solution:
    # 객체 상태를 사용하지 않는 도우미이므로 정적 메서드로 만든다.
    @staticmethod
    # 홀수와 짝수 팰린드롬을 같은 방식으로 처리할 변환 문자열을 만든다.
    def _build_transformed_string(s: str) -> str:
        # append가 평균 O(1)인 list에 시작 감시 문자를 먼저 넣는다.
        characters: list[str] = ["^"]
        # 원본 문자열의 각 문자를 순서대로 방문한다.
        for character in s:
            # 문자 사이 구분자를 넣어 모든 팰린드롬의 길이를 홀수로 통일한다.
            characters.append("#")
            # 현재 원본 문자를 구분자 뒤에 추가한다.
            characters.append(character)
        # 마지막 구분자와 입력에 없는 끝 감시 문자를 추가한다.
        characters.extend(("#", "$"))
        # 반복적인 문자열 덧셈 대신 join으로 문자들을 한 번에 결합한다.
        return "".join(characters)

    # LeetCode가 호출하는 함수로 가장 긴 팰린드롬 부분 문자열을 반환한다.
    def longestPalindrome(self, s: str) -> str:
        # 원래 제약 밖인 빈 문자열을 방어적으로 확인한다.
        if not s:
            # 만들 수 있는 부분 문자열이 없으므로 빈 문자열을 반환한다.
            return ""

        # 변환 문자열을 한 번 만들어 홀수와 짝수 중심을 통합한다.
        transformed = self._build_transformed_string(s)
        # 각 중심의 팰린드롬 반지름을 저장할 연속 list를 0으로 초기화한다.
        radius = [0] * len(transformed)
        # 지금까지 가장 오른쪽까지 도달한 팰린드롬의 중심을 저장한다.
        center = 0
        # 해당 팰린드롬이 포함하는 가장 오른쪽 인덱스를 저장한다.
        right_boundary = 0
        # 동률일 때 가장 왼쪽 답을 유지하도록 최적 시작점을 0으로 둔다.
        best_start = 0
        # 문자 하나는 항상 팰린드롬이므로 최적 길이를 1로 둔다.
        best_length = 1

        # range로 양끝 감시 문자를 제외한 모든 중심 인덱스를 만든다.
        for index in range(1, len(transformed) - 1):
            # 현재 중심을 기준으로 index와 대칭인 위치를 계산한다.
            mirror = 2 * center - index
            # index가 기존 오른쪽 경계 안이면 대칭 위치의 계산 결과를 재사용한다.
            if index < right_boundary:
                # 경계를 넘지 않는 범위에서 대칭 반지름과 남은 거리 중 작은 값을 취한다.
                radius[index] = min(right_boundary - index, radius[mirror])

            # 감시 문자 덕분에 별도 경계 검사 없이 같은 문자인 동안 확장할 수 있다.
            while (
                transformed[index + radius[index] + 1]
                == transformed[index - radius[index] - 1]
            ):
                # 현재 중심의 팰린드롬 반지름을 한 칸 늘린다.
                radius[index] += 1

            # 이번 팰린드롬이 기존에 알려진 오른쪽 경계를 넘어서는지 확인한다.
            if index + radius[index] > right_boundary:
                # 가장 오른쪽 팰린드롬의 중심을 현재 위치로 갱신한다.
                center = index
                # 새 팰린드롬의 오른쪽 끝을 재사용 경계로 기록한다.
                right_boundary = index + radius[index]

            # 변환 문자열의 반지름은 원본 팰린드롬의 길이와 같다.
            candidate_length = radius[index]
            # 정수 나눗셈으로 원본 문자열에서의 시작 인덱스를 계산한다.
            candidate_start = (index - candidate_length) // 2
            # 더 길거나, 길이가 같으면서 더 왼쪽인 후보만 최적 답으로 선택한다.
            if candidate_length > best_length or (
                candidate_length == best_length and candidate_start < best_start
            ):
                # 최적 답의 시작점을 현재 후보로 갱신한다.
                best_start = candidate_start
                # 최적 답의 길이를 현재 후보로 갱신한다.
                best_length = candidate_length

        # 슬라이스로 최장 원본 구간의 새 문자열을 만들어 반환한다.
        return s[best_start : best_start + best_length]


# 이 파일이 모듈로 불린 때가 아니라 직접 실행된 때만 로컬 하네스를 실행한다.
if __name__ == "__main__":
    # 첫째 줄을 읽고 줄 끝 개행 문자만 제거한다.
    input_string = sys.stdin.readline().rstrip("\r\n")
    # 풀이 객체를 만들고 계산 결과를 출력한다.
    print(Solution().longestPalindrome(input_string))
