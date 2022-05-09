json_fuzzer:
	clang++ -c -Icereal/include -fsanitize=fuzzer json_fuzzer.cpp
	clang++ -fsanitize=fuzzer json_fuzzer.o -o json_fuzzer

clean:
	rm -f *.o json_fuzzer
