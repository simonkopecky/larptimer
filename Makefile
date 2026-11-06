build:
	gcc -Wall -pedantic -Wextra main.c -o larptimer

test: build
	./larptimer

test-fonts: build
	@for file in figlet-fonts/*.flf; do \
		[ -e "$$file" ] || continue; \
		font=$$(basename "$$file" .flf); \
		./larptimer -f "$$font" || (echo "$$font"; exit 1); \
	done

test-list: build
	./larptimer --list

test-help: build
	./larptimer --help
