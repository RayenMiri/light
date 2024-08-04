#include <stdio.h>
#include "include/lexer.h"
#include "include/parser.h"

int main(int argc , char* argv[]){
    lexer_t* lexer = init_lexer(
        "var name = \"Hello World\";\n"
        "var test = \"test\";\n"
        "var here = \"here\";\n"
        "print(name);\n"
    );
    
   
    parser_t* parser = init_parser(lexer);
 
    ast_t* root = parser_parse(parser);
  
    printf("%d\n",root->type);
    printf("size %d\n",root->compound_size);
    
    return 0;
}

/*main function to lex the input 
int main(int argc , char* argv[]){
    lexer_t* lexer = init_lexer(
        "var a = \"Hello World\";\n"
        "print(a)\n"
    );
    token_t * token = (void*)0;
    while((token = lexer_get_next_token(lexer)) != (void*)0){
        printf("TOKEN( %d, %s )\n",token->type,token->value);
    }
    return 0;
}*/