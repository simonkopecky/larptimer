build:
	gcc -Wall -pedantic -Wextra main.c -o hacktimer

test: build
	./hacktimer

test-fonts: build
	@for file in figlet-fonts/*.flf; do \
		[ -e "$$file" ] || continue; \
		font=$$(basename "$$file" .flf); \
		./hacktimer -f "$$font" || (echo "$$font"; exit 1); \
	done
