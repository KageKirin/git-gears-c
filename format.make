## formatting makerules

format: $(wildcard src/*.h) $(wildcard src/*.c) $(wildcard src/*/*.h) $(wildcard src/*/*.c)
	@clang-format -i $^
