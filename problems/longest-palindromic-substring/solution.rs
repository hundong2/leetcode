// 표준 입력 전체를 읽는 read_to_string 메서드를 사용하기 위해 Read 트레이트를 가져온다.
use std::io::{self, Read};

// LeetCode 풀이 메서드를 묶을 빈 구조체를 선언한다.
struct Solution;

// Solution 구조체에 관련 함수를 구현한다.
impl Solution {
    // 바이트 슬라이스를 빌려 중심 확장 결과인 시작점과 길이를 반환한다.
    fn expand_from_center(bytes: &[u8], mut left: isize, mut right: isize) -> (usize, usize) {
        // 인덱스가 범위 안에 있고 좌우 바이트가 같은 동안 범위를 넓힌다.
        while left >= 0
            && right < bytes.len() as isize
            && bytes[left as usize] == bytes[right as usize]
        {
            // 왼쪽 경계를 바깥으로 한 칸 옮긴다.
            left -= 1;
            // 오른쪽 경계를 바깥으로 한 칸 옮긴다.
            right += 1;
        // 중심 확장 while 블록을 끝낸다.
        }
        // 마지막 실패 위치를 제외한 시작점을 음수 없는 usize로 변환한다.
        let start = (left + 1) as usize;
        // 두 실패 경계 사이에 남은 실제 팰린드롬 길이를 계산한다.
        let length = (right - left - 1) as usize;
        // 두 결과를 튜플로 묶어 반환한다.
        (start, length)
    // 중심 확장 함수를 끝낸다.
    }

    // LeetCode가 호출하는 메서드로 입력 String의 소유권을 받아 정답 String을 반환한다.
    pub fn longest_palindrome(s: String) -> String {
        // 제약 밖의 빈 문자열도 안전하게 처리한다.
        if s.is_empty() {
            // 입력의 소유권을 그대로 반환해 불필요한 새 할당을 피한다.
            return s;
        // 빈 문자열 처리 블록을 끝낸다.
        }

        // ASCII로 제한된 입력을 복사하지 않고 바이트 슬라이스로 빌린다.
        let bytes = s.as_bytes();
        // 현재 최장 구간의 시작점을 첫 바이트로 초기화한다.
        let mut best_start = 0usize;
        // 한 문자는 항상 팰린드롬이므로 길이를 1로 초기화한다.
        let mut best_length = 1usize;

        // 모든 바이트 위치를 가능한 중심으로 검사한다.
        for center in 0..bytes.len() {
            // 한 문자를 중심으로 하는 홀수 길이 팰린드롬을 찾는다.
            let (odd_start, odd_length) =
                Self::expand_from_center(bytes, center as isize, center as isize);
            // 새 홀수 구간이 기존 답보다 긴지 검사한다.
            if odd_length > best_length {
                // 정답 시작점을 새 홀수 구간의 시작점으로 갱신한다.
                best_start = odd_start;
                // 정답 길이를 새 홀수 구간의 길이로 갱신한다.
                best_length = odd_length;
            // 홀수 구간 갱신 블록을 끝낸다.
            }

            // 두 문자 사이를 중심으로 하는 짝수 길이 팰린드롬을 찾는다.
            let (even_start, even_length) =
                Self::expand_from_center(bytes, center as isize, center as isize + 1);
            // 새 짝수 구간이 기존 답보다 긴지 검사한다.
            if even_length > best_length {
                // 정답 시작점을 새 짝수 구간의 시작점으로 갱신한다.
                best_start = even_start;
                // 정답 길이를 새 짝수 구간의 길이로 갱신한다.
                best_length = even_length;
            // 짝수 구간 갱신 블록을 끝낸다.
            }
        // 모든 중심을 확인하는 for 블록을 끝낸다.
        }

        // ASCII 문자 경계와 같은 바이트 구간을 빌린 뒤 새 String으로 복사해 반환한다.
        s[best_start..best_start + best_length].to_string()
    // LeetCode 풀이 메서드를 끝낸다.
    }
// Solution 구현 블록을 끝낸다.
}

// 저장소에서 표준 입력으로 풀이를 실행하기 위한 프로그램 진입점이다.
fn main() {
    // 표준 입력 전체를 저장할 가변 String을 만든다.
    let mut input = String::new();
    // 표준 입력 핸들을 빌려 모든 내용을 문자열에 읽고 실패하면 명확한 메시지로 종료한다.
    io::stdin()
        .read_to_string(&mut input)
        .expect("표준 입력을 읽을 수 없습니다.");
    // 줄 끝의 개행과 캐리지 리턴만 제거한 슬라이스를 새 String으로 만든다.
    let s = input.trim_end_matches(['\r', '\n']).to_string();
    // 계산한 가장 긴 팰린드롬을 println! 매크로로 한 줄 출력한다.
    println!("{}", Solution::longest_palindrome(s));
// main 함수 블록을 끝낸다.
}
