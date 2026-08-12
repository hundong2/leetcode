// 두 값 중 작은 값을 고르는 min 함수를 가져온다.
use std::cmp::min;
// 표준 입력 전체를 읽는 read_to_string 메서드를 사용하기 위해 Read 트레이트를 가져온다.
use std::io::{self, Read};

// LeetCode 풀이 메서드를 묶을 빈 구조체를 선언한다.
struct Solution;

// Solution 구조체에 관련 함수를 구현한다.
impl Solution {
    // 홀수와 짝수 팰린드롬을 같은 방식으로 처리할 바이트 배열을 만든다.
    fn build_transformed_bytes(bytes: &[u8]) -> Vec<u8> {
        // 정확한 용량을 미리 확보해 Vec가 커질 때 발생할 재할당을 피한다.
        let mut transformed = Vec::with_capacity(bytes.len() * 2 + 3);
        // 입력에 없는 시작 감시 바이트로 확장 중 왼쪽 경계 검사를 없앤다.
        transformed.push(b'^');
        // 바이트 슬라이스를 빌려 각 값을 복사하는 패턴으로 원본을 순회한다.
        for &byte in bytes {
            // 문자 사이 구분자를 넣어 모든 팰린드롬의 길이를 홀수로 통일한다.
            transformed.push(b'#');
            // 현재 원본 바이트를 구분자 뒤에 추가한다.
            transformed.push(byte);
        // 원본 바이트를 변환 배열에 추가하는 반복 블록을 끝낸다.
        }
        // 마지막 구분자와 입력에 없는 끝 감시 바이트를 슬라이스로 추가한다.
        transformed.extend_from_slice(b"#$");
        // 완성된 Vec의 소유권을 호출자에게 반환한다.
        transformed
    // 변환 바이트 배열 생성 함수를 끝낸다.
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
        // 홀수와 짝수 중심을 통합한 연속 바이트 배열을 만든다.
        let transformed = Self::build_transformed_bytes(bytes);
        // 각 중심의 팰린드롬 반지름을 저장할 usize 배열을 0으로 초기화한다.
        let mut radius = vec![0usize; transformed.len()];
        // 지금까지 가장 오른쪽까지 도달한 팰린드롬의 중심을 저장한다.
        let mut center = 0usize;
        // 해당 팰린드롬이 포함하는 가장 오른쪽 인덱스를 저장한다.
        let mut right_boundary = 0usize;
        // 동률일 때 가장 왼쪽 답을 유지하도록 최적 시작점을 0으로 둔다.
        let mut best_start = 0usize;
        // 문자 하나는 항상 팰린드롬이므로 최적 길이를 1로 둔다.
        let mut best_length = 1usize;

        // 양끝 감시 문자를 제외한 모든 위치를 팰린드롬 중심으로 검사한다.
        for index in 1..transformed.len() - 1 {
            // index가 기존 오른쪽 경계 안이면 대칭 위치의 계산 결과를 재사용한다.
            if index < right_boundary {
                // 현재 중심을 기준으로 index와 대칭인 유효 위치를 계산한다.
                let mirror = 2 * center - index;
                // 경계를 넘지 않는 범위에서 대칭 반지름과 남은 거리 중 작은 값을 취한다.
                radius[index] = min(right_boundary - index, radius[mirror]);
            // 대칭 정보 재사용 조건 블록을 끝낸다.
            }

            // 감시 바이트 덕분에 별도 경계 검사 없이 같은 바이트인 동안 확장한다.
            while transformed[index + radius[index] + 1]
                == transformed[index - radius[index] - 1]
            {
                // 현재 중심의 팰린드롬 반지름을 한 칸 늘린다.
                radius[index] += 1;
            // 중심 확장 반복 블록을 끝낸다.
            }

            // 이번 팰린드롬이 기존에 알려진 오른쪽 경계를 넘어서는지 확인한다.
            if index + radius[index] > right_boundary {
                // 가장 오른쪽 팰린드롬의 중심을 현재 위치로 갱신한다.
                center = index;
                // 새 팰린드롬의 오른쪽 끝을 재사용 경계로 기록한다.
                right_boundary = index + radius[index];
            // 오른쪽 경계 갱신 조건 블록을 끝낸다.
            }

            // 변환 배열의 반지름은 원본 팰린드롬의 길이와 같다.
            let candidate_length = radius[index];
            // 구분자를 제거했을 때 원본 문자열에서의 시작 인덱스를 계산한다.
            let candidate_start = (index - candidate_length) / 2;
            // 더 길거나, 길이가 같으면서 더 왼쪽인 후보만 최적 답으로 선택한다.
            if candidate_length > best_length
                || (candidate_length == best_length && candidate_start < best_start)
            {
                // 최적 답의 시작점을 현재 후보로 갱신한다.
                best_start = candidate_start;
                // 최적 답의 길이를 현재 후보로 갱신한다.
                best_length = candidate_length;
            // 최적 답 갱신 조건 블록을 끝낸다.
            }
        // 모든 중심을 검사하는 반복 블록을 끝낸다.
        }

        // ASCII 문자 경계와 같은 원본 바이트 구간을 새 String으로 복사해 반환한다.
        s[best_start..best_start + best_length].to_owned()
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
    let s = input.trim_end_matches(['\r', '\n']).to_owned();
    // 계산한 가장 긴 팰린드롬을 println! 매크로로 한 줄 출력한다.
    println!("{}", Solution::longest_palindrome(s));
// main 함수 블록을 끝낸다.
}
