#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <ctype.h>
#include <list>
#include <iostream>
//ㅇ
char STR[32][8] = { "const","int","char","void","main","if","else",
						"do","while","for","scanf","printf","return","+","-","*","/","<","<=",">",">=","==","!=","=",";",",","(",")","[","]","{","}" };
char temp[32][20] = { "RBRACE }\n", "CONSTTK const\n","INTTK int\n","CHARTK char\n","VOIDTK void\n","MAINTK main\n","IFTK if\n","ELSETK else\n",
				"DOTK do\n","WHILETK while\n","FORTK for\n","SCANFTK scanf\n","PRINTFTK printf\n","RETURNTK return\n","PLUS +\n","MINU -\n","MULT *\n","DIV /\n","LSS <\n","LEQ <=\n","GRE >\n","GEQ >=\n","EQL ==\n","NEQ !=\n"
				,"ASSIGN =\n","SEMICN ;\n","COMMA ,\n","LPARENT (\n","RPARENT )\n","LBRACK [\n","RBRACK ]\n","LBRACE {\n" };

FILE* fin;
FILE* fout;

int scan(char* ch, char c);
void print_syn();
//void syn_scan();
using namespace std;

int check_V(string ch, string void_[], int void_ck[], int k);
int check_ck(int void_ck[], int k);
int scan_hs(list<struct SYN>::iterator *it);
int scan_parent(list<struct SYN>::iterator* it);
struct SYN {
	SYN() {};
	string token;
	string ch;
};
struct SYN *syn;


list <struct SYN> token_list;

static int size_ = 1500;
int main_ = 0;
int main() {
	
	//char str[50];
	//char str_[50];

	char* p = (char*)calloc(size_, sizeof(char));
	char ch;
	int p_i = 0;
	//int k = 0; //str
	//int k_ = 0; //char
	int j = 0;



	fin = fopen("testfile.txt", "rt");
	fout = fopen("output.txt", "wt");


	while ((ch = fgetc(fin)) != EOF) {
		
		
		if ( ch == ' ') {
			scan(p, ch);
			memset(p, '\0', size_);
			p_i = 0;

		}
		else if (((ch == ',') || (ch == '(') || (ch == ')') || (ch == '{') || (ch == '}') || (ch == '[') || (ch == ']') || (ch == ';') || (ch == '-') || (ch == '+') || (ch == '/') || (ch == '*'))) {
			scan(p, ch);
			p[0] = ch;
			p[1] = '\0';
			scan(p, ch);
			memset(p, '\0', size_);
			p_i = 0;

		}
		else if (isdigit(ch)) {
			do {
				p[p_i] = ch;
				p[p_i++ + 1] = '\0';
				ch = fgetc(fin);
			} while (isdigit(ch));
			fseek(fin, -1, SEEK_CUR);
			scan(p, ch);
			memset(p, '\0', size_);
			p_i = 0;
		}
		else if (isalpha(ch) || ch == '_') {
			do {
				p[p_i] = ch;
				p[p_i++ + 1] = '\0';
				ch = fgetc(fin);
			} while (isalnum(ch) || ch == '_');
			fseek(fin, -1, SEEK_CUR);
			if (!scan(p, ch)) {
				//fprintf(fout, "IDENFR %s\n", p);
				syn->token = "IDENFR";
				syn->ch = p;
				token_list.push_back(*syn);
			}
			memset(p, '\0', size_);
			p_i = 0;
		}
		else if (ch == '"') {
			p[p_i] = ch;
			p[p_i++ + 1] = '\0';

			while ((ch = fgetc(fin)) != '"')
			{
				p[p_i] = ch;
				p[p_i + 1] = '\0';
				p_i++;
			}
			scan(p, ch);
			memset(p, '\0', size_);
			p_i = 0;
		}
		else if (ch == '\'') {
			p[p_i++] = ch;
			while ((ch = fgetc(fin)) != '\'')
			{
				p[p_i] = ch;
				p[p_i + 1] = '\0';
				p_i++;
			}
			scan(p, ch);
			memset(p, '\0', size_);
			p_i = 0;
		}
		else if ((ch == '<' || ch == '>' || ch == '=' || ch == '!')) {
			p[0] = ch;
			p[1] = '\0';
			ch = fgetc(fin);
			if (ch == '=') {
				p[1] = ch;
				p[2] = '\0';
				scan(p, ch);
			}
			else {
				fseek(fin, -1, SEEK_CUR);
				scan(p, ch);
			}
			memset(p, '\0', size_);
			p_i = 0;
			j = 0;
		}
		
	}

	
	print_syn();
	if(main_)
	fprintf(fout, "<主函数>\n");
	fprintf(fout, "<程序>\n");
	free(p);
	fclose(fin);
	fclose(fout);
}

void print_syn() {
	list<struct SYN>::iterator it = token_list.begin();
	
	int const_ = 0;  //const
	int int_ = 0;  //int 
	int fuc_ = 0; //函数
	int cs_ = 0;  //参数表
	int yin_=0;   //因子，项
	int yin = 0;
	int yfuc_ = 0;//<有返回值函数调用语句>
	int assign_ = 0;
	int printf_ = 0;
	int scanf_ = 0;
	int return_ = 0;
	int jz_ = 0;
	int jz_lie = 0;
	int if_ = 0;
	int if_define = 0;
	int while_ = 0;
	int for_ = 0;
	int for_c = 0;
	string void_[40];
	int k_ = 1;
	int void_ck[40];
	memset(void_ck, -1, sizeof(void_ck));
	
	int fuc_define = 0;		
	int vfuc_ = 0;

	int x = 0;  
	int y = 0;
	//int else_ = 0;
	
	int x_ = 0;
	int y_ = 0;

	int x_x = 0;
	int y_y = 0;

	int if_t = 0;    //{语句}
	int if_c = 0;	// next { ? 
	//int else_t = 0;  //{语句}
	int else_c = 0; // next { ? 

	int while_t = 0; //{语句}
	int while_c = 0;
	int yfuc_jz=0;
	int vfuc_jz=0;

	
	int for_c_ = 0;

	int if_jz = 0;
	int for_jz = 0;

	int yin__ = 0;
	int while_define = 0;
	int for_define = 0;
	int else_define = 0;

	int p_c = 0;

	int b = 0;
	int lbrace_c = 0;

	int else_4 = 0;
	int for_4 = 0;
	int if_4 = 0;
	int while_4 = 0;
	int jz_c = 0;
	int jz_lie_c = 0;
	int do_ = 0;
	int do_c = 0;

	int c_c = 0;
	int else_4_c = 0;
	int else_t = 0;
	//*************************************************************************************
	while (it != token_list.end()) {
		int control = 1;
		int sum = 0;
		if (it->token == "CONSTTK") {
			const_ = 1;
		}
		else if ((it->token == "INTTK" || it->token == "CHARTK")) {
			it++;
			it++;
			if (it->token == "SEMICN") {
				int_ = 1;
				it--;
				it--;
			}
			else if (it->token == "COMMA") {
				it++;
				if (it->token == "INTTK" || it->token == "CHARTK"|| it->token == "RPARENT") {
					cs_ = 1;
					it--;
					it--;
					it--;
				}
				else {
					int_ = 1;
					it--;
					it--;
					it--;
				}
			}
			else if (it->token == "RPARENT") {
				cs_ = 1;
				it--;
				it--;

			}
			else if (it->token=="LBRACK") {
					int_ = 1;
					it--;
					it--;

			}
			else if (it->token == "LPARENT") {

				it++;
				if (it->token == "RPARENT") {
					cs_ = 1;
				}
				it--;
				it--;
				it--;
				fuc_ = 1;
			}
			else {
				//int_ = 1;   int buchongfen

			
				it--;
				it--;
			}

		}
		else if (it->token == "VOIDTK") {
			it++;
			if (it->token == "IDENFR") {
				void_[k_++] = it->ch;
				vfuc_ = 1;
				it++;
				it++;
				if (it->token == "RPARENT") {
					cs_ = 1;
				}
				it--;
				it--;
			}
			
			
			it--;

		}
		else if (it->token == "IDENFR"&& !check_V(it->ch, void_, void_ck, k_) &&!int_) {
			it--;
			if (it->token != "INTTK" && it->token != "CHARTK" && it->token != "VOIDTK") {
				it++; //IDENFR
				it++;
				if ((it->token != "ASSIGN") && (it->token != "LPARENT")) {
					yin_ = 1;
					it--;
				}
				else if (it->token == "LPARENT"&&it->token!="LBRACK") {
					yfuc_ ++;
					it--;
				}
				else {
					it--;
				}

			}
			else {
				it++;
			}
		}
		else if (it->token == "ASSIGN") {
			assign_ = 1;
		}
		else if (it->token == "PRINTFTK") {
			printf_ = 1;
		}
		else if (it->token == "SCANFTK") {
			scanf_ = 1;
		}
		else if (it->token == "RETURNTK") {
			return_ = 1;
		}
		else if (it->token == "LBRACE") {
			lbrace_c++;
		}
		else if (it->token == "RBRACE") {
			lbrace_c--;
		}
		else if (it->token == "IFTK") {
			if_ = 1;
		}
		else if (it->token == "FORTK") {
			for_ = 1;
		
		}
		else if (it->token == "DOTK") {
			do_ ++;
			it++;
			if (it->token == "LBRACE") {
				do_c = 1;
			}
			else {
				do_c = 2;
			}
			it--;
		}
		else if (it->token == "WHILETK") {
			while_ = 1;
		
		}
		else if (it->token == "ELSETK") {
		
			it++;
			if (it->token == "LBRACE") {
				else_c = 1;
				else_t = 1;
				else_define++;
			}
			else if(it->token=="SEMICN"){
				else_c = 3;
				//else_define++;
			}
			else if(it->token!="IFTK"&& it->token != "WHILETK"&& it->token != "FORTK"){
				else_c = 2;
				//else_define++;
			}
			else if (it->token == "DOTK") {
				else_c = 4;
				//else_define++;
			}
			else {
				if (else_c == 1&&else_t) {
					else_4_c = else_4;
					else_t = 0;
				}
				else_4 ++;
			}
			it--;

			it--;
			if (it->token == "RBRACE") {
				fprintf(fout, "<语句>\n");
			}
			it++;
		}
		else if (it->token == "MAINTK") {
			main_ = 1;
		}



		if (check_V(it->ch,void_,void_ck,k_)) {
			void_ck[check_V(it->ch, void_, void_ck, k_)]++;
		}
		if (it->token=="LBRACE") {
			x++;
		}
		else if (it->token == "RBRACE") {
			y++;
		}
		else if (it->token == "LPARENT") {
			x_++;
		}
		else if (it->token == "RPARENT") {
			y_++;
		}
		else if (for_ && it->token == "SEMICN") {
			for_c_++;
		}

		if (it->token == "LPARENT" && yfuc_) {
			x_x++;
		}// ((if_c == 1) || (for_c == 1) || (while_c == 1))
		else if (it->token == "RPARENT" && yfuc_) {
			y_y++;
		}
		//*************************************************************************************

		sum = const_ || int_ || cs_ || fuc_;


		//*************************************************************************************
		if (it->token == "SEMICN" && const_) {
			fprintf(fout, "<常量定义>\n");
			fprintf(fout, "%s ", (it->token.data()));
			fprintf(fout, "%s\n", (it->ch.data()));
			it++;
			if (it->token != "CONSTTK") {
				fprintf(fout, "<常量说明>\n");
				c_c = 1;
			}
			it--;
			int_ = 0;
			yin_ = 0;
			const_ = 0;
			control = 0;
			assign_ = 0;
			
		}
		else if (it->token == "SEMICN" && int_) {
			fprintf(fout, "<变量定义>\n");
			fprintf(fout, "%s ", (it->token.data()));
			fprintf(fout, "%s\n", (it->ch.data()));
			it++;
			if (it->token != "INTTK" && it->token != "CHARTK") {
				fprintf(fout, "<变量说明>\n");
				it--;
				c_c = 1;
			}
			else {
				it++;
				it++;
				if (it->token != "SEMICN"&& it->token != "COMMA"&& it->token != "LBRACK") {
					fprintf(fout, "<变量说明>\n");
					c_c = 1;
					it--;
					it--;
					it--;
				}
				else {
					it--;
					it--;
					it--;
				}
			}
			assign_ = 0;
			control = 0;
			int_ = 0;
		}
		else if (it->token == "SEMICN" && printf_) {
			fprintf(fout, "<写语句>\n");
			fprintf(fout, "%s ", (it->token.data()));
			fprintf(fout, "%s\n", (it->ch.data()));
			jz_ = 1;
			control = 0;
			printf_ = 0;

		}
		else if (it->token == "SEMICN" && scanf_) {
			fprintf(fout, "<读语句>\n");
			fprintf(fout, "%s ", (it->token.data()));
			fprintf(fout, "%s\n", (it->ch.data()));
			jz_ = 1;
			control = 0;
			scanf_ = 0;

		}
		else if (it->token == "SEMICN" && return_) {
			fprintf(fout, "<返回语句>\n");
			fprintf(fout, "%s ", (it->token.data()));
			fprintf(fout, "%s\n", (it->ch.data()));
			jz_ = 1;

			control = 0;
			return_ = 0;

		}
		else if (it->token == "SEMICN" && assign_ && !sum&&!for_&&!if_&&!vfuc_jz) {
	
			

				it--;
				if (it->token == "RBRACK") {
					fprintf(fout, "<因子>\n");
					fprintf(fout, "<项>\n");
					fprintf(fout, "<表达式>\n");
				}
				it++;
				fprintf(fout, "<赋值语句>\n");
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				jz_ = 1;
				control = 0;
				assign_ = 0;
			
		

		}
		else if (it->token == "SEMICN"&&for_&&!assign_) {
			it--;
			it--;
			//if (it->token == "LSS" || it->token == "LEQ" || it->token == "GRE" || it->token == "GEQ" || it->token == "EQL" || it->token == "NEQ"||it->token=="SEMICN") {
					fprintf(fout, "<条件>\n");
			//}
			it++;
			it++;
			fprintf(fout, "%s ", (it->token.data()));
			fprintf(fout, "%s\n", (it->ch.data()));
			control = 0;
		}
		/*else if (it->token == "SEMICN" && (if_c ||while_c||for_c||else_c)&&!for_&&!do_) {
			fprintf(fout, "%s ", (it->token.data()));
			fprintf(fout, "%s\n", (it->ch.data()));
			control = 0;
			it++;
			if (it->token == "RBRACE") {
				it--;
				jz_lie++;
			}
			else {
				it--;
			}
			jz_ = 1;
		}*/
		/*else if (it->token == "SEMICN"&&main_) {
		it++;
		if (it->token == "RBRACE") {
			y++;
			if (x == y) {
				it--;
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				fprintf(fout, "<语句>\n");
				fprintf(fout, "<语句列>\n");
				y--;
			}
			else {
				y--;
				it--;
			}
		}
		else {
			it--;
		}
		
			control = 0;
		}*/
		else if (it->token == "SEMICN"&&!for_) {
			if (yfuc_jz) {
				yfuc_jz = 0;
				it++;
				if(it->token == "RETURNTK"||it->token=="SCANFTK"||it->token == "PRINTFTK"/*||it->token=="IDENFR"*/) {
					it--;
					jz_ = 1;
					fprintf(fout, "%s ", (it->token.data()));
					fprintf(fout, "%s\n", (it->ch.data()));
					//fprintf(fout, "<语句>\n");
					control = 0;
				}
				else if (lbrace_c) {
					it--;
					fprintf(fout, "%s ", (it->token.data()));
					fprintf(fout, "%s\n", (it->ch.data()));
					control = 0;
					jz_ = 1;
				}
				else {
					it--;
				}
				
				
			}
			else if (vfuc_jz) {
				vfuc_jz = 0;
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				control = 0;
				jz_ = 1;
			}
			else if (lbrace_c) {
			
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				control = 0;
				jz_ = 1;
			}
			else {
				it++;
				if (it->token == "RETURNTK"|| it->token == "SCANFTK" || it->token == "PRINTFTK") {
					it--;
					jz_ = 1;
					fprintf(fout, "%s ", (it->token.data()));
					fprintf(fout, "%s\n", (it->ch.data()));
					//fprintf(fout, "<语句>\n");
					control = 0;
				}
				else {
					it--;
				}


				jz_lie = 0;
			}
			
		}
		else if (it->token == "INTCON") {
			it--;
			if (it->token == "LBRACK"&&int_) {
				it++;
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				fprintf(fout, "<无符号整数>\n");
				control = 0;
			}
			else if (const_) {
				it++;
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				fprintf(fout, "<无符号整数>\n");
				fprintf(fout, "<整数>\n");

				control = 0;
			}
			else {
					it++;
					it++;
				 if (for_ && it->token == "RPARENT") {
					 it--;
					fprintf(fout, "%s ", (it->token.data()));
					fprintf(fout, "%s\n", (it->ch.data()));
					fprintf(fout, "<无符号整数>\n");
					fprintf(fout, "<步长>\n");
					for_ = 0;
					for_c_ = 0;
					control = 0;
					assign_ = 0;
					it++;
					it++;
					if (it->token == "LBRACE") {
						for_c = 1;
						for_define++;
					}
					else if(it->token == "SEMICN"){
						for_c = 3;
						//for_define++;
					}
					else if(it->token != "IFTK" && it->token != "WHILETK" && it->token != "FORTK"&&it->token!="DOTK"){
						for_c = 2;
						//for_define++;
					}
					else if (it->token == "DOTK") {
						for_c = 4;
					}
					else {
						for_4 = 1;
					}
					it--;
					it--;
					
				 }
				 else {
					 it--;
					 fprintf(fout, "%s ", (it->token.data()));
					 fprintf(fout, "%s\n", (it->ch.data()));
					 fprintf(fout, "<无符号整数>\n");
					 fprintf(fout, "<整数>\n");
					 yin = 1;
					 control = 0;
				 }
			}

		}
		else if (it->token == "LPARENT" && fuc_) {
			fprintf(fout, "<声明头部>\n");
			fprintf(fout, "%s ", (it->token.data()));
			fprintf(fout, "%s\n", (it->ch.data()));
			fuc_ = 0;
			fuc_define = 1;
			control = 0;

		}
		else if (it->token == "RPARENT") {
			if (cs_) {
				if (x_ == y_) {
					fprintf(fout, "<参数表>\n");
					fprintf(fout, "%s ", (it->token.data()));
					fprintf(fout, "%s\n", (it->ch.data()));
					cs_ = 0;
					control = 0;
				}
			}
			else if (check_ck(void_ck,k_)&& scan_parent(&it)) {
				fprintf(fout, "<值参数表>\n");
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				fprintf(fout, "<无返回值函数调用语句>\n");
				vfuc_jz = 1;
				void_ck[check_ck(void_ck, k_)] = 0;
				control = 0;
			}
			else if (yfuc_&&scan_parent(&it)) {
				
				fprintf(fout, "<值参数表>\n");
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				fprintf(fout, "<有返回值函数调用语句>\n");
				p_c = 1;
				yin = 1;
				yfuc_jz = 1;
				yfuc_--;
				control = 0;
				x_x = 0;
				y_y = 0;
			}
			/*else if (yfuc_) {

				fprintf(fout, "<值参数表>\n");
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				fprintf(fout, "<有返回值函数调用语句>\n");
				p_c = 1;
				yin = 1;
				yfuc_jz = 1;
				yfuc_ = 0;
				control = 0;
				x_x = 0;
				y_y = 0;
			}*/
			
			else if (if_) {
				it++;
				if ( it->token == "LBRACE") {
					fprintf(fout, "<条件>\n");
					if_ = 0;
					if_define++;
					if_t = 1;
					if_c = 1;
				}
				else if (x_==y_) {
					fprintf(fout, "<条件>\n");
					if_ = 0;
					
					if_t = 1;
					if (it->token == "LBRACE") {
						if_c = 1;
						if_define++;
					}
					else if (it->token == "SEMICN") {
						if_c = 3;

					}
					else if(it->token!="IFTK"&& it->token != "WHILETK"&& it->token != "FORTK"){
						if_c = 2;
						//if_define++;
					}
					else if (it->token == "DOTK") {
						if_c = 4;
					}
					else {
						if (it->token != "ELSETK")
						if_4++;
					}

				}
				/*else if (it->token == "SEMICN" ) {
					fprintf(fout, "<条件>\n");
					if_ = 0;
					//if_define++;
					if_t = 1;
					if_c = 3;

				}*/
				else if (it->token == "LSS" || it->token == "LEQ" || it->token == "GRE" || it->token == "GEQ" || it->token == "EQL" || it->token == "NEQ") {
					yin = 1;
				}
				else if (it->token == "RPARENT") {
					yin = 1;	
				}
				else if (it->token == "RBRACK") {
					yin = 1;
				}
				/*else if (yin__) {
					it++;
					if (it->token == "RPARENT") {
						it--;
						yin = 1;
						control = 0;
						p_c = 1;
					}
					else if (it->token == "PLUS" || it->token == "MINU") {
						it--;
						yin = 1;
						control = 0;
						p_c = 1;
					}
					else if (it->token == "MULT" || it->token == "DIV") {
						it--;
						yin = 1;

						p_c = 0;
						control = 0;
					}
					else if (it->token == "SEMICN" && assign_ && !printf_) {
						it--;
						yin = 1;

						p_c = 0;
						control = 0;
					}
					else if (it->token == "RBRACK") {
						it--;
						yin = 1;

						control = 0;
						p_c = 1;
					}
					else if (it->token == "COMMA") {
						it--;
						yin = 1;

						control = 0;
					}
					else {
						it--;
					}
					yin__ = 0;


				}*/
				it--;
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				
				control = 0;
			}
			else if (while_) {
				it++;
				if (it->token == "LBRACE") {
					fprintf(fout, "<条件>\n");
					while_ = 0;
					while_t = 1;
					while_define ++;
					
				}
				else if (x_ == y_) {
					fprintf(fout, "<条件>\n");
					while_ = 0;
					while_t = 1;
					
					if (it->token == "LBRACE") {
						while_c = 1;
						while_define++;
					}
					else if (it->token == "SEMICN") {
						while_c = 3;

					}
					else if ((it->token != "IFTK" && it->token != "WHILETK" && it->token != "FORTK"&& it->token != "DOTK")) {
						while_c = 2;
						//while_define++;
					}
					else {
						if(!do_)
						while_4 = 1;
					}
				}
				else if (it->token == "LSS" || it->token == "LEQ" || it->token == "GRE" || it->token == "GEQ" || it->token == "EQL" || it->token == "NEQ") {
					yin = 1;
					control = 0;
				}
				else if (yin__) {
					it++;
					if (it->token == "RPARENT") {
						it--;
						yin = 1;
						control = 0;
						p_c = 1;
					}
					else if (it->token == "PLUS" || it->token == "MINU") {
						it--;
						yin = 1;
						control = 0;
						p_c = 1;
					}
					else if (it->token == "MULT" || it->token == "DIV") {
						it--;
						yin = 1;
				
						p_c = 0;
						control = 0;
					}
					else if (it->token == "SEMICN" && assign_ && !printf_&&!scanf_) {
						it--;
						yin = 1;
					
						p_c = 0;
						control = 0;
					}
					else if (it->token == "RBRACK") {
						it--;
						yin = 1;
				
						control = 0;
						p_c = 1;
					}
					else if (it->token == "COMMA") {
						it--;
						yin = 1;
					
						control = 0;
					}
					else {
						it--;
					}
					yin__ = 0;

				
				}
				it--;
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				if (do_) {
					if (while_c == 3) {
						it++;
						it++;
						//do_c = 0;
						if (it->token != "WHILETK") {
							do_--;
							fprintf(fout, "<循环语句>\n");
							jz_ = 1;
							while_c = 0;

						}
						it--;
						it--;

					}
					else {
						do_--;
						fprintf(fout, "<循环语句>\n");
						jz_ = 1;
						while_c = 0;
					}
					if (for_c == 4) {				
						fprintf(fout, "<语句>\n");
						fprintf(fout, "<循环语句>\n");
						for_c = 0;
					}
					else if (else_c==4) {
						fprintf(fout, "<语句>\n");
						fprintf(fout, "<条件语句>\n");
						else_c = 0;
					}
					
				}
				control = 0;
			}
			else if (yin__) {
				it++;
				if (it->token == "RPARENT") {
					it--;
					yin = 1;
					fprintf(fout, "%s ", (it->token.data()));
					fprintf(fout, "%s\n", (it->ch.data()));
					control = 0;
					p_c = 1;
				}
				else if (it->token == "PLUS"|| it->token == "MINU") {
					it--;
					yin = 1;
					fprintf(fout, "%s ", (it->token.data()));
					fprintf(fout, "%s\n", (it->ch.data()));
					control = 0;
					p_c = 1;
				}
				else if (it->token == "MULT" || it->token == "DIV") {
					it--;
					yin = 1;
					fprintf(fout, "%s ", (it->token.data()));
					fprintf(fout, "%s\n", (it->ch.data()));
					p_c = 0;
					control = 0;
				}
				else if (it->token == "SEMICN"&&assign_&&!printf_) {
					it--;
					yin = 1;
					fprintf(fout, "%s ", (it->token.data()));
					fprintf(fout, "%s\n", (it->ch.data()));
					p_c = 0;
					control = 0;
				}
				else if (it->token == "RBRACK") {
					it--;
					yin = 1;
					fprintf(fout, "%s ", (it->token.data()));
					fprintf(fout, "%s\n", (it->ch.data()));
					control = 0;
					p_c = 1;
				}
				else if (it->token == "COMMA") {
					it--;
					yin = 1;
					fprintf(fout, "%s ", (it->token.data()));
					fprintf(fout, "%s\n", (it->ch.data()));
					control = 0;
				}
				else {
					it--;
				}
				yin__ = 0;
				
			}
			else {
				it++;
				if (it->token == "RBRACK") {
					yin = 1;
				}
				it--;
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				control = 0;
			}


		}
		else if ( (it->token == "RBRACE")&& !(fuc_define && (y == x))&&!( vfuc_ && (y == x))&&!(main_ && (y == x))) {

			int q = 0;
				if (if_define|| (else_define) || (for_define) || (while_define)) {
					
					q= scan_hs(&it);
					//fprintf(fout, "%d ", q);
					if (q == 1) {
						it++;
						if (it->token != "ELSETK") {
							if_jz = 1;
							jz_ = 1;
							
						}
						if_define--;
						it--;
						if_c = 0;
					}
					else if (q==2) {
						if_jz = 1;
						jz_ = 1;
						else_c = 0;
						else_define--;
					}
					else if (q == 3) {
						for_jz = 1;
						for_c = 0;
						jz_ = 1;
						for_define--;
					}
					else if (q == 4) {
						for_jz = 1;
						while_c = 0;
						while_define--;
						jz_ = 1;
					}
					
					//c = q;
					fprintf(fout, "%s ", (it->token.data()));
					fprintf(fout, "%s\n", (it->ch.data()));
					if (q == 5) {
						fprintf(fout, "<语句>\n");
						jz_c = 1;
					}
					control = 0;
				}
				else {
					//q = scan_hs(&it);
					//fprintf(fout, "%d ", q);
				}
				
			
			
		}
		else if (it->token == "IDENFR" && yin_ && !sum&&!scanf_&&!(for_&&(for_c_==2))) {
			fprintf(fout, "%s ", (it->token.data()));
			fprintf(fout, "%s\n", (it->ch.data()));
			it++;
			if (it->token != "LBRACK"&&it->token!="LPARENT") {
				yin = 1;
			}
			it--;
			yin_ = 0;
			control = 0;
		}
		else if (it->token == "PLUS" || it->token == "MINU" ) {
			it--;
			if(it->token=="CHARCON"){
				fprintf(fout, "<因子>\n");
				fprintf(fout, "<项>\n");
			}
			it++;
		}
		else if ( it->token == "MULT" || it->token == "DIV") {
		it--;
		if (it->token == "CHARCON") {
			fprintf(fout, "<因子>\n");
			
		}
		it++;
		}
		else if (it->token == "CHARCON" && !const_ & !int_) {
			it++;
			if (it->token == "RPARENT" || it->token == "COMMA") {
				yin = 1;
				it--;
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				control = 0;
			}
			else if (it->token == "SEMICN" && !int_ && !const_) {
				yin = 1;
				it--;
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				control = 0;
			}
			else {
				it--;
			}


		}
		else if (it->token == "LBRACE") {
			it++;
			if (it->token == "RBRACE") {
				it--;
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				fprintf(fout, "<语句列>\n");
				jz_lie_c = 1;
				control = 0;
			}
			else {
				it--;
			}

		}
		else if (it->token == "RBRACK") {
		if (yin__) {
			it++;
			if (it->token == "RPARENT" || it->token == "RBRACK") {
				yin = 1;
				it--;
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				control = 0;
				yin__ = 0;
			}
			else if (it->token == "PLUS" || it->token == "MINU"|| it->token == "MULT" || it->token == "DIV"|| it->token == "LSS" || it->token == "LEQ" || it->token == "GRE" || it->token == "GEQ" || it->token == "EQL" || it->token == "NEQ") {
				yin = 1;
				it--;
				fprintf(fout, "%s ", (it->token.data()));
				fprintf(fout, "%s\n", (it->ch.data()));
				control = 0;
				yin__ = 0;
			}
			else {
				it--;
				yin__ = 0;
			}
		}
			

	}
		


		


	//*************************************************************************************

	if (it->token == "RBRACE" && (x == y) && if_t) {
		if_t = 0;
	}
	else if ((vfuc_ || fuc_define) && it->token == "SEMICN") {
		jz_lie++;
	}
	




	if (jz_) {
		
		fprintf(fout, "<语句>\n");
		jz_c = 1;
		it++;
		jz_lie++;
		if ( if_c == 2 || for_c == 2||while_c==2||else_c==2) {
			if (if_c==2) {
				
				if (it->token != "ELSETK") {
					fprintf(fout, "<条件语句>\n");
					fprintf(fout, "<语句>\n");
					if (if_4 || else_4) {
						it++;
						if (it != token_list.end()) {
							if (it->token != "ELSETK") {
								if (else_4) {
									for (int i = 0; i < else_4; i++) {
										fprintf(fout, "<条件语句>\n");
										fprintf(fout, "<语句>\n");

									}
									else_4 = 0;
								}
								else if (if_4) {
									for (int i = 0; i < if_4; i++) {
										fprintf(fout, "<条件语句>\n");
										fprintf(fout, "<语句>\n");

									}
									if_4 = 0;
								}


							}
						}
						it--;
					}
					else if (while_4 || for_4) {
						fprintf(fout, "<循环语句>\n");
						fprintf(fout, "<语句>\n");

						if (while_4) while_4 = 0;
						else for_4 = 0;


					}
				}
			
				if_c = 0;
				//if_define--;
			}
			else if(else_c==2){
				fprintf(fout, "<条件语句>\n");
				fprintf(fout, "<语句>\n");
					else_c = 0;
					if (if_4 || else_4) {
						it++;
						if (it != token_list.end()) {
							if (it->token != "ELSETK") {
								if (else_4) {
									int i = 0;
									for (i = 0; i < else_4 - else_4_c; i++) {
										fprintf(fout, "<条件语句>\n");
										fprintf(fout, "<语句>\n");

									}
									else_4 = else_4 - i;
								}
								else if (if_4) {
									for (int i = 0; i < if_4; i++) {
										fprintf(fout, "<条件语句>\n");
										fprintf(fout, "<语句>\n");

									}
									if_4 = 0;
								}


							}
						}
						it--;
					}
					else if (while_4 || for_4) {
						fprintf(fout, "<循环语句>\n");
						fprintf(fout, "<语句>\n");

						if (while_4) while_4 = 0;
						else for_4 = 0;


					}
					//else_define--;
								
			}
			else if (for_c==2||while_c==2) {
				fprintf(fout, "<循环语句>\n");
				fprintf(fout, "<语句>\n");
				if (if_4 || else_4) {
					it++;
					if (it != token_list.end()) {
						if (it->token != "ELSETK") {
							if (else_4) {
								int i = 0;
								for (i = 0; i < else_4 - else_4_c; i++) {
									fprintf(fout, "<条件语句>\n");
									fprintf(fout, "<语句>\n");

								}
								else_4 = else_4 - i;
							}
							else if (if_4) {
								for (int i = 0; i < if_4; i++) {
									fprintf(fout, "<条件语句>\n");
									fprintf(fout, "<语句>\n");

								}
								if_4 = 0;
							}


						}
					}
					it--;
				}
				else if (while_4 || for_4) {
					fprintf(fout, "<循环语句>\n");
					fprintf(fout, "<语句>\n");

					if (while_4) while_4 = 0;
					else for_4 = 0;


				}
				if (while_c==2) {
					while_c = 0;
					//while_define--;
				}
				else {
					for_c = 0;
					//for_define--;
				}
			}
			jz_lie++;

		}
		else if (if_c == 3 || while_c == 3 || else_c == 3 || for_c == 3) {
			if (if_c == 3) {
				
				if (it->token != "ELSETK") {
					
					
					fprintf(fout, "<条件语句>\n");
					fprintf(fout, "<语句>\n");
					if (if_4 || else_4) {
						it++;
						if (it != token_list.end()) {
							if (it->token != "ELSETK") {
								if (else_4) {
									int i = 0;
									for (i = 0; i < else_4-else_4_c; i++) {
										fprintf(fout, "<条件语句>\n");
										fprintf(fout, "<语句>\n");

									}
									else_4 = else_4 - i;
								}
								else if (if_4) {
									for (int i = 0; i < if_4; i++) {
										fprintf(fout, "<条件语句>\n");
										fprintf(fout, "<语句>\n");

									}
									if_4 = 0;
								}


							}
						}
						it--;
					}
					else if (while_4 || for_4) {
						fprintf(fout, "<循环语句>\n");
						fprintf(fout, "<语句>\n");

						if (while_4) while_4 = 0;
						else for_4 = 0;


					}
					
					if_c = 0;

				}
				else {
					
					if_c = 0;
				
				}
			}
			else if (else_c == 3) {
				
				fprintf(fout, "<条件语句>\n");
				fprintf(fout, "<语句>\n");
				if (if_4 || else_4) {
					it++;
					if (it != token_list.end()) {
						if (it->token != "ELSETK") {
							if (else_4) {
								for (int i = 0; i < else_4; i++) {
									fprintf(fout, "<条件语句>\n");
									fprintf(fout, "<语句>\n");

								}
								else_4 = 0;
							}
							else if (if_4) {
								for (int i = 0; i < if_4; i++) {
									fprintf(fout, "<条件语句>\n");
									fprintf(fout, "<语句>\n");

								}
								if_4 = 0;
							}


						}
					}
					it--;
				}
				else if (while_4 || for_4) {
					fprintf(fout, "<循环语句>\n");
					fprintf(fout, "<语句>\n");

					if (while_4) while_4 = 0;
					else for_4 = 0;


				}
				else_c = 0;
			}
			else {	
				/*if (while_c == 3 && do_) {
					do_--;

				}*/
				fprintf(fout, "<循环语句>\n");
				fprintf(fout, "<语句>\n");
				if (while_c == 3) {
					while_c = 0;
				}
				else {
					for_c = 0;
				}

			}
		}
		 else if (if_jz) {
			fprintf(fout, "<条件语句>\n");
			fprintf(fout, "<语句>\n");
			if_jz = 0;
			if (if_4||else_4) {
				
				if (it->token != "ELSETK") {
					if (else_4) {
						for (int i = 0; i < else_4; i++) {
							fprintf(fout, "<条件语句>\n");
							fprintf(fout, "<语句>\n");
							
						}
						else_4 = 0;
					}
					else if (if_4) {
						for (int i = 0; i < if_4; i++) {
							fprintf(fout, "<条件语句>\n");
							fprintf(fout, "<语句>\n");

						}
						if_4 = 0;
					}


				}
			
				
			}
			else if (while_4|| for_4) {
				fprintf(fout, "<循环语句>\n");
				fprintf(fout, "<语句>\n");

				if (while_4) while_4 = 0;
				else for_4 = 0;
				

			}
			

			
			if (it != token_list.end()) {
				if (it->token == "RBRACE") {
					//fprintf(fout, "<语句列>\n");
				}
				
			}
			
		}
		else if (for_jz) {
			fprintf(fout, "<循环语句>\n");
			fprintf(fout, "<语句>\n");
			for_jz = 0;
			if (if_4||else_4) {
				
				if (it->token != "ELSETK"&&it->token!="RBRACE") {
					if (else_4) {
						for (int i = 0; i < else_4; i++) {
							fprintf(fout, "<条件语句>\n");
							fprintf(fout, "<语句>\n");

						}
							else_4 = 0;
					}
					else if (if_4) {
						for (int i = 0; i < if_4; i++) {
							fprintf(fout, "<条件语句>\n");
							fprintf(fout, "<语句>\n");

						}
						if_4 = 0;
					}
					
					
				}
				

				
			}
			else if (while_4 || for_4) {
				fprintf(fout, "<循环语句>\n");
				fprintf(fout, "<语句>\n");

				if (while_4) while_4 = 0;
				else for_4 = 0;


			}
			
			if (it != token_list.end()) {
				if (it->token == "RBRACE") {
					//fprintf(fout, "<语句列>\n");
				}
				
			}
			
				
		}
	
			if ((jz_lie > 1) && !(it->token == "PRINTFTK" || it->token == "SCANFTK" || it->token == "RETURNTK" || it->token == "IFTK" || it->token == "DOTK" || it->token == "WHILETK" || it->token == "FORTK" || it->token == "ELSETK" || it->token == "IDENFR" || it->token == "SEMICN"||it->token=="LBRACE")) {
				fprintf(fout, "<语句列>\n");
				jz_lie = 1;
				b = 1;
				jz_lie_c = 1;
			}
			else {
				b = 0;
				jz_lie_c = 0;
			}
			it--;
			jz_ = 0;

		}
		else if (yin) {
			it++;
			jz_c = 0;
			jz_lie_c = 0;
			if ((it->token == "PLUS" || it->token == "MINU" || it->token == "MULT" || it->token == "DIV")) {
				it--;
				if (it->token == "RPARENT") {
					if (p_c) {
						it++;
						if (it->token == "MULT" || it->token == "DIV") {
							fprintf(fout, "<因子>\n");
							p_c = 0;
						}
						else {
							fprintf(fout, "<因子>\n");
							fprintf(fout, "<项>\n");
							p_c = 0;
						}
						it--;
						
						
					}
					else {
						fprintf(fout, "<因子>\n");
					}
					
				}
				else {
					it++;
					if (it->token == "MULT" || it->token == "DIV") {
						fprintf(fout, "<因子>\n");
					}
					else {
						fprintf(fout, "<因子>\n");
						fprintf(fout, "<项>\n");
					}
					it--;
				}
				
			}
			else if(it->token != "ASSIGN"){
				
				it--;
				if (it->token == "RPARENT") {
					it++;
					if (!(yfuc_jz&&(it->token=="SEMICN"))||assign_) {
						fprintf(fout, "<因子>\n");
						fprintf(fout, "<项>\n");
						fprintf(fout, "<表达式>\n");
						yin__ = 1;
					}
					it--;
				}
				else {
					fprintf(fout, "<因子>\n");
					fprintf(fout, "<项>\n");
					fprintf(fout, "<表达式>\n");
					yin__ = 1;
				}
			


			}
			else {
				it--;
				yin__ = 0;
			}
			c_c = 0;
			yin = 0;
		}
		else {
		if (it->token != "RBRACE"&&it->token!="LBRACE") {
			jz_c = 0;
			jz_lie_c = 0;
		}
			jz_lie = 1;
			yin__ = 0;
		}
	
		//*******************************************
	

	
	
	







		if (it->token == "SEMICN") {
			vfuc_jz = 0;
			yfuc_jz = 0;
			assign_ = 0;
			if (if_c == 3||if_c==2) {
				if_c = 0;
			}
			else if (else_c == 3 || else_c == 2) {
				else_c = 0;
			}
			else if (while_c == 3 || while_c == 2) {
				while_c = 0;
			}
			else if(for_c==3 || for_c == 2){
				for_c = 0;
			}
		}










		if (control) {
			 if (it->token == "RBRACE" && fuc_define && (y == x)) {		
				fprintf(fout, "<复合语句>\n");
			}
			else if (it->token == "RBRACE" && vfuc_ && (y == x)) {
				 it--;
				 if (it->token == "SEMICN") {
					 if (!jz_c&&!c_c) {
						 fprintf(fout, "<语句>\n");
					 }
					if (!jz_lie_c) {
						fprintf(fout, "<语句列>\n");
					 }
				 }
				 else if (it->token=="LBRACE") {
					 if (!jz_lie_c) {
						 fprintf(fout, "<语句列>\n");
					 }
				 }
				 else {
					 
					 if (!jz_lie_c) {
						 fprintf(fout, "<语句列>\n");
					 }
					 else if (!jz_c && !c_c) {
						 fprintf(fout, "<语句>\n");
					 }

				 }
				 it++;
			
			fprintf(fout, "<复合语句>\n");
			}
			else if (it->token == "RBRACE" && main_ && (y == x)) {

				 it--;
				 if (it->token != "LBRACE") {
					 if (jz_c == 0) {
						 fprintf(fout, "<语句>\n");
					 }
				 }
				 it++;
				 

				
					 it--;
					 if (it->token == "LBRACE"&&!jz_lie_c) {
						 fprintf(fout, "<语句列>\n");
						 jz_lie_c = 1;
					 }
					 else {
						 if (!jz_lie_c) {
							 fprintf(fout, "<语句列>\n");
						 }
						 else if (!jz_c && !c_c&&!jz_lie_c) {
							 fprintf(fout, "<语句>\n");
						 }
						 

					 }
					 it++;

				 fprintf(fout, "<复合语句>\n");
				
			 }

			fprintf(fout, "%s ", (it->token.data()));
			fprintf(fout, "%s\n", (it->ch.data()));
			if (it->token == "STRCON") {
				fprintf(fout, "<字符串>\n");
			}
			else if (it->token == "RBRACE" && fuc_define &&(y==x)) {
				fprintf(fout, "<有返回值函数定义>\n");
				jz_lie = 0;
				fuc_define = 0;
			}
			else if (it->token == "RBRACE" && vfuc_ && (y == x)) {
				fprintf(fout, "<无返回值函数定义>\n");
				jz_lie = 0;
				vfuc_ = 0;
			}
			else if (it->token == "RBRACE"&&((x-y)>0)) {
				
				fprintf(fout, "<语句>\n");
				it++;
				if (it->token == "RBRACE"/*||it->token=="LBRACE"*/) {
					
					fprintf(fout, "<语句列>\n");
				}
				it--;
			}
		
		}
	it++;
	}
}

int scan_parent(list<struct SYN>::iterator* it) {
	int i = 0;
	int x = 1;
	int y = 0;
	while (1) {

		(*it)--;
		i++;
		if ((*it) != token_list.begin()) {		
			if ((*it)->token == "RPARENT") {
				x++;
			}
			else if ((*it)->token == "LPARENT") {
				y++;	
			}
			if (x == y)break;
		}
		else {
			for (int j = 0; j < i; j++) {
				(*it)++;
			}
			return 0;
		}

	}

	if ((*it)->token == "LPARENT") {
		(*it)--;
		if ((*it)->token == "IDENFR") {
			(*it)++;
			for (int j = 0; j < i; j++) {
				(*it)++;
			}
			return 1;

		}
		else {
			(*it)++;
			for (int j = 0; j < i; j++) {
				(*it)++;
			}
			return 0;
		}
		

	}
	else {
		for (int j = 0; j < i; j++) {
			(*it)++;
		}
		return 0;
	}
}
int scan_hs(list<struct SYN>::iterator *it) {
	int i = 0;
	int x = 1;
	int y = 0;
	while (1) {
		
		(*it)--;
		i++;
		if ((*it) != token_list.end()) {
			if (((*it)->token == "IFTK" || (*it)->token == "ELSETK" || (*it)->token == "FORTK" || (*it)->token == "WHILETK" || (*it)->token == "DOTK") && (x == y)) {
				break;
			}
			else if ((*it)->token == "RBRACE") {
				x++;
			}
			else if ((*it)->token == "LBRACE") {
				y++;
			}

			/*if (x == y) {
				(*it)--;
				if (!((*it)->token == "IFTK" || (*it)->token == "ELSETK" || (*it)->token == "FORTK" || (*it)->token == "WHILETK" || (*it)->token == "DOTK")) {
					(*it)++;
					for (int j = 0; j < i; j++) {
						(*it)++;
					}
					return 0;
				}

			}*/
		}
		else {
			for (int j = 0; j < i; j++) {
				(*it)++;
			}
			return 0;
		}

	}
	if ((*it)->token == "IFTK"&&(x == y)) {
		for (int j = 0; j < i; j++) {
			(*it)++;
		}
		return 1;
	}
	else if ((*it)->token == "ELSETK" && (x == y)) {
		for (int j = 0; j < i; j++) {
			(*it)++;
		}
		return 2;
	}
	else if ((*it)->token == "FORTK" && (x == y)) {
		for (int j = 0; j < i; j++) {
			(*it)++;
		}
		return 3;
	}
	else if ((*it)->token == "WHILETK" && (x == y)) {
		for (int j = 0; j < i; j++) {
			(*it)++;
		}
		return 4;
	}
	else if ((*it)->token == "DOTK" && (x == y)) {
		for (int j = 0; j < i; j++) {
			(*it)++;
		}
		return 5;
	}
	else {
		for (int j = 0; j < i; j++) {
			(*it)++;
		}
		return 0;
	}
	

}

int check_V(string ch,string void_[],int void_ck[],int k) {
	for (int i = 1; i < k; i++) {
		if (ch == void_[i]) {
			
			return i;
		}
	}
	return 0;
}

int check_ck(int void_ck[],int k) {
	for (int i = 1; i < k; i++) {
		if (void_ck[i] > 0) {
			return i;
		}
	}
	return 0;
}

int scan(char* ch, char c) {
	syn = new SYN;
	if (strcmp(ch, "const") == 0) {
		//fprintf(fout, "%s", temp[1]);
		syn->token = "CONSTTK";
		syn->ch = ch;
		token_list.push_back(*syn);
		
		//printf("%s", temp[1]);
		return 1;

	}
	else if (strcmp(ch, "int") == 0) {
		//fprintf(fout, "%s", temp[2]);
		syn->token = "INTTK";
		syn->ch = ch;
		token_list.push_back(*syn);
		//printf("%s", temp[2]);
		return 1;
	}
	else if (strcmp(ch, "char") == 0) {
		//fprintf(fout, "%s", temp[3]);
		syn->token = "CHARTK";
		syn->ch = ch;
		token_list.push_back(*syn);
		//printf("%s", temp[3]);
		return 1;
	}
	else if (strcmp(ch, "void") == 0) {
		//fprintf(fout, "%s", temp[4]);
		syn->token = "VOIDTK";
		syn->ch = ch;
		token_list.push_back(*syn);
		//printf("%s", temp[4]);
		return 1;
	}
	else if (strcmp(ch, "main") == 0) {
		//fprintf(fout, "%s", temp[5]);
		syn->ch = ch;
		syn->token = "MAINTK";
		token_list.push_back(*syn);
		//printf("%s", temp[5]);
		return 1;
	}
	else if (strcmp(ch, "if") == 0) {
		//fprintf(fout, "%s", temp[6]);
		syn->token = "IFTK";
		syn->ch = ch;
		token_list.push_back(*syn);
		//printf("%s", temp[6]);
		return 1;
	}
	else if (strcmp(ch, "else") == 0) {
		//fprintf(fout, "%s", temp[7]);
		syn->token = "ELSETK";
		syn->ch = ch;
		token_list.push_back(*syn);
		//printf("%s", temp[7]);
		return 1;
	}
	else if (strcmp(ch, "do") == 0) {
		//fprintf(fout, "%s", temp[8]);
		syn->token = "DOTK";
		syn->ch = ch;
		token_list.push_back(*syn);
		//printf("%s", temp[8]);
		return 1;
	}
	else if (strcmp(ch, "while") == 0) {
		//fprintf(fout, "%s", temp[9]);
		syn->token = "WHILETK";
		syn->ch = ch;
		token_list.push_back(*syn);
		//printf("%s", temp[9]);
		return 1;
	}
	else if (strcmp(ch, "for") == 0) {
		//fprintf(fout, "%s", temp[10]);
		syn->token = "FORTK";
		syn->ch = ch;
		token_list.push_back(*syn);
		//printf("%s", temp[10]);
		return 1;
	}
	else if (strcmp(ch, "scanf") == 0) {
		//fprintf(fout, "%s", temp[11]);
		syn->token = "SCANFTK";
		syn->ch = ch;
		token_list.push_back(*syn);
		//printf("%s", temp[11]);
		return 1;
	}
	else if (strcmp(ch, "printf") == 0) {
		//fprintf(fout, "%s", temp[12]);
		syn->token = "PRINTFTK";
		syn->ch = ch;
		token_list.push_back(*syn);
		//printf("%s", temp[12]);
		return 1;
	}
	else if (strcmp(ch, "return") == 0) {
		//fprintf(fout, "%s", temp[13]);
		syn->token = "RETURNTK";
		syn->ch = ch;
		token_list.push_back(*syn);
		//printf("%s", temp[13]);
		return 1;
	}
	else if (strcmp(ch, "+") == 0) {
		//fprintf(fout, "%s", temp[14]);
		syn->token = "PLUS";
		syn->ch = ch;
		token_list.push_back(*syn);
		//printf("%s", temp[14]);
		return 1;
	}
	else if (strcmp(ch, "-") == 0) {
		//fprintf(fout, "%s", temp[15]);
		syn->token = "MINU";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, "*") == 0) {
		//fprintf(fout, "%s", temp[16]);
		syn->token = "MULT";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, "/") == 0) {
		//fprintf(fout, "%s", temp[17]);
		syn->token = "DIV";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, "<") == 0) {
		//fprintf(fout, "%s", temp[18]);
		syn->token = "LSS";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, "<=") == 0) {
		//fprintf(fout, "%s", temp[19]);
		syn->token = "LEQ";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, ">") == 0) {
		//fprintf(fout, "%s", temp[20]);
		syn->token = "GRE";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, ">=") == 0) {
		//fprintf(fout, "%s", temp[21]);
		syn->token = "GEQ";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, "==") == 0) {
		//fprintf(fout, "%s", temp[22]);
		syn->token = "EQL";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, "!=") == 0) {
		//fprintf(fout, "%s", temp[23]);
		syn->token = "NEQ";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, "=") == 0) {
		//fprintf(fout, "%s", temp[24]);
		syn->token = "ASSIGN";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, ";") == 0) {
		//fprintf(fout, "%s", temp[25]);
		syn->token = "SEMICN";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, ",") == 0) {
		//fprintf(fout, "%s", temp[26]);
		syn->token = "COMMA";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, "(") == 0) {
		//fprintf(fout, "%s", temp[27]);
		syn->token = "LPARENT";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, ")") == 0) {
		//fprintf(fout, "%s", temp[28]);
		syn->token = "RPARENT";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, "[") == 0) {
		//fprintf(fout, "%s", temp[29]);
		syn->token = "LBRACK";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, "]") == 0) {
		//fprintf(fout, "%s", temp[30]);
		syn->token = "RBRACK";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, "{") == 0) {
		//fprintf(fout, "%s", temp[31]);
		syn->token = "LBRACE";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (strcmp(ch, "}") == 0) {
		//fprintf(fout, "%s", temp[0]);
		syn->token = "RBRACE";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if ((atoi(ch)) || strcmp(ch, "0") == 0) {
		/*fprintf(fout, "INTCON %s\n", ch);
		fprintf(fout, "<无符号整数>\n");
		fprintf(fout, "<整数>\n");*/
		syn->token = "INTCON";
		syn->ch = ch;
		token_list.push_back(*syn);
		return 1;
	}
	else if (ch[0] == '\'') {
		//fprintf(fout, "CHARCON %c\n", ch[1]);
		//&&(c==';'||c==',')&&strlen(ch)==3!(atoi(ch))&&
		syn->token = "CHARCON";
		syn->ch = ch[1];
		token_list.push_back(*syn);
		return 1;
	}

	else if (ch[0] == '"') {
		char k[1000] = { 0 };
		//fprintf(fout, "STRCON ");
		for (int i = 1; i < (unsigned)strlen(ch); i++) {
			//fprintf(fout, "%c", ch[i]);
			k[i-1] = ch[i];
			k[i] = '\0';
		}
		//fprintf(fout,"\n");
		syn->token = "STRCON";
		syn->ch = k;
		token_list.push_back(*syn);
		return 1;
	}
	else {
	
		//printf("%s", ch);
		return 0;
	}
	

}