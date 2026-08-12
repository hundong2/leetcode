#!/usr/bin/env bash

set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd -P)"
build_dir="${repo_root}/build"

if [[ -d "$build_dir" ]]; then
    find "$build_dir" -mindepth 1 -delete
    echo "[clean] build 디렉터리의 생성물을 정리했습니다."
else
    echo "[clean] 정리할 build 디렉터리가 없습니다."
fi
