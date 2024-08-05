#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/symbol_table.h"
#include "include/io.h"
#include <string.h>
#include <stdio.h>

int main(int argc , char* argv[]){

    char* file_content = get_file_content(argv[1]);
   
    lexer_t* lexer = init_lexer(
      file_content

    );
    symbol_table_t * symbol_table = init_symbol_table();

    parser_t* parser = init_parser(lexer);
    parser->symbol_table = symbol_table;  // Pass the symbol table to the parser

    ast_t* root = parser_parse(parser);
  
    visitor_t* visitor = init_visitor(symbol_table);
    visitor->symbol_table = symbol_table;  // Pass the symbol table to the visitor

    visitor_visit(visitor,root);
   
    return 0;
}