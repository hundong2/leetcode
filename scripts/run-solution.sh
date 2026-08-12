#!/usr/bin/env bash

set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd -P)"
requested_path="${1:-}"

if [[ -z "$requested_path" ]]; then
    echo "오류: 실행할 소스 파일 경로가 필요합니다." >&2
    exit 2
fi

if [[ "$requested_path" = /* ]]; then
    source_path="$requested_path"
else
    source_path="${repo_root}/${requested_path#./}"
fi

if [[ ! -f "$source_path" ]]; then
    echo "오류: 파일을 찾을 수 없습니다: $requested_path" >&2
    exit 2
fi

source_dir="$(cd "$(dirname "$source_path")" && pwd -P)"
source_path="${source_dir}/$(basename "$source_path")"

case "$source_path" in
    "${repo_root}"/*) ;;
    *)
        echo "오류: 워크스페이스 밖의 파일은 실행하지 않습니다: $source_path" >&2
        exit 2
        ;;
esac

relative_path="${source_path#${repo_root}/}"
extension="${source_path##*.}"
relative_without_extension="${relative_path%.*}"
output_base="${repo_root}/build/${relative_without_extension}"

mkdir -p "$(dirname "$output_base")"

# 실행할 명령을 셸에서 다시 입력할 수 있는 형태로 표시한다.
print_command() {
    local label="$1"
    shift

    printf '[%s]' "$label"
    printf ' %q' "$@"
    printf '\n'
}

# 컴파일 명령과 성공·실패 여부를 함께 표시한다.
run_build() {
    print_command "빌드 명령" "$@"

    if "$@"; then
        echo "[빌드 결과] 성공"
    else
        local exit_code=$?
        echo "[빌드 결과] 실패 (종료 코드: ${exit_code})" >&2
        return "$exit_code"
    fi
}

# 프로그램의 표준 출력과 종료 코드를 구분해서 표시한다.
run_program() {
    print_command "실행 명령" "$@"
    echo "[실행 출력]"

    if "$@"; then
        echo
        echo "[실행 결과] 성공 (종료 코드: 0)"
    else
        local exit_code=$?
        echo
        echo "[실행 결과] 실패 (종료 코드: ${exit_code})" >&2
        return "$exit_code"
    fi
}

echo "[실행 대상] ${relative_path}"

case "$extension" in
    cpp|cc|cxx)
        cxx_command="$(command -v c++ || true)"
        if [[ -z "$cxx_command" ]] && command -v brew >/dev/null 2>&1; then
            cxx_command="$(brew --prefix llvm 2>/dev/null)/bin/clang++"
        fi
        if [[ ! -x "$cxx_command" ]]; then
            echo "오류: C++ 컴파일러가 없습니다. 먼저 'make env'를 실행하세요." >&2
            exit 1
        fi
        run_build "$cxx_command" -std=c++20 -O2 -Wall -Wextra -Wpedantic "$source_path" -o "$output_base"
        run_program "$output_base"
        ;;
    csx)
        dotnet_script="${repo_root}/.tools/dotnet-script/dotnet-script"
        if [[ ! -x "$dotnet_script" ]]; then
            echo "오류: dotnet-script가 없습니다. 먼저 'make env'를 실행하세요." >&2
            exit 1
        fi
        echo "[빌드 결과] 생략 (C# 스크립트)"
        run_program "$dotnet_script" "$source_path"
        ;;
    cs)
        sdk_version="$(dotnet --version)"
        sdk_major="${sdk_version%%.*}"
        project_dir="${output_base}.dotnet"
        project_file="${project_dir}/Runner.csproj"
        mkdir -p "$project_dir"
        sed \
            -e "s|__TARGET_FRAMEWORK__|net${sdk_major}.0|g" \
            -e "s|__SOURCE_FILE__|${source_path}|g" \
            "${repo_root}/scripts/templates/Runner.csproj.template" > "$project_file"
        run_build dotnet build "$project_file" --configuration Release --output "${project_dir}/out" --nologo
        run_program dotnet "${project_dir}/out/Runner.dll"
        ;;
    py)
        python_command="${repo_root}/.venv/bin/python"
        if [[ ! -x "$python_command" ]]; then
            echo "오류: uv Python 가상 환경이 없습니다. 먼저 'make env'를 실행하세요." >&2
            exit 1
        fi
        echo "[빌드 결과] 생략 (Python 인터프리터 사용)"
        run_program "$python_command" "$source_path"
        ;;
    rs)
        run_build rustc --edition=2021 -C opt-level=2 "$source_path" -o "$output_base"
        run_program "$output_base"
        ;;
    kt)
        jar_path="${output_base}.jar"
        java_command="$(command -v java || true)"
        if [[ -n "$java_command" ]] && ! "$java_command" -version >/dev/null 2>&1; then
            java_command=""
        fi
        if [[ -z "$java_command" ]] && command -v brew >/dev/null 2>&1; then
            java_command="$(brew --prefix openjdk 2>/dev/null)/bin/java"
        fi
        if [[ ! -x "$java_command" ]]; then
            echo "오류: Java 런타임이 없습니다. 먼저 'make env'를 실행하세요." >&2
            exit 1
        fi
        run_build kotlinc "$source_path" -include-runtime -d "$jar_path"
        run_program "$java_command" -jar "$jar_path"
        ;;
    kts)
        echo "[빌드 결과] 생략 (Kotlin 스크립트)"
        run_program kotlinc -script "$source_path"
        ;;
    *)
        echo "오류: 지원하지 않는 확장자입니다: .$extension" >&2
        echo "지원 확장자: .cpp .cc .cxx .cs .csx .py .rs .kt .kts" >&2
        exit 2
        ;;
esac
