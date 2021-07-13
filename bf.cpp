/*
 * Example BrainFuck Code: [Oprand should be 8-bits]
 * Addition: ,>,[<+>-]<.
 * Multiplication: ,>>>,[<<<[>+>+<<-]>[<+>-]>>-]<.
 * Hello World: +++[>+++<-]>[>++<-]>[>++++<-]>.[-]<<<+++++[>+++++<-]>[>++++<-]>+.+++++++..+++.
 *              <<++++[>++++<-]>[<++>-]<.>>++++++++.--------.+++.------.--------.<<+.[-]+++++[>++<-]>.
 */

#include <iostream>
#include <vector>
#include <stack>

std::vector<char> tape(1,0), prog;
std::stack<int> loop;
int ptr = 0, ip = 0;

inline void Right(){
	if(tape.size() == ptr + 1) tape.push_back(0);
	ptr++;
}

inline void Left(){
	ptr--;
	if(ptr < 0) ptr = 0;
}

inline void Insert(char c){
	prog.push_back(c);
}

int main(int argc, char *argv[]){
	if(argc < 2){
		fprintf(stderr, "Usage: ./bf <source_file> [-c]\n\t-c: Output is in char mode. By default it is in int mode.\
			\nInput data to be read by the program should be fed via cmd.\n");
		exit(-1);
	}
	FILE *fp = fopen(argv[1], "r");
	if(fp == NULL){
		fprintf(stderr, "%s: Source file not found.\n", argv[1]);
		exit(-1);
	}

	char c;
	while(fscanf(fp, "%c", &c) != EOF)
		if(c == '+' || c == '-' || c == '<' || c == '>' || c == '[' || c == ']' || c == '.' || c == ',') Insert(c);
	const int size = prog.size();
	int readin, cnt = 0;
	bool skip = false, charmode = argc > 2 && !strcmp(argv[2], "-c");
	
	while(true){
		if(ip == size) return 0;
		c = prog[ip];
		if(skip){
			if(c == ']')
				if(cnt == 1) skip = false;
				else cnt--;
			if(c == '[') cnt++;
			ip++;
			continue;
		}

		switch (c)
		{
		case '+':
			tape[ptr] += 1;
			ip++;
			break;
		case '-':
			tape[ptr] -= 1;
			ip++;
			break;
		case '>':
			Right();
			ip++;
			break;
		case '<':
			Left();
			ip++;
			break;
		case '.':
			if(charmode) printf("%c", tape[ptr]);
			else printf("%d ", tape[ptr]);
			fflush(stdout);
			ip++;
			break;
		case ',':
			scanf("%d", &readin);
			tape[ptr] = readin;
			ip++;
			break;
		case '[':
			if(!tape[ptr]) {skip = true; cnt = 1;}
			else loop.push(ip);
			ip++;
			break;
		case ']':
			ip = loop.top();
			loop.pop();
			break;
		default:
			break;
		}
	}
}
