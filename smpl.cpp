#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

const std::string VERSION = "0.1.1";
const std::string TAPE_SIZE = "32";
const std::string REF_TAPE_SIZE = "256";

const std::string TAPE = "tape";
const std::string PTR = "ptr";
const std::string REF_TAPE = "ref_tape";
const std::string REF_PTR = "ref_ptr";

enum Lang { Golang, C };

std::string prelude(Lang lang) {
	switch (lang) {
		case Lang::C:
			return "#include <stdio.h>\n\nunsigned int " + TAPE + "[" + TAPE_SIZE +
				"];\n"
				"unsigned int " +
				REF_TAPE + "[" + REF_TAPE_SIZE +
				"];\n"
				"unsigned int " +
				PTR +
				" = 0;\n"
				"unsigned int " +
				REF_PTR +
				" = 0;\n\n"
				"unsigned int allocate() { \n\
				unsigned int size = " +
				TAPE + "[" + PTR + "]; \n\
				int cons_empty_spaces = 0; \n\
				for (int i=" +
					TAPE_SIZE + "-1; i>0; i--) {\n\
						if (" +
							TAPE + "[i] == 0) { cons_empty_spaces++; }\n\
						else { cons_empty_spaces = 0; }\n\
							if (cons_empty_spaces == size) { return i; }\n\
	}\n\
	return 0;\n}\n\nint main() {\r\n\t";
		case Lang::Golang:
return "package main\n\
	import \"fmt\"\n\
	\n\
	var " + TAPE + " = [" + TAPE_SIZE + "]uint{}\n\
	var " + REF_TAPE + " = [" + REF_TAPE_SIZE + "]uint{}\n\
	var " + PTR + " uint = 0\n\
	var " + REF_PTR + " uint = 0\n\n\
	func allocate() uint {\n\
		var cons_empty_spaces uint = 0\n\
			var size uint = " + TAPE + "[" + PTR + "]\n\
			for i := uint(" + TAPE_SIZE + "); i>0; i-- {\n\
				if tape[i] == 0 {\n\
					cons_empty_spaces++\n\
				} else { cons_empty_spaces = 0 }\n\
					if cons_empty_spaces == size { return i }\n\
			}\n\
		return 0\n\
	}\n\n\n\
func main() {\n\t";
	}
	return "";
}

std::string postlude(Lang lang) {
	switch (lang) {
		case Lang::C:
			return "\n\tfor(int loop = 0; loop < 32; loop++) printf(\"%u \", "
				"tape[loop]);\n}";
		case Lang::Golang:
			return "\n\tfmt.Println(tape[0:32])\n}";
	}
	return "";
}

std::string plus(Lang lang) {
	switch (lang) {
		case Lang::C:
			return TAPE + "[" + PTR + "]++;\r\n\t";
		case Lang::Golang:
			return TAPE + "[" + PTR + "]++\r\n\t";
	}
	return "";
}

std::string minus(Lang lang) {
	switch (lang) {
		case Lang::C:
			return TAPE + "[" + PTR + "]--;\r\n\t";
		case Lang::Golang:
			return TAPE + "[" + PTR + "]--\r\n\t";
	}
	return "";
}

std::string left(Lang lang) {
	switch (lang) {
		case Lang::C:
			return PTR + "--;\r\n\t";
		case Lang::Golang:
			return PTR + "--\r\n\t";
	}
	return "";
}

std::string right(Lang lang) {
	switch (lang) {
		case Lang::C:
			return PTR + "++;\r\n\t";
		case Lang::Golang:
			return PTR + "++\r\n\t";
	}
	return "";
}

std::string begin(Lang lang) {
	switch (lang) {
		case Lang::C:
			return "while (" + TAPE + "[" + PTR + "]) {";
		case Lang::Golang:
			return "for " + TAPE + "[" + PTR + "] != 0 {\r\n\t\t";
	}
	return "";
}

std::string end(Lang lang) {
	switch (lang) {
		case Lang::C:
			return "}\r\n\t";
		case Lang::Golang:
			return "}\r\n\t";
	}
	return "";
}

std::string output(Lang lang) {
	switch (lang) {
		case Lang::C:
			return "printf(\"%c\", (char)(" + TAPE + "[" + PTR + "]%256));\r\n\t";
		case Lang::Golang:
			return "fmt.Print(string(" + TAPE + "[" + PTR + "]%256))\r\n\t";
	}
	return "";
}

std::string input(Lang lang) { return ""; }

std::string deref(Lang lang) {
	switch (lang) {
		case Lang::C:
			return REF_TAPE + "[" + REF_PTR + "++ % " + REF_TAPE_SIZE + "] = " + PTR +
				";\r\n\t" + PTR + " = " + TAPE + "[" + PTR + "];\r\n\t";
		case Lang::Golang:
			return REF_TAPE + "[" + REF_PTR + " % " + REF_TAPE_SIZE + "] = " + PTR +
				"\r\n\t" + PTR + " = " + TAPE + "[" + PTR + "]\r\n\t" + REF_PTR + "++\r\n\t";
	}
	return "";
}

std::string ref(Lang lang) {
	switch (lang) {
		case Lang::C:
			return PTR + " = " + REF_TAPE + "[--" + REF_PTR + " % " + REF_TAPE_SIZE +
				"];\r\n\t";
		case Lang::Golang:
			return REF_PTR + "--\r\n\t" + PTR + " = " + REF_TAPE + "[" + REF_PTR + " % " + REF_TAPE_SIZE +
				"]\r\n\t";
	}
	return "";
}

std::string allocate(Lang lang) {
	switch (lang) {
		case Lang::C:
			return TAPE + "[" + PTR + "] = allocate();\r\n\t";
		case Lang::Golang:
			return TAPE + "[" + PTR + "] = allocate()\r\n\t";
	}
	return "";
}

enum Instruction {
	Plus,
	Minus,
	Left,
	Right,
	Begin,
	End,
	Output,
	Input,
	Ref,
	Deref,
	Allocate
};

class Compiled {
	public:
		Compiled(std::vector<Instruction> instructions) {
			this->instructions = instructions;
		}

		std::string compile(Lang lang) {
			std::string result = prelude(lang);
			for (auto i : this->instructions) {
				switch (i) {
					case Instruction::Plus:
						result += plus(lang);
						break;
					case Instruction::Minus:
						result += minus(lang);
						break;
					case Instruction::Left:
						result += left(lang);
						break;
					case Instruction::Right:
						result += right(lang);
						break;
					case Instruction::Begin:
						result += begin(lang);
						break;
					case Instruction::End:
						result += end(lang);
						break;
					case Instruction::Output:
						result += output(lang);
						break;
					case Instruction::Input:
						result += input(lang);
						break;
					case Instruction::Ref:
						result += ref(lang);
						break;
					case Instruction::Deref:
						result += deref(lang);
						break;
					case Instruction::Allocate:
						result += allocate(lang);
						break;
				}
			}
			return result + postlude(lang);
		}

	private:
		std::vector<Instruction> instructions;
};

Compiled parse(std::string input) {
	std::vector<Instruction> ins;
	for (unsigned int i = 0; i < input.length(); i++) {
		switch (input[i]) {
			case '+':
				ins.push_back(Instruction::Plus);
				break;
			case '-':
				ins.push_back(Instruction::Minus);
				break;
			case '<':
				ins.push_back(Instruction::Left);
				break;
			case '>':
				ins.push_back(Instruction::Right);
				break;
			case '[':
				ins.push_back(Instruction::Begin);
				break;
			case ']':
				ins.push_back(Instruction::End);
				break;
			case '.':
				ins.push_back(Instruction::Output);
				break;
			case ',':
				ins.push_back(Instruction::Input);
				break;
			case '*':
				ins.push_back(Instruction::Deref);
				break;
			case '&':
				ins.push_back(Instruction::Ref);
				break;
			case '?':
				ins.push_back(Instruction::Allocate);
				break;
		}
	}

	return Compiled(ins);
}

void version() {
	std::cout << "smpl " << VERSION << std::endl;
}

void help() {
	version();
	std::cout << "Adam McDaniel <adam.mcdaniel17@gmail.com>\n\
		Compiler for the smpl assembly language\n\
		\n\
		USAGE:\n\
		smpl <-g|-c>\n\
		\n\
		FLAGS:\n\
		-g, --go         Compile to Golang\n\
		-c               Compile to C\n\
		-h, --help       Prints help information\n\
		-V, --version    Prints version information\n\
		-f, --file       Pass a file into the compiler"	<< std::endl;
}

void compile(Lang lang) {  
	std::string input, line;
	input = "";
	while (std::getline(std::cin, line)) {
		input += line;
	}

	std::cout << parse(input).compile(lang) << std::endl;
}

void compileFromFile(Lang lang, std::string fileName) {  
	std::ifstream infile;
	std::string input, line;
	infile.open(fileName.c_str());
	input = "";
	while (std::getline(infile, line)) {
		input += line;
	}
	std::cout << parse(input).compile(lang) << std::endl;
}

int main(int argc, char** argv) {
	std::vector<std::string> args;
	for (int i=0; i<argc; i++) {
		args.push_back(argv[i]);
	}

	if (argc == 2 && (args[1] == "-h" || args[1] == "--help")) {
		help();
	} else if (argc == 2 && (args[1] == "-V" || args[1] == "--version")) {
		version();
	} else if (argc == 2 && (args[1] == "-g" || args[1] == "--go")) {
		compile(Lang::Golang);
	} else if (argc == 2 && args[1] == "-c") {
		compile(Lang::C);
	} else if (argc == 4 && (args[2] == "-f" || args[2] == "--file")) {
		if (args[1] == "-g" || args[1] == "--go") {
			compileFromFile(Lang::Golang, args[3]);
		} else if (args[1] == "-c") {
			compileFromFile(Lang::C, args[3]);
		} else {
			help();
		}	
	} else {
		help();
	}
	return 0;
}
