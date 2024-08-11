#include <stdio.h>
#include <string.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/io.h"


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
    
    return 0;
}
