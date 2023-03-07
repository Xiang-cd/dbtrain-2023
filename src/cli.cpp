#include <signal.h>

#include <iostream>
#include <memory>
#include <string>

#include "exception/exceptions.h"
#include "parser/visitor.h"
#include "result/printers.h"
#include "system/system_manager.h"

using namespace dbtrain;

void sigint_handler(int signo) {
  SystemManager::GetInstance().CloseDatabase();
  std::cout << "Bye" << std::endl;
  exit(0);
}

int yyparse();

void cli(std::unique_ptr<Printer> printer) {
  ast::Visitor *visitor = new ast::Visitor();
  try {
    while (true) {
      std::cerr << "dbtrain> ";
      if (yyparse() == 0) {
        if (ast::parse_tree != nullptr) {
          try {
            Result result = std::any_cast<Result>(ast::parse_tree->accept(visitor));
            printer->Print(&result);
          } catch (DbError &e) {
            Result result({"FAILURE"});
            printer->Print(&result);
            std::cerr << e.what() << std::endl;
          }
        } else {
          break;
        }
      }
    }
    delete visitor;
    SystemManager::GetInstance().CloseDatabase();
    std::cout << "Bye" << std::endl;
  } catch (DbError &e) {
    std::cerr << e.what() << std::endl;
    delete visitor;
    SystemManager::GetInstance().CloseDatabase();
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  signal(SIGINT, sigint_handler);
  std::unique_ptr<Printer> printer;
  if (argc >= 2 && strcmp(argv[1], "-s") == 0) {
    printer = std::make_unique<RawPrinter>();
  } else {
    printer = std::make_unique<TablePrinter>();
  }
  cli(std::move(printer));
  return 0;
}
