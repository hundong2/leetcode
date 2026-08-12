SHELL := /bin/bash

.DEFAULT_GOAL := help

# `make run file.cpp`에서 run 뒤에 전달된 모든 경로를 실행 대상으로 취급한다.
RUN_FILES := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

# Make가 파일 경로를 별도의 목표로 해석하므로 빈 규칙을 등록해 오류를 막는다.
ifneq ($(filter run,$(MAKECMDGOALS)),)
.PHONY: $(RUN_FILES)
$(foreach source,$(RUN_FILES),$(eval $(source):;@:))
endif

.PHONY: help env run clean

help:
	@echo "사용법:"
	@echo "  make env                         누락된 실행 환경을 한 번만 구성합니다."
	@echo "  make run <파일경로>             빌드·실행 명령과 결과를 표시합니다."
	@echo "  make run *.cpp                  일치하는 여러 파일을 차례로 실행합니다."
	@echo "  make clean                       생성된 build 디렉터리를 삭제합니다."
	@echo
	@echo "지원 확장자: .cpp .cc .cxx .cs .csx .py .rs .kt .kts"

env:
	@./scripts/setup-env.sh

run: env
	@if [[ -z "$(strip $(RUN_FILES))" ]]; then \
		echo "오류: make run 뒤에 실행할 파일 경로를 입력하세요." >&2; \
		exit 2; \
	fi
	@for source in $(RUN_FILES); do \
		./scripts/run-solution.sh "$$source"; \
	done

clean:
	@./scripts/clean-build.sh
