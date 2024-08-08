#include <stdio.h>
#include <string.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/io.h"

#define MAX_LIMIt 20


int main(int argc, char* argv[])
{
    if (argc >= 2){
        for(int i = 1; i < argc; i++){
            int len = strlen(argv[i]);
            char* last_four = &argv[i][len-6];
            if(strcmp(last_four,".light") == 0){
                                lexer_t* lexer = init_lexer(
                    get_file_content(argv[i])
                );
                parser_t* parser = init_parser(lexer);
                ast_t* root = parser_parse(parser, parser->scope);
                visitor_t* visitor = init_visitor();
                visitor_visit(visitor, root);
            }
        }
    }
    else {
        char input[MAX_LIMIt];
        while(1){
            printf("Welcome to the hello language v. 0.0.1!\nCreated by sebbekarlsson\n>>> ");
            fgets(input,MAX_LIMIt, stdin);
            lexer_t* lexer = init_lexer(input);
            parser_t* parser = init_parser(lexer);
            ast_t* root = parser_parse(parser, parser->scope);
            visitor_t* visitor = init_visitor();
            visitor_visit(visitor, root);
        }
    }
    return 0;
}