#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace lightex {

class Workspace {
 public:
  virtual ~Workspace() {}

  virtual bool LoadStyle(const std::string& style_file_data, std::string* error_message) = 0;
  virtual bool ParseProgram(const std::string& input, std::string* error_message, std::string* output) = 0;
};

std::shared_ptr<Workspace> MakeDotWorkspace();
std::shared_ptr<Workspace> MakeHtmlWorkspace();

namespace ast {
  struct Program;
}
bool ParseProgramToAst(const std::string& input, std::string* error_message, ast::Program* output);

}  // namespace lightex
