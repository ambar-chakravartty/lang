#include <cctype>
#include <iostream>
#include <stdexcept>
#include "./include/Scanner.hpp"

Scanner::Scanner(std::string src) : source(std::move(src)), current(0), start(0), line(1) {}

char Scanner::peek() {
    return atEnd() ? '\0' : source[current];
}

char Scanner::advance() {
    if (atEnd()) throw std::runtime_error("Unexpected end of input");
    return source[current++];
}

bool Scanner::atEnd() {
    return current >= source.size();
}

void Scanner::number() {
    std::string result;
    while (std::isdigit(peek())) advance();

    if (peek() == '.' && std::isdigit(peekNext())) {
        advance();
        while (std::isdigit(peek())) advance();
    }

    result = source.substr(start, current - start);
    tokens.push_back(Token(TokenType::NUMBER, result));
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

bool Scanner::match(const char& expected) {
    if (atEnd() || source[current] != expected) return false;
    current++;
    return true;
}

void Scanner::string() {
    while (peek() != '"' && !atEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (atEnd()) throw std::runtime_error("Unterminated string literal");

    // Closing quote
    advance();

    std::string value = source.substr(start + 1, current - start - 2);
    tokens.push_back(Token(TokenType::STRING, value));
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '+': tokens.push_back(Token(TokenType::PLUS, "+")); break;
        case '-': tokens.push_back(Token(TokenType::MINUS, "-")); break;
        case '/': tokens.push_back(Token(TokenType::SLASH, "/")); break;
        case '*': tokens.push_back(Token(TokenType::STAR, "*")); break;
        case '(': tokens.push_back(Token(TokenType::LEFT_PAREN, "(")); break;
        case ')': tokens.push_back(Token(TokenType::RIGHT_PAREN, ")")); break;
        case '{': tokens.push_back(Token(TokenType::LEFT_BRACE, "{")); break;
        case '}': tokens.push_back(Token(TokenType::RIGHT_BRACE, "}")); break;
        case ';': tokens.push_back(Token(TokenType::SEMICOLON, ";")); break;
        case '>': tokens.push_back(match('=') ? Token(TokenType::GREATER_EQUAL, ">=") : Token(TokenType::GREATER, ">")); break;
        case '<': tokens.push_back(match('=') ? Token(TokenType::LESS_EQUAL, "<=") : Token(TokenType::LESS, "<")); break;
        case '!': tokens.push_back(match('=') ? Token(TokenType::BANG_EQUAL, "!=") : Token(TokenType::BANG, "!")); break;
        case '=': tokens.push_back(match('=') ? Token(TokenType::EQUAL_EQUAL, "==") : Token(TokenType::EQUAL, "=")); break;
        case ' ': case '\r': case '\t': case '\n': break; // Ignore whitespace
        case '"': string(); break;
        case ',': tokens.push_back(Token(TokenType::COMMA, ",")); break;
        default:
            if (std::isdigit(c)) {
                number();
            } else if (std::isalpha(c)) {
                identifier();
            } else {
                std::cerr << "Unexpected character\n" << c << "\n";
            }
    }
}

void Scanner::scanTokens() {
    while (!atEnd()) {
        start = current;
        scanToken();
    }

    tokens.push_back(Token(TokenType::END, ""));
}

char Scanner::peekNext() {
    return (current + 1 >= source.size()) ? '\0' : source[current + 1];
}
