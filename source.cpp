#include <iostream>
#include <stdio.h>
#include <fstream>

#define EMPTY_SYMBOL ' '
#define DEL_SYMBOL '*'

struct Node {
	char symbol;
	Node *next, *prev;
};

class List {
public:
	Node *head, *tail, *current;
	List(char *str) {
		head = new Node();
		head->symbol = EMPTY_SYMBOL;
		tail = head;

		for (int i = 0; str[i] != '\0'; i++) {
			addend(str[i]);
		}

		addend(EMPTY_SYMBOL);

		current = head->next;
	}

	void addbeg(char symbol) {
		Node *node = new Node;
		node->symbol = symbol;
		node->prev = head->prev;

		head->prev = node;
		node->next = head;
		head = node;
	}

	void addend(char symbol) {
		Node *node = new Node;
		node->symbol = symbol;
		node->next = tail->next;

		tail->next = node;
		node->prev = tail;
		tail = node;
	}
};

struct Instruction
{
	char symbol;
	char direction;
	int move;
};

int inline generate(char *alphabet, int alphabet_size, Instruction **instructions,
	int offset, char *right, char *left, bool terminating)
{
	if (*right != '=') {
		
		if (*left != '\0') {

			for (int i = 0; i < alphabet_size; i++) {

				if (*right == alphabet[i]) {
					instructions[offset][i].move = offset + 2;
					instructions[offset][i].symbol = *left;
					instructions[offset][i].direction = 'R';
				} else {
					instructions[offset][i].move = offset;
					instructions[offset][i].symbol = alphabet[i];
					instructions[offset][i].direction = 'R';
				}

				if (*left == alphabet[i]) {
					instructions[offset + 1][i].move = offset;
					instructions[offset + 1][i].symbol = *right;
					instructions[offset + 1][i].direction = 'R';
				} else {
					instructions[offset + 1][i].move = 0;
					instructions[offset + 1][i].symbol = ' ';
					instructions[offset + 1][i].direction = ' ';
				}
			}

			offset += 2;

			for (right++, left++; *right != '=' && *left != '\0'; right++, left++) {

				for (int i = 0; i < alphabet_size; i++) {

					if (*right == alphabet[i]) {
						instructions[offset][i].move = offset + 2;
						instructions[offset][i].symbol = *left;
						instructions[offset][i].direction = 'R';
					} else {
						instructions[offset][i].move = offset - 1;
						instructions[offset][i].symbol = alphabet[i];
						instructions[offset][i].direction = 'L';
					}

					if (*left == alphabet[i]) {
						instructions[offset + 1][i].move = offset - 1;
						instructions[offset + 1][i].symbol = *right;
						instructions[offset + 1][i].direction = 'L';
					} else {
						instructions[offset + 1][i].move = 0;
						instructions[offset + 1][i].symbol = ' ';
						instructions[offset + 1][i].direction = ' ';
					}
				}

				offset += 2;
			}

			if (*right == '=' && *left == '\0')
				goto end;

			if (*right == '=')
				goto end_of_right;

			if (*left == '\0')
				goto end_of_left;

		} else {

			for (int i = 0; i < alphabet_size; i++) {

				if (*right == alphabet[i]) {
					instructions[offset][i].move = offset + 2;
					instructions[offset][i].symbol = DEL_SYMBOL;
					instructions[offset][i].direction = 'R';
				} else {
					instructions[offset][i].move = offset;
					instructions[offset][i].symbol = alphabet[i];
					instructions[offset][i].direction = 'R';
				}

				if (DEL_SYMBOL == alphabet[i]) {
					instructions[offset + 1][i].move = offset;
					instructions[offset + 1][i].symbol = *right;
					instructions[offset + 1][i].direction = 'R';
				} else {
					instructions[offset + 1][i].move = 0;
					instructions[offset + 1][i].symbol = ' ';
					instructions[offset + 1][i].direction = ' ';
				}
			}

			offset += 2;
end_of_left:
			
			for (; *right != '='; right++) {

				for (int i = 0; i < alphabet_size; i++) {

					if (*right == alphabet[i]) {
						instructions[offset][i].move = offset + 2;
						instructions[offset][i].symbol = DEL_SYMBOL;
						instructions[offset][i].direction = 'R';
					} else {
						instructions[offset][i].move = offset - 1;
						instructions[offset][i].symbol = alphabet[i];
						instructions[offset][i].direction = 'L';
					}

					if (DEL_SYMBOL == alphabet[i]) {
						instructions[offset + 1][i].move = offset - 1;
						instructions[offset + 1][i].symbol = *right;
						instructions[offset + 1][i].direction = 'L';
					} else {
						instructions[offset + 1][i].move = 0;
						instructions[offset + 1][i].symbol = ' ';
						instructions[offset + 1][i].direction = ' ';
					}
				}

				offset += 2;
			}

			for (int i = 0; i < alphabet_size; i++) {

				if (EMPTY_SYMBOL == alphabet[i]) {
					instructions[offset][alphabet_size].move = offset + i + 1;
					instructions[offset][alphabet_size].symbol = EMPTY_SYMBOL;
					instructions[offset][alphabet_size].direction = 'L';
				} else {
					instructions[offset][i].move = offset;
					instructions[offset][i].symbol = alphabet[i];
					instructions[offset][i].direction = 'R';
				}
			}

			offset++;
			
			for (int i = 0; i < alphabet_size; i++) {

				for (int j = 0; j < alphabet_size; j++) {

					if (DEL_SYMBOL == alphabet[j]) {
						instructions[offset + i][j].move = offset + alphabet_size;
						instructions[offset + i][j].symbol = alphabet[i];
						instructions[offset + i][j].direction = 'L';
					} else {
						instructions[offset + i][j].move = offset + i;
						instructions[offset + i][j].symbol = alphabet[i];
						instructions[offset + i][j].direction = 'L';
					}
				}
			}

			for (int i = 0; i < alphabet_size; i++) {

				if (EMPTY_SYMBOL == alphabet[i]) {
					instructions[offset + alphabet_size][i].move = terminating ? 0 : 1;
					instructions[offset + alphabet_size][i].symbol = EMPTY_SYMBOL;
					instructions[offset + alphabet_size][i].direction = 'R';
				} else if (DEL_SYMBOL == alphabet[i]) {
					instructions[offset + alphabet_size][i].move = offset - 1;
					instructions[offset + alphabet_size][i].symbol = DEL_SYMBOL;
					instructions[offset + alphabet_size][i].direction = 'R';
				} else {
					instructions[offset + alphabet_size][i].move = offset;
					instructions[offset + alphabet_size][i].symbol = alphabet[i];
					instructions[offset + alphabet_size][i].direction = 'L';
				}
			}

			offset += alphabet_size + 1;

			return offset;
		}
	} else {
end_of_right:

		for (;*left != '\0'; left++) {

			for (int i = 0; i < alphabet_size; i++) {
				instructions[offset][i].move = offset + i + 1;
				instructions[offset][i].symbol = DEL_SYMBOL;
				instructions[offset][i].direction = 'R';
			}

			offset++;

			for (int i = 0; i < alphabet_size; i++) {

				for (int j = 0; j < alphabet_size; j++) {

					if (EMPTY_SYMBOL == alphabet[j]) {
						instructions[offset + i][j].move = offset + alphabet_size;
						instructions[offset + i][j].symbol = alphabet[i];
						instructions[offset + i][j].direction = 'L';
					} else {
						instructions[offset + i][j].move = offset + i;
						instructions[offset + i][j].symbol = alphabet[i];
						instructions[offset + i][j].direction = 'R';
					}
				}
			}

			for (int i = 0; i < alphabet_size; i++) {
				if (DEL_SYMBOL == alphabet[i]) {
					instructions[offset + alphabet_size][i].move = offset + alphabet_size + 1;
					instructions[offset + alphabet_size][i].symbol = *left;
					instructions[offset + alphabet_size][i].direction = 'R';
				} else {
					instructions[offset + alphabet_size][i].move = offset + alphabet_size;
					instructions[offset + alphabet_size][i].symbol = alphabet[i];
					instructions[offset + alphabet_size][i].direction = 'L';
				}
			}

			offset += alphabet_size + 1;
		}
end:
		for (int i = 0; i < alphabet_size; i++) {
			if (EMPTY_SYMBOL == alphabet[i]) {
				instructions[offset][i].move = terminating ? 0 : 1;
				instructions[offset][i].symbol = alphabet[i];
				instructions[offset][i].direction = 'R';
			} else {
				instructions[offset][i].move = offset;
				instructions[offset][i].symbol = alphabet[i];
				instructions[offset][i].direction = 'L';
			}
		}

		return ++offset;
	}
}


int main() {

	std::ifstream input_file;
	input_file.open("Алгоритмы Маркова.txt");
	if (!input_file) {
		std::cerr << "Error: could not open input file\n";
		return 0;
	}

	char alphabet[256]; // 256 is number of symbols in ASCII... sort of
	input_file >> alphabet; // the first line is alphabet.
	int alphabet_size;
	for (alphabet_size = 0; alphabet[alphabet_size] != '\0'; alphabet_size++);
	alphabet[alphabet_size++] = EMPTY_SYMBOL;
	alphabet[alphabet_size] = '\0';
	char word[256]; // yeah... it's bad that its size is hardcoded but for simple algorithm it's okay
	input_file >> word; // the second line is word.

	int offset = 0;
	Instruction **instructions = new Instruction *[1000];
	for (int i = 0; i < 1000; i++) {
		instructions[i] = new Instruction[alphabet_size];
	}
	while (!input_file.eof()) {
		char algorithm[500];
		input_file >> algorithm;
		bool terminating = false;
		int i;
		for (i = 0; algorithm[i] != '='; i++);
		if (algorithm[++i] == '!') {
			terminating = true;
			i++;
		}
		int new_offset = generate(alphabet, alphabet_size, instructions, offset, &algorithm[0], &algorithm[i], terminating);

		if (i == 0) {
			instructions[offset][alphabet_size - 1].move = new_offset;

			for (int i = 0; i < alphabet_size; i++) {
				if (EMPTY_SYMBOL == alphabet[i]) {
					instructions[new_offset][i].move = new_offset + 1;
					instructions[new_offset][i].symbol = alphabet[i];
					instructions[new_offset][i].direction = 'R';
				} else {
					instructions[new_offset][i].move = new_offset;
					instructions[new_offset][i].symbol = alphabet[i];
					instructions[new_offset][i].direction = 'L';
				}
			}

			offset = new_offset + 1;
		}
	}

	List lenta(word);

	return 0;
}