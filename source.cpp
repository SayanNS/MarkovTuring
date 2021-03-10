#include <iostream>
#include <fstream>

#define EMPTY_SYMBOL '_'
#define DEL_SYMBOL '*'
#define TERMINATING_INSTRUCTION_INDEX 0

struct Node {
	char symbol;
	Node *next, *prev;
};

struct Instruction
{
	char symbol;
	char direction;
	int instruction;
};

class TuringMachine {
private:

	Node *head, *tail, *current;
public:

	TuringMachine(char *word)
	{
		head = new Node();
		head->symbol = EMPTY_SYMBOL;
		tail = head;

		for (int i = 0; word[i] != '\0'; i++) {
			addend(word[i]);
		}

		addend(EMPTY_SYMBOL);
		current = head->next;
	}

	void run(int instruction_index, char *alphabet, Instruction **instructions)
	{
		while (instruction_index != TERMINATING_INSTRUCTION_INDEX) {
			for (int symbol_index = 0; symbol_index < alphabet[symbol_index]; symbol_index++) {
				if (alphabet[symbol_index] == current->symbol) {
					current->symbol = instructions[instruction_index][symbol_index].symbol;
					if (current == head && current->symbol != EMPTY_SYMBOL) {
						addbeg(EMPTY_SYMBOL);
					}
					if (current == tail && current->symbol != EMPTY_SYMBOL) {
						addend(EMPTY_SYMBOL);
					}
					if (instructions[instruction_index][symbol_index].direction == 'R') current = current->next;
					if (instructions[instruction_index][symbol_index].direction == 'L') current = current->prev;
					instruction_index = instructions[instruction_index][symbol_index].instruction;
				}
			}
		}
	}

	void outputList(std::ostream &output_stream)
	{
		for (Node *node = head; node != tail; node = node->next) {
			if (node->symbol != EMPTY_SYMBOL) {
				output_stream << node->symbol;
			}
		}
	}

	void addbeg(char symbol)
	{
		Node *node = new Node;
		node->symbol = symbol;
		node->prev = head->prev;

		head->prev = node;
		node->next = head;
		head = node;
	}

	void addend(char symbol)
	{
		Node *node = new Node;
		node->symbol = symbol;
		node->next = tail->next;

		tail->next = node;
		node->prev = tail;
		tail = node;
	}
};

void inline generate(char *alphabet, int alphabet_size, Instruction **instructions,
	int &offset, char *right, char *left, bool terminating)
{
	if (*right != '=') {

		int first_offset = offset;
		
		if (*left != '\0') {

			for (int i = 0; i < alphabet_size; i++) {

				if (*right == alphabet[i]) {
					instructions[offset][i].instruction = offset + 2;
					instructions[offset][i].symbol = *left;
					instructions[offset][i].direction = 'R';
				} else {
					instructions[offset][i].instruction = offset;
					instructions[offset][i].symbol = alphabet[i];
					instructions[offset][i].direction = 'R';
				}

				if (*left == alphabet[i]) {
					instructions[offset + 1][i].instruction = offset;
					instructions[offset + 1][i].symbol = *right;
					instructions[offset + 1][i].direction = 'R';
				} else {
					instructions[offset + 1][i].instruction = 0;
					instructions[offset + 1][i].symbol = EMPTY_SYMBOL;
					instructions[offset + 1][i].direction = EMPTY_SYMBOL;
				}
			}

			offset += 2;

			for (right++, left++; *right != '=' && *left != '\0'; right++, left++) {

				for (int i = 0; i < alphabet_size; i++) {

					if (*right == alphabet[i]) {
						instructions[offset][i].instruction = offset + 2;
						instructions[offset][i].symbol = *left;
						instructions[offset][i].direction = 'R';
					} else {
						instructions[offset][i].instruction = offset - 1;
						instructions[offset][i].symbol = alphabet[i];
						instructions[offset][i].direction = 'L';
					}

					if (*left == alphabet[i]) {
						instructions[offset + 1][i].instruction = offset - 1;
						instructions[offset + 1][i].symbol = *right;
						instructions[offset + 1][i].direction = 'L';
					} else {
						instructions[offset + 1][i].instruction = 0;
						instructions[offset + 1][i].symbol = EMPTY_SYMBOL;
						instructions[offset + 1][i].direction = EMPTY_SYMBOL;
					}
				}

				offset += 2;
			}

			if (*right == '=' && *left == '\0') {

				for (int i = 0; i < alphabet_size; i++) {
					if (EMPTY_SYMBOL == alphabet[i]) {
						instructions[offset][i].instruction = offset + 1;
						instructions[offset][i].symbol = alphabet[i];
						instructions[offset][i].direction = 'L';
					} else {
						instructions[offset][i].instruction = offset;
						instructions[offset][i].symbol = alphabet[i];
						instructions[offset][i].direction = 'R';
					}
				}

				offset++;

				for (int i = 0; i < alphabet_size; i++) {
					if (EMPTY_SYMBOL == alphabet[i]) {
						instructions[offset][i].instruction = terminating ? TERMINATING_INSTRUCTION_INDEX : 1;
						instructions[offset][i].symbol = EMPTY_SYMBOL;
						instructions[offset][i].direction = 'R';
					} else {
						instructions[offset][i].instruction = offset;
						instructions[offset][i].symbol = alphabet[i];
						instructions[offset][i].direction = 'L';
					}
				}

				instructions[first_offset][alphabet_size - 1].instruction = ++offset;
				instructions[first_offset][alphabet_size - 1].direction = 'L';
				
				for (int i = 0; i < alphabet_size; i++) {

					if (EMPTY_SYMBOL == alphabet[i]) {
						instructions[offset][i].instruction = offset + 1;
						instructions[offset][i].symbol = EMPTY_SYMBOL;
						instructions[offset][i].direction = 'R';
					} else {
						instructions[offset][i].instruction = offset;
						instructions[offset][i].symbol = alphabet[i];
						instructions[offset][i].direction = 'L';
					}
				}

				offset++;
				return;
			}

			if (*right == '=') {

				for (;*left != '\0'; left++) {

					for (int i = 0; i < alphabet_size; i++) {
						instructions[offset][i].instruction = offset + i + 1;
						instructions[offset][i].symbol = DEL_SYMBOL;
						instructions[offset][i].direction = 'R';
					}

					offset++;

					for (int i = 0; i < alphabet_size - 1; i++) {

						for (int j = 0; j < alphabet_size; j++) {

							instructions[offset + i][j].instruction = offset + j;
							instructions[offset + i][j].symbol = alphabet[i];
							instructions[offset + i][j].direction = 'R';
						}
					}

					offset += alphabet_size - 1;

					for (int i = 0; i < alphabet_size; i++) {
						if (DEL_SYMBOL == alphabet[i]) {
							instructions[offset][i].instruction = offset + 1;
							instructions[offset][i].symbol = *left;
							instructions[offset][i].direction = 'R';
						} else {
							instructions[offset][i].instruction = offset;
							instructions[offset][i].symbol = alphabet[i];
							instructions[offset][i].direction = 'L';
						}
					}

					offset++;
				}
				
				for (int i = 0; i < alphabet_size; i++) {
					if (EMPTY_SYMBOL == alphabet[i]) {
						instructions[offset][i].instruction = offset + 1;
						instructions[offset][i].symbol = EMPTY_SYMBOL;
						instructions[offset][i].direction = 'L';
					} else {
						instructions[offset][i].instruction = offset;
						instructions[offset][i].symbol = alphabet[i];
						instructions[offset][i].direction = 'R';
					}
				}

				offset++;

				for (int i = 0; i < alphabet_size; i++) {
					if (EMPTY_SYMBOL == alphabet[i]) {
						instructions[offset][i].instruction = terminating ? TERMINATING_INSTRUCTION_INDEX : 1;
						instructions[offset][i].symbol = EMPTY_SYMBOL;
						instructions[offset][i].direction = 'R';
					} else {
						instructions[offset][i].instruction = offset;
						instructions[offset][i].symbol = alphabet[i];
						instructions[offset][i].direction = 'L';
					}
				}

				instructions[first_offset][alphabet_size - 1].instruction = ++offset;
				instructions[first_offset][alphabet_size - 1].direction = 'L';
				
				for (int i = 0; i < alphabet_size; i++) {

					if (EMPTY_SYMBOL == alphabet[i]) {
						instructions[offset][i].instruction = offset + 1;
						instructions[offset][i].symbol = EMPTY_SYMBOL;
						instructions[offset][i].direction = 'R';
					} else {
						instructions[offset][i].instruction = offset;
						instructions[offset][i].symbol = alphabet[i];
						instructions[offset][i].direction = 'L';
					}
				}

				offset++;
				return;
			}

			if (*left == '\0')
				goto end_of_left;

		} else {

			for (int i = 0; i < alphabet_size; i++) {

				if (*right == alphabet[i]) {
					instructions[offset][i].instruction = offset + 2;
					instructions[offset][i].symbol = DEL_SYMBOL;
					instructions[offset][i].direction = 'R';
				} else {
					instructions[offset][i].instruction = offset;
					instructions[offset][i].symbol = alphabet[i];
					instructions[offset][i].direction = 'R';
				}

				if (DEL_SYMBOL == alphabet[i]) {
					instructions[offset + 1][i].instruction = offset;
					instructions[offset + 1][i].symbol = *right;
					instructions[offset + 1][i].direction = 'R';
				} else {
					instructions[offset + 1][i].instruction = 0;
					instructions[offset + 1][i].symbol = EMPTY_SYMBOL;
					instructions[offset + 1][i].direction = EMPTY_SYMBOL;
				}
			}

			right++;
			offset += 2;
end_of_left:
			
			for (; *right != '='; right++) {

				for (int i = 0; i < alphabet_size; i++) {

					if (*right == alphabet[i]) {
						instructions[offset][i].instruction = offset + 2;
						instructions[offset][i].symbol = DEL_SYMBOL;
						instructions[offset][i].direction = 'R';
					} else {
						instructions[offset][i].instruction = offset - 1;
						instructions[offset][i].symbol = alphabet[i];
						instructions[offset][i].direction = 'L';
					}

					if (DEL_SYMBOL == alphabet[i]) {
						instructions[offset + 1][i].instruction = offset - 1;
						instructions[offset + 1][i].symbol = *right;
						instructions[offset + 1][i].direction = 'L';
					} else {
						instructions[offset + 1][i].instruction = 0;
						instructions[offset + 1][i].symbol = EMPTY_SYMBOL;
						instructions[offset + 1][i].direction = EMPTY_SYMBOL;
					}
				}

				offset += 2;
			}
			
			for (int i = 0; i < alphabet_size; i++) {

				if (EMPTY_SYMBOL == alphabet[i]) {
					instructions[offset][i].instruction = offset + i + 1;
					instructions[offset][i].symbol = EMPTY_SYMBOL;
					instructions[offset][i].direction = 'L';
				} else {
					instructions[offset][i].instruction = offset;
					instructions[offset][i].symbol = alphabet[i];
					instructions[offset][i].direction = 'R';
				}
			}

			offset++;
			
			for (int i = 0; i < alphabet_size; i++) {

				for (int j = 0; j < alphabet_size; j++) {

					if (DEL_SYMBOL == alphabet[j]) {
						instructions[offset + i][j].instruction = offset + alphabet_size;
						instructions[offset + i][j].symbol = alphabet[i];
						instructions[offset + i][j].direction = 'L';
					} else {
						instructions[offset + i][j].instruction = offset + j;
						instructions[offset + i][j].symbol = alphabet[i];
						instructions[offset + i][j].direction = 'L';
					}
				}
			}

			for (int i = 0; i < alphabet_size; i++) {

				if (EMPTY_SYMBOL == alphabet[i]) {
					instructions[offset + alphabet_size][i].instruction = terminating ? TERMINATING_INSTRUCTION_INDEX : 1;
					instructions[offset + alphabet_size][i].symbol = EMPTY_SYMBOL;
					instructions[offset + alphabet_size][i].direction = 'R';
				} else if (DEL_SYMBOL == alphabet[i]) {
					instructions[offset + alphabet_size][i].instruction = offset - 1;
					instructions[offset + alphabet_size][i].symbol = DEL_SYMBOL;
					instructions[offset + alphabet_size][i].direction = 'R';
				} else {
					instructions[offset + alphabet_size][i].instruction = offset + alphabet_size;
					instructions[offset + alphabet_size][i].symbol = alphabet[i];
					instructions[offset + alphabet_size][i].direction = 'L';
				}
			}

			offset += alphabet_size + 1;
		}

		instructions[first_offset][alphabet_size - 1].instruction = offset;
		instructions[first_offset][alphabet_size - 1].direction = 'L';
			
		for (int i = 0; i < alphabet_size; i++) {

			if (EMPTY_SYMBOL == alphabet[i]) {
				instructions[offset][i].instruction = offset + 1;
				instructions[offset][i].symbol = EMPTY_SYMBOL;
				instructions[offset][i].direction = 'R';
			} else {
				instructions[offset][i].instruction = offset;
				instructions[offset][i].symbol = alphabet[i];
				instructions[offset][i].direction = 'L';
			}
		}

		offset++;
		return;

	} else {
end_of_right:

		for (;*left != '\0'; left++) {

			for (int i = 0; i < alphabet_size; i++) {
				instructions[offset][i].instruction = offset + i + 1;
				instructions[offset][i].symbol = DEL_SYMBOL;
				instructions[offset][i].direction = 'R';
			}

			offset++;

			for (int i = 0; i < alphabet_size - 1; i++) {

				for (int j = 0; j < alphabet_size; j++) {

					instructions[offset + i][j].instruction = offset + j;
					instructions[offset + i][j].symbol = alphabet[i];
					instructions[offset + i][j].direction = 'R';
				}
			}

			offset += alphabet_size - 1;

			for (int i = 0; i < alphabet_size; i++) {
				if (DEL_SYMBOL == alphabet[i]) {
					instructions[offset][i].instruction = offset + 1;
					instructions[offset][i].symbol = *left;
					instructions[offset][i].direction = 'R';
				} else {
					instructions[offset][i].instruction = offset;
					instructions[offset][i].symbol = alphabet[i];
					instructions[offset][i].direction = 'L';
				}
			}

			offset++;
		}
		
		for (int i = 0; i < alphabet_size; i++) {
			if (EMPTY_SYMBOL == alphabet[i]) {
				instructions[offset][i].instruction = offset + 1;
				instructions[offset][i].symbol = EMPTY_SYMBOL;
				instructions[offset][i].direction = 'L';
			} else {
				instructions[offset][i].instruction = offset;
				instructions[offset][i].symbol = alphabet[i];
				instructions[offset][i].direction = 'R';
			}
		}

		offset++;

		for (int i = 0; i < alphabet_size; i++) {
			if (EMPTY_SYMBOL == alphabet[i]) {
				instructions[offset][i].instruction = terminating ? TERMINATING_INSTRUCTION_INDEX : 1;
				instructions[offset][i].symbol = EMPTY_SYMBOL;
				instructions[offset][i].direction = 'R';
			} else {
				instructions[offset][i].instruction = offset;
				instructions[offset][i].symbol = alphabet[i];
				instructions[offset][i].direction = 'L';
			}
		}

		offset++;
	}
}

int main() {

	/* ... INSTRUCTIONS GENERATING ROUTINE ... */
	
	std::ifstream input_file;
	input_file.open("MarkovAlgorithms.txt");
	if (!input_file) {
		std::cerr << "Error: could not open input file\n";
		return 0;
	}

	char alphabet[256]; // 256 is number of symbols in ASCII... sort of
	input_file >> alphabet; // the first line is alphabet.
	int alphabet_size;
	for (alphabet_size = 0; alphabet[alphabet_size] != '\0'; alphabet_size++);
	alphabet[alphabet_size++] = DEL_SYMBOL;
	alphabet[alphabet_size++] = EMPTY_SYMBOL;
	alphabet[alphabet_size] = '\0';
	char word[256]; // yeah... it's bad that its size is hardcoded but for simple algorithm it's okay
	input_file >> word; // the second line is word.

	Instruction **instructions = new Instruction *[1000];
	for (int i = 0; i < 1000; i++) {
		instructions[i] = new Instruction[alphabet_size];
	}
	int offset = 1;
	while (true) {
		char algorithm[500];
		input_file >> algorithm;
		if (input_file.eof()) {
			input_file.close();
			break;
		}
		bool terminating = false;
		int i;
		for (i = 0; algorithm[i] != '='; i++);
		if (algorithm[++i] == '!') {
			terminating = true;
			i++;
		}
		generate(alphabet, alphabet_size, instructions, offset, &algorithm[0], &algorithm[i], terminating);
	}

	for (int i = 0; i < alphabet_size; i++) {
		instructions[offset][i].symbol = alphabet[i];
		instructions[offset][i].instruction = TERMINATING_INSTRUCTION_INDEX;
		instructions[offset][i].direction = EMPTY_SYMBOL;
	}

	offset++;

	/* ... INSTRUCTIONS PRINT ROUTINE ... */

	std::ofstream output_file;
	output_file.open("TuringInstructions.txt");

	int num = 0;
	for (int i = 1; i < offset; i *= 10, num++); // Get how many numbers offset has 

	int number_counter = 0;
	int offset_counter = 1;

	output_file << "   ";

	for (int numbers = 1; numbers < offset; numbers *= 10, number_counter++) {

		for (;offset_counter < numbers; offset_counter++) {

			output_file << "    " << offset_counter;
			for (int k = number_counter; k < num; k++) {
				output_file << ' ';
			}
			output_file << ' ';
		}
	}

	for (; offset_counter < offset; offset_counter++) {

		output_file << "    " << offset_counter;
		for (int k = number_counter; k < num; k++) {
			output_file << ' ';
		}
		output_file << ' ';
	}

	for (int i = 0; i < alphabet_size; i++) {

		output_file << '\n';
		output_file << alphabet[i] << "  ";

		for (int j = 1; j < offset; j++) {
			output_file << instructions[j][i].symbol << ',' << instructions[j][i].direction << ',';
			int off = (int) output_file.tellp();
			output_file << instructions[j][i].instruction;
			off = (int) output_file.tellp() - off;

			for (int k = off; k < num; k++) {
				output_file << ' ';
			}

			output_file << ' ';
		}
	}

	/* ... WORD MODIFICATION ROUTINE ... */

	output_file << "\n\n";

	TuringMachine machine(word);
	machine.run(1, alphabet, instructions);
	machine.outputList(output_file);

	output_file.close();

	return 0;
}
