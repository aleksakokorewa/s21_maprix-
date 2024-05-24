CC = clang++
STANDARD = -std=c++17
CFLAGS = -Wall -Wextra -Werror $(STANDARD) -pedantic
TEST_FLAGS = $(shell pkg-config --cflags --libs gtest)
OS = $(shell uname -s)

ifeq ($(OS), Darwin)
	LFAGS =
else
	LFAGS = -lgcov
endif

SRC = ./
PROJECT = s21_matrix_oop

all : $(PROJECT).a

clean :
	find . -type f \( -name "*.a" -o -name "*.o" -o -name "*.gcda" -o -name "*.gcno" -o -name "*_gcov_report" -o -name "*_test" \) -delete
	rm -rf report

rebuild : clean all

$(PROJECT).a :
	$(CC) $(CFLAGS) -c $(PROJECT).cc
	ar rc $(PROJECT).a *.o 
	ranlib $(PROJECT).a 
	rm -rf *.o 

test : 
	$(CC) $(CFLAGS) $(PROJECT)_$@.cc $(PROJECT).a -o $(PROJECT)_$@ $(TEST_FLAGS)
	$(SRC)$(PROJECT)_$@

format :
	find . -type f \( -name "*.cc" -o -name "*.h" \) | xargs clang-format --style=file:"../materials/linters/.clang-format" -i

check :
	find . -type f \( -name "*.cc" -o -name "*.h" \) | xargs clang-format --style=file:"../materials/linters/.clang-format" -n
	find . -type f \( -name "*.cc" -o -name "*.h" \) | xargs cppcheck --enable=all --suppress=missingIncludeSystem -$(STANDARD) --language=c++

valgrind_check :
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -s --track-origins=yes $(SRC)$(PROJECT)_test
