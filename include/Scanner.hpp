#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <unordered_map>
#include <vector>

enum class TokenType{
   // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN,TRUE, LET, WHILE,

  END
};

struct Token{
  std::string value;
  TokenType type;
  Token(TokenType type,const std::string& value) : type(type), value(value) {}
};

class Scanner{
  public:
    int line;
    int start;
    int current;
    std::vector<Token> tokens;
    std::unordered_map<std::string, TokenType> keywords = {
      {"let",TokenType::LET},
      {"if",TokenType::IF},
      {"else",TokenType::ELSE},
      {"while",TokenType::WHILE},
      {"for",TokenType::FOR},
      {"or",TokenType::OR},
      {"and",TokenType::AND},
      {"true",TokenType::TRUE},
      {"false",TokenType::FALSE},
      {"fun",TokenType::FUN},
      {"nil",TokenType::NIL},
      {"print",TokenType::PRINT}
    };
    std::string source;

    Scanner(std::string src);
    void scanToken();
    void scanTokens();
    void printToken(const Token& token);

  private:
    char advance();
    char peek();
    bool atEnd();
    void number();
    void identifier();
    bool match(const char& expected);
    void string();
    char peekNext();

};



#endif
