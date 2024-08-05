#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/io.h"
#include <string.h>
#include <stdio.h>

int main(int argc , char* argv[]){

    char* file_content = get_file_content(argv[1]);
   
    lexer_t* lexer = init_lexer(
      file_content

    );
    
    parser_t* parser = init_parser(lexer);
 
    ast_t* root = parser_parse(parser);

    visitor_t* visitor = init_visitor();
    
    visitor_visit(visitor,root);
   
    return 0;
}