#include <cctype>
#include <iostream>
#include "Scanner.hpp"



Scanner::Scanner(std::string src){
    this->source = src;
    this->current = 0;
    this->start = 0;
    this->line = 1;
}

char Scanner::peek(){
    return source[current];
}

char Scanner::advance(){
    int index = current;
    current += 1;
    return source[index];
}

bool Scanner::atEnd(){
    return current >= source.size();
}

void Scanner::number(){
    std::string result;

    while(std::isdigit(peek())) advance();

    if(peek() == '.' && std::isdigit(source[current+1])){
        advance();


        while(std::isdigit(peek())) advance();
    }

    result = source.substr(start,current-start);

    tokens.push_back(Token(TokenType::NUMBER,result));

}

void Scanner::identifier() {
    while (std::isalnum(peek())) advance();
    std::string text = source.substr(start, current - start);

    TokenType type = TokenType::IDENTIFIER;
    if (keywords.find(text) != keywords.end()) {
        type = keywords.at(text);
    }

    tokens.push_back(Token(type, text));
}

bool Scanner::match(const char& expected){
    if(atEnd()) return false;

    if(source[current] != expected) return false;

    current++;

    return true;
}

void Scanner::string(){
    while(peek() != '"' && !atEnd()){
        if (peek() == '\n') line++;
        advance();
    }

    advance();

    tokens.push_back(Token(TokenType::STRING,source.substr(start,current-start)));
}

void Scanner::scanToken(){

    char c = advance();

    switch(c){
        case '+': tokens.push_back(Token(TokenType::PLUS,"+")); break;
        case '-': tokens.push_back(Token(TokenType::MINUS,"-")); break;
        case '/': tokens.push_back( Token(TokenType::SLASH,"/")); break;
        case '*': tokens.push_back(Token(TokenType::STAR,"*")); break;
        case '(': tokens.push_back(Token(TokenType::LEFT_PAREN,"(")); break;
        case ')': tokens.push_back(Token(TokenType::RIGHT_PAREN,")")); break;
        case '{': tokens.push_back(Token(TokenType::LEFT_BRACE,"{")); break;
        case '}': tokens.push_back(Token(TokenType::RIGHT_BRACE,"}")); break;
        case ';': tokens.push_back(Token(TokenType::SEMICOLON,";")); break;
        
        case '"': string(); break;

        case '>':
            match('=') ? tokens.push_back(Token(TokenType::GREATER_EQUAL,">=")) : tokens.push_back(Token(TokenType::GREATER,">"));
            break;
        case '<':
            match('=') ? tokens.push_back(Token(TokenType::LESS_EQUAL,"<=")) : tokens.push_back(Token(TokenType::LESS,"<"));
            break;
        case '=':
            match('=') ? tokens.push_back(Token(TokenType::EQUAL_EQUAL,"==")) : tokens.push_back(Token(TokenType::EQUAL,"="));
            break;

        
        default:
            if(std::isdigit(c)){
                number();
            }else if(std::isalpha(c)){
                identifier();
            }
    }
}

void Scanner::scanTokens(){

    while(!atEnd()){
        start = current;
        scanToken();

    }

    tokens.push_back(Token(TokenType::END,""));

}

void Scanner::printToken(const Token& token){
    switch (token.type) {
        case TokenType::PLUS: std::cout << "+\n"; break;
        case TokenType::MINUS: std::cout << "-\n"; break;
        case TokenType::SLASH: std::cout << "/\n"; break;
        case TokenType::STAR: std::cout << "*\n"; break;
        case TokenType::LEFT_PAREN: std::cout << "(\n"; break;
        case TokenType::RIGHT_PAREN: std::cout << ")\n"; break;
        case TokenType::LEFT_BRACE: std::cout << "{\n"; break;
        case TokenType::RIGHT_BRACE: std::cout << "}\n"; break;
        case TokenType::SEMICOLON: std::cout << ";\n"; break;
        case TokenType::END: std::cout<< "EOF\n"; break;
        case TokenType::NUMBER: std::cout << "NUMBER(" << token.value << ")\n"; break;
        case TokenType::IDENTIFIER: std::cout << "IDENTIFIER(" << token.value << ")\n"; break;
        case TokenType::FOR: std::cout << "KEYWORD(" << token.value <<")\n"; break;
        case TokenType::IF: std::cout << "KEYWORD(" << token.value <<")\n"; break;
        case TokenType::VAR: std::cout << "KEYWORD(" << token.value <<")\n"; break;
        case TokenType::AND: std::cout << "KEYWORD(" << token.value <<")\n"; break;
        case TokenType::ELSE: std::cout << "KEYWORD(" << token.value <<")\n"; break;
        case TokenType::FUN: std::cout << "KEYWORD(" << token.value <<")\n"; break;
        case TokenType::PRINT: std::cout << "KEYWORD(" << token.value <<")\n"; break;

    }
}

