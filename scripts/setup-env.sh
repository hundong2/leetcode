#!/usr/bin/env bash

set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd -P)"
tools_dir="${repo_root}/.tools"
venv_dir="${repo_root}/.venv"

has_command() {
    command -v "$1" >/dev/null 2>&1
}

install_with_brew() {
    local package="$1"
    shift

    if ! has_command brew; then
        echo "오류: '$package' 설치가 필요하지만 Homebrew를 찾지 못했습니다." >&2
        echo "Homebrew를 설치한 뒤 'make env'를 다시 실행하세요: https://brew.sh" >&2
        exit 1
    fi

    echo "[env] 누락된 $package 패키지를 설치합니다."
    brew install "$@"
}

if [[ "$(uname -s)" != "Darwin" ]]; then
    echo "오류: 자동 설치는 현재 워크스페이스 환경인 macOS만 지원합니다." >&2
    echo "make, C++20 컴파일러, .NET SDK, uv, Rust, Kotlin/JVM을 설치한 뒤 다시 실행하세요." >&2
    exit 1
fi

if ! has_command make; then
    install_with_brew "make" make
fi

if ! has_command c++ && [[ ! -x "$(brew --prefix llvm 2>/dev/null)/bin/clang++" ]]; then
    install_with_brew "LLVM C++ 컴파일러" llvm
fi

if ! has_command dotnet; then
    if ! has_command brew; then
        echo "오류: .NET SDK 설치를 위해 Homebrew가 필요합니다: https://brew.sh" >&2
        exit 1
    fi
    echo "[env] 누락된 .NET SDK를 설치합니다."
    brew install --cask dotnet-sdk
fi

if ! has_command uv; then
    install_with_brew "uv" uv
fi

if ! has_command rustc; then
    install_with_brew "Rust" rust
fi

if ! has_command kotlinc; then
    install_with_brew "Kotlin" kotlin
fi

if ! java -version >/dev/null 2>&1 && [[ ! -x "$(brew --prefix openjdk 2>/dev/null)/bin/java" ]]; then
    install_with_brew "OpenJDK" openjdk
fi

mkdir -p "$tools_dir"

if [[ ! -x "${tools_dir}/dotnet-script/dotnet-script" ]]; then
    echo "[env] 워크스페이스 전용 dotnet-script를 설치합니다."
    mkdir -p "${tools_dir}/dotnet-script"
    dotnet tool install dotnet-script --tool-path "${tools_dir}/dotnet-script"
fi

if [[ ! -x "${venv_dir}/bin/python" ]]; then
    echo "[env] uv로 워크스페이스 전용 Python 가상 환경을 만듭니다."
    uv venv "$venv_dir"
fi

mkdir -p "${repo_root}/build"

echo "[env] 실행 환경이 준비되어 있습니다. 이미 설치된 항목은 변경하지 않았습니다."
