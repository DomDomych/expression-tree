#include "expression_tree.hpp"
#include <iostream>
#include <string>
#include <vector>

void printTreeInfo(ExpressionTree &tree) {

  if (tree.getexpr_infix() == "") {
    std::cout << "Дерево Пустое" << std::endl;
    return;
  }
  std::cout << "Infix:   " << tree.getexpr_infix() << std::endl;
  std::cout << "Prefix:  " << tree.getexpr_prefix() << std::endl;
  std::cout << "Postfix: " << tree.getexpr_postfix() << std::endl;

  std::cout << "Высота: " << tree.height() << std::endl;
  std::cout << "Счетчик Операторов: " << tree.operators_count() << std::endl;

  int result;

  tree.evaluate(result);
  std::cout << "Результат: " << result << std::endl;
}

int main() {
  std::cout << "=== Пример 1: Строим Из Постфикса ===" << std::endl;

  ExpressionTree tree1;

  std::vector<std::string> postfix = {"2", "3", "+", "4", "5", "*", "+"};

  if (!tree1.build_expression_tree(postfix)) {
    std::cout << "Не Удалось Построить" << std::endl;
    return 1;
  }

  printTreeInfo(tree1);

  std::cout << std::endl;

  std::cout << "=== Пример 2: Строим Из Инфикса ===" << std::endl;

  ExpressionTree tree2;

  std::vector<std::string> infix = {"(", "2", "+", "3", ")", "*", "4"};

  if (!tree2.build_from_infix(infix)) {
    std::cout << "Не Удалось Построить" << std::endl;
    return 1;
  }

  printTreeInfo(tree2);

  std::cout << std::endl;

  std::cout << "=== Пример 3: Переменные  ===" << std::endl;

  ExpressionTree tree3;

  std::vector<std::string> variables = {"(", "a", "+", "b", ")", "*", "2"};

  if (!tree3.build_from_infix(variables)) {
    std::cout << "Не Удалось Построить" << std::endl;
    return 1;
  }
  // Задаем Значения Переменных
  tree3.setVariable("a", "3");
  tree3.setVariable("b", "5");

  printTreeInfo(tree3);

  std::cout << std::endl;

  std::cout << "=== Пример 4: Оператор ^ ===" << std::endl;

  ExpressionTree tree4;

  std::vector<std::string> power = {"2", "^", "3"};

  if (!tree4.build_from_infix(power)) {
    std::cout << "Не Удалось Построить" << std::endl;
    return 1;
  }

  printTreeInfo(tree4);

  std::cout << std::endl;

  std::cout << "=== Пример 5:  Упрощение ===" << std::endl;

  ExpressionTree tree5;

  std::vector<std::string> expr1 = {"(", "2", "+", "3", ")", "*", "x"};

  if (!tree5.build_from_infix(expr1)) {
    std::cout << "Не Удалось Построить" << std::endl;
    return 1;
  }

  std::cout << "Перед Упрощением:" << std::endl;
  std::cout << "Инфикс:   " << tree5.getexpr_infix() << std::endl;
  std::cout << "Постфикс: " << tree5.getexpr_postfix() << std::endl;

  tree5.simplify();

  std::cout << "После Упрощения:" << std::endl;
  std::cout << "Инфикс:   " << tree5.getexpr_infix() << std::endl;
  std::cout << "Постфикс: " << tree5.getexpr_postfix() << std::endl;

  tree5.setVariable("x", "10");

  int simplified;

  tree5.evaluate(simplified);
  std::cout << "Result with x = 10: " << simplified << std::endl;

  std::cout << std::endl;

  std::cout << "=== Пример 6: Унарный Минус ===" << std::endl;

  ExpressionTree tree6;

  std::vector<std::string> expr2 = {"-5", "+", "2"};

  if (!tree6.build_from_infix(expr2)) {
    std::cout << "Не Удалось Построить" << std::endl;
    return 1;
  }

  printTreeInfo(tree6);

  std::cout << "=== Пример 7: Очистка Дерева ===" << std::endl;

  ExpressionTree tree7;

  std::vector<std::string> expr3 = {"5", "2", "+", "10", "*"};

  if (tree7.build_expression_tree(expr3)) {
    printTreeInfo(tree7);
  }

  std::cout << "Очистим Дерево и Посмотрим На Результат\n";
  tree7.clear();

  printTreeInfo(tree7);

  std::cout << "=== Пример 8: Некорректное Использование Дерева ==="
            << std::endl;

  ExpressionTree tree8;

  std::vector<std::string> expr4 = {"5", "0", "/"};

  if (tree8.build_expression_tree(expr4)) {
    std::cout << "Дерево Удалось Построить!" << std::endl;
    std::cout << "Однако Выражение Не Является Верным Математически"
              << std::endl;
    try {
      int res;
      tree8.evaluate(res);
    } catch (const std::exception &e) {
      std::cout << "Мы Поймали Ошибку: " << e.what() << std::endl;
    }
  }

  return 0;
}