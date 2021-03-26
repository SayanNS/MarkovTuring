#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

template <typename T>
class MyVector : public std::vector<T>
{
public:
	T & operator[] (std::size_t index)
	{
		if (std::vector<T>::size() <= index) {
			std::vector<T>::resize(index + 1);
		}

		return std::vector<T>::operator[](index);
	}
};

class TuringMachine
{
private:

	Node *head, *tail, *current;
public:

	TuringMachine(std::string word)
	{
		head = new Node();
		head->symbol = EMPTY_SYMBOL;
		tail = head;

		for (int i = 0; i < word.size(); i++) {
			addend(word[i]);
		}

		addend(EMPTY_SYMBOL);
		current = head->next;
	}

	void run(int instruction_index, std::string alphabet, std::vector<std::vector<Instruction>> &instructions)
	{
		while (instruction_index != TERMINATING_INSTRUCTION_INDEX) {
			for (int symbol_index = 0; symbol_index < alphabet.size(); symbol_index++) {
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
					break;
				}
			}
		}
	}

	void outputList(std::ostream &output_stream)
	{
		for (Node *node = head->next; node != tail; node = node->next) {
			if (node->symbol != EMPTY_SYMBOL) {
				output_stream << node->symbol;
			}
		}
		output_stream << '\n';
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

void inline generate(std::string alphabet, MyVector<MyVector<Instruction>> &instructions,
	int offset, std::string left, std::string right, bool terminating)
{
	int l_offset = 0;
	int r_offset = 0;

	if (l_offset < left.size()) {

		int first_offset = offset;
		
		if (r_offset < right.size()) {
			for (int i = 0; i < alphabet.size(); i++) {
				if (left[l_offset] == alphabet[i]) {
					instructions[offset][i].instruction = offset + 2;
					instructions[offset][i].symbol = right[r_offset];
					instructions[offset][i].direction = 'R';
				} else {
					instructions[offset][i].instruction = offset;
					instructions[offset][i].symbol = alphabet[i];
					instructions[offset][i].direction = 'R';
				}

				if (right[r_offset] == alphabet[i]) {
					instructions[offset + 1][i].instruction = offset;
					instructions[offset + 1][i].symbol = left[l_offset];
					instructions[offset + 1][i].direction = 'R';
				} else {
					instructions[offset + 1][i].instruction = 0;
					instructions[offset + 1][i].symbol = EMPTY_SYMBOL;
					instructions[offset + 1][i].direction = EMPTY_SYMBOL;
				}
			}

			offset += 2;

			for (l_offset++, r_offset++; l_offset < left.size() && r_offset < right.size(); l_offset++, r_offset++) {
				for (int i = 0; i < alphabet.size(); i++) {
					if (left[l_offset] == alphabet[i]) {
						instructions[offset][i].instruction = offset + 2;
						instructions[offset][i].symbol = right[r_offset];
						instructions[offset][i].direction = 'R';
					} else {
						instructions[offset][i].instruction = offset - 1;
						instructions[offset][i].symbol = alphabet[i];
						instructions[offset][i].direction = 'L';
					}

					if (right[r_offset] == alphabet[i]) {
						instructions[offset + 1][i].instruction = offset - 1;
						instructions[offset + 1][i].symbol = left[l_offset];
						instructions[offset + 1][i].direction = 'L';
					} else {
						instructions[offset + 1][i].instruction = 0;
						instructions[offset + 1][i].symbol = EMPTY_SYMBOL;
						instructions[offset + 1][i].direction = EMPTY_SYMBOL;
					}
				}

				offset += 2;
			}

			if (l_offset == left.size() && r_offset == right.size()) {
				for (int i = 0; i < alphabet.size(); i++) {
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

				for (int i = 0; i < alphabet.size(); i++) {
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

				instructions[first_offset][alphabet.size() - 1].instruction = ++offset;
				instructions[first_offset][alphabet.size() - 1].direction = 'L';

				for (int i = 0; i < alphabet.size(); i++) {

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

			if (l_offset == left.size()) {
				for (;r_offset < right.size(); r_offset++) {
					for (int i = 0; i < alphabet.size(); i++) {
						instructions[offset][i].instruction = offset + i + 1;
						instructions[offset][i].symbol = DEL_SYMBOL;
						instructions[offset][i].direction = 'R';
					}

					offset++;

					for (int i = 0; i < alphabet.size() - 1; i++) {
						for (int j = 0; j < alphabet.size(); j++) {
							instructions[offset + i][j].instruction = offset + j;
							instructions[offset + i][j].symbol = alphabet[i];
							instructions[offset + i][j].direction = 'R';
						}
					}

					offset += alphabet.size() - 1;

					for (int i = 0; i < alphabet.size(); i++) {
						if (DEL_SYMBOL == alphabet[i]) {
							instructions[offset][i].instruction = offset + 1;
							instructions[offset][i].symbol = right[r_offset];
							instructions[offset][i].direction = 'R';
						} else {
							instructions[offset][i].instruction = offset;
							instructions[offset][i].symbol = alphabet[i];
							instructions[offset][i].direction = 'L';
						}
					}

					offset++;
				}

				for (int i = 0; i < alphabet.size(); i++) {
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

				for (int i = 0; i < alphabet.size(); i++) {
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

				instructions[first_offset][alphabet.size() - 1].instruction = ++offset;
				instructions[first_offset][alphabet.size() - 1].direction = 'L';

				for (int i = 0; i < alphabet.size(); i++) {

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

			if (r_offset == right.size())
				goto end_of_left;

		} else {

			for (int i = 0; i < alphabet.size(); i++) {

				if (left[l_offset] == alphabet[i]) {
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
					instructions[offset + 1][i].symbol = left[l_offset];
					instructions[offset + 1][i].direction = 'R';
				} else {
					instructions[offset + 1][i].instruction = 0;
					instructions[offset + 1][i].symbol = EMPTY_SYMBOL;
					instructions[offset + 1][i].direction = EMPTY_SYMBOL;
				}
			}

			l_offset++;
			offset += 2;
end_of_left:
			for (; l_offset < left.size(); l_offset++) {
				for (int i = 0; i < alphabet.size(); i++) {
					if (left[l_offset] == alphabet[i]) {
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
						instructions[offset + 1][i].symbol = left[l_offset];
						instructions[offset + 1][i].direction = 'L';
					} else {
						instructions[offset + 1][i].instruction = 0;
						instructions[offset + 1][i].symbol = EMPTY_SYMBOL;
						instructions[offset + 1][i].direction = EMPTY_SYMBOL;
					}
				}

				offset += 2;
			}

			for (int i = 0; i < alphabet.size(); i++) {
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
			
			for (int i = 0; i < alphabet.size(); i++) {
				for (int j = 0; j < alphabet.size(); j++) {
					if (DEL_SYMBOL == alphabet[j]) {
						instructions[offset + i][j].instruction = offset + alphabet.size();
						instructions[offset + i][j].symbol = alphabet[i];
						instructions[offset + i][j].direction = 'L';
					} else {
						instructions[offset + i][j].instruction = offset + j;
						instructions[offset + i][j].symbol = alphabet[i];
						instructions[offset + i][j].direction = 'L';
					}
				}
			}

			for (int i = 0; i < alphabet.size(); i++) {
				if (EMPTY_SYMBOL == alphabet[i]) {
					instructions[offset + alphabet.size()][i].instruction = terminating ? TERMINATING_INSTRUCTION_INDEX : 1;
					instructions[offset + alphabet.size()][i].symbol = EMPTY_SYMBOL;
					instructions[offset + alphabet.size()][i].direction = 'R';
				} else if (DEL_SYMBOL == alphabet[i]) {
					instructions[offset + alphabet.size()][i].instruction = offset - 1;
					instructions[offset + alphabet.size()][i].symbol = DEL_SYMBOL;
					instructions[offset + alphabet.size()][i].direction = 'R';
				} else {
					instructions[offset + alphabet.size()][i].instruction = offset + alphabet.size();
					instructions[offset + alphabet.size()][i].symbol = alphabet[i];
					instructions[offset + alphabet.size()][i].direction = 'L';
				}
			}

			offset += alphabet.size() + 1;
		}

		instructions[first_offset][alphabet.size() - 1].instruction = offset;
		instructions[first_offset][alphabet.size() - 1].direction = 'L';

		for (int i = 0; i < alphabet.size(); i++) {

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
		for (;r_offset < right.size(); r_offset++) {
			for (int i = 0; i < alphabet.size(); i++) {
				instructions[offset][i].instruction = offset + i + 1;
				instructions[offset][i].symbol = DEL_SYMBOL;
				instructions[offset][i].direction = 'R';
			}

			offset++;

			for (int i = 0; i < alphabet.size() - 1; i++) {
				for (int j = 0; j < alphabet.size(); j++) {
					instructions[offset + i][j].instruction = offset + j;
					instructions[offset + i][j].symbol = alphabet[i];
					instructions[offset + i][j].direction = 'R';
				}
			}

			offset += alphabet.size() - 1;

			for (int i = 0; i < alphabet.size(); i++) {
				if (DEL_SYMBOL == alphabet[i]) {
					instructions[offset][i].instruction = offset + 1;
					instructions[offset][i].symbol = right[r_offset];
					instructions[offset][i].direction = 'R';
				} else {
					instructions[offset][i].instruction = offset;
					instructions[offset][i].symbol = alphabet[i];
					instructions[offset][i].direction = 'L';
				}
			}

			offset++;
		}

		for (int i = 0; i < alphabet.size(); i++) {
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

		for (int i = 0; i < alphabet.size(); i++) {
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

	std::string alphabet;
	input_file >> alphabet; // the first line is alphabet.
	alphabet.push_back(DEL_SYMBOL);
	alphabet.push_back(EMPTY_SYMBOL);

	std::string word;
	input_file >> word; // the second line is word.

	MyVector<MyVector<Instruction>> instructions;
	instructions.resize(1);

	while (!input_file.eof()) {
		std::string algorithm;
		input_file >> algorithm;
		int i = algorithm.find('=');
		int l = algorithm.find('!');
		if (i == -1) {
			continue;
		}
		std::string left = algorithm.substr(0, i);
		if (l == -1) {
			std::string right = algorithm.substr(i + 1);
			generate(alphabet, instructions, instructions.size(), left, right, false);
		} else {
			std::string right = algorithm.substr(l + 1);
			generate(alphabet, instructions, instructions.size(), left, right, true);
		}
	}

	input_file.close();

	int index = instructions.size();
	instructions.resize(index + 1);

	for (int i = 0; i < alphabet.size(); i++) {
		instructions[index][i].symbol = alphabet[i];
		instructions[index][i].instruction = TERMINATING_INSTRUCTION_INDEX;
		instructions[index][i].direction = EMPTY_SYMBOL;
	}

	/* ... INSTRUCTIONS PRINT ROUTINE ... */

std::ofstream output_file;
	output_file.open("TuringInstructions.txt");

	int num = 0;
	for (int i = 1; i < instructions.size(); i *= 10, num++); // Get how many numbers offset has 

	int number_counter = 0;
	int offset_counter = 1;

	output_file << "   ";

	for (int numbers = 1; numbers < instructions.size(); numbers *= 10, number_counter++) {

		for (;offset_counter < numbers; offset_counter++) {

			output_file << "    " << offset_counter;
			for (int k = number_counter; k < num + 1; k++) {
				output_file << ' ';
			}
		}
	}

	for (; offset_counter < instructions.size(); offset_counter++) {

		output_file << "    " << offset_counter;
		for (int k = number_counter; k < num + 1; k++) {
			output_file << ' ';
		}
	}

	for (int i = 0; i < alphabet.size(); i++) {

		output_file << '\n';
		output_file << alphabet[i] << "  ";

		for (int j = 1; j < instructions.size(); j++) {
			output_file << instructions[j][i].symbol << ',' << instructions[j][i].direction << ',';
			int off = (int) output_file.tellp();
			output_file << instructions[j][i].instruction;
			off = (int) output_file.tellp() - off;

			for (int k = off; k < num + 1; k++) {
				output_file << ' ';
			}
		}
	}

	output_file << "\n";

	/* ... WORD MODIFICATION ROUTINE ... */

	TuringMachine machine(word);
	// std::vector<std::vector<Instruction>> *casted_vector = reinterpret_cast<std::vector<std::vector<Instruction>> *>(&instructions);
	machine.run(1, alphabet, *reinterpret_cast<std::vector<std::vector<Instruction>> *>(&instructions));
	machine.outputList(output_file);
	machine.outputList(std::cout);

	output_file.close();
	return 0;
}
