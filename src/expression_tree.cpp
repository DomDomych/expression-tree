#include "expression_tree.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <stack>
#include <stdexcept>
#include <string>

NumberNode::NumberNode(int value) : value(value) {};

BinaryOperatorNode::BinaryOperatorNode(const std::string &op, std::unique_ptr<ExprNode> left,
                                       std::unique_ptr<ExprNode> right)
    : op(op)
{
    this->left = std::move(left);
    this->right = std::move(right);
}

VariableNode::VariableNode(const std::string &name) : name(name) {};

ExpressionTree::ExpressionTree() : root(nullptr) {};

void ExpressionTree::setVariable(const std::string &name, const std::string &value)
{
    Storage[name] = value;
}

bool ExpressionTree::isOperator(const std::string &token) const
{
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

bool ExpressionTree::isNumber(const std::string &token) const
{
    if (token.empty())
        return false;

    int start = 0;

    if (token[0] == '-')
    {
        if (token.size() == 1)
            return false;

        start = 1;
    }

    for (int i = start; i < token.size(); i++)
    {
        if (token[i] < '0' || token[i] > '9')
            return false;
    }

    return true;
}

int NumberNode::evaluate(const VariableStorage &) const
{
    return value;
}

int BinaryOperatorNode::evaluate(const VariableStorage &Storage) const
{
    int leftValue = left->evaluate(Storage);
    int rightValue = right->evaluate(Storage);
    if (op == "+")
    {
        return leftValue + rightValue;
    }

    if (op == "-")
    {
        return leftValue - rightValue;
    }

    if (op == "*")
    {
        return leftValue * rightValue;
    }

    if (op == "/")
    {
        if (rightValue == 0)
        {
            throw std::logic_error("Division by zero!");
        }
        return leftValue / rightValue;
    }
    if (op == "^")
    {
        return static_cast<int>(pow(leftValue, rightValue));
    }
    throw std::logic_error("Operator " + op + " is unknown");
}

int VariableNode::evaluate(const VariableStorage &Storage) const
{
    std::string res;
    if (!Storage.get(name, res))
    {
        throw std::logic_error("Variable " + name + " is not defined");
    }

    try
    {
        return std::stoi(res);
    }
    catch (const std::invalid_argument &e)
    {
        throw std::logic_error("Variable " + name + " is not a valid integer");
    }

    catch (const std::out_of_range &e)
    {
        throw std::logic_error("Variable " + name + " is out of range");
    }
}

void ExpressionTree::evaluate(int &result) const
{
    if (root == nullptr)
    {
        throw std::logic_error("Expression Tree is empty");
    }

    result = root->evaluate(Storage);
}

int NumberNode::height() const
{
    return 1;
}

int VariableNode::height() const
{
    return 1;
}

int BinaryOperatorNode::height() const
{
    return std::max(left->height(), right->height()) + 1;
}

bool ExpressionTree::build_expression_tree(const std::vector<std::string> &tokens)
{
    root = nullptr;

    std::stack<std::unique_ptr<ExprNode>> stack;
    for (int i = 0; i < tokens.size(); i++)
    {
        if (isOperator(tokens[i]))
        {
            if (stack.size() < 2)
            {
                return false;
            }

            auto right = std::move(stack.top());
            stack.pop();

            auto left = std::move(stack.top());
            stack.pop();

            stack.push(
                std::make_unique<BinaryOperatorNode>(tokens[i], std::move(left), std::move(right)));
        }
        else if (isNumber(tokens[i]))
        {
            stack.push(std::make_unique<NumberNode>(std::stoi(tokens[i])));
        }
        else
        {
            stack.push(std::make_unique<VariableNode>(tokens[i]));
        }
    }
    if (stack.size() != 1)
    {
        return false;
    }

    root = std::move(stack.top());
    stack.pop();
    return true;
}

int ExpressionTree::height() const
{
    if (root == nullptr)
        return 0;

    return root->height();
}

int NumberNode::operatorsCount() const
{
    return 0;
}

int VariableNode::operatorsCount() const
{
    return 0;
}

int BinaryOperatorNode::operatorsCount() const
{
    return left->operatorsCount() + right->operatorsCount() + 1;
}

int ExpressionTree::operators_count() const
{
    if (root == nullptr)
        return 0;

    return root->operatorsCount();
}
std::string NumberNode::toPostfix() const
{
    return std::to_string(value);
}
std::string NumberNode::toInfix() const
{
    return std::to_string(value);
}
std::string NumberNode::toPrefix() const
{
    return std::to_string(value);
}

std::string VariableNode::toPostfix() const
{
    return name;
}
std::string VariableNode::toPrefix() const
{
    return name;
}
std::string VariableNode::toInfix() const
{
    return name;
}

std::string BinaryOperatorNode::toPostfix() const
{
    std::string result = left->toPostfix() + " " + right->toPostfix() + " " + op + " ";
    return result;
}

std::string BinaryOperatorNode::toInfix() const
{
    std::string result = "( " + left->toInfix() + " " + op + " " + right->toInfix() + " )";
    return result;
}

std::string BinaryOperatorNode::toPrefix() const
{
    std::string result = op + " " + left->toPrefix() + " " + right->toPrefix() + " ";

    return result;
}

std::string ExpressionTree::getexpr_postfix() const
{

    return root ? root->toPostfix() : "";
}

std::string ExpressionTree::getexpr_prefix() const
{
    return root ? root->toPrefix() : "";
}

std::string ExpressionTree::getexpr_infix() const
{
    return root ? root->toInfix() : "";
}

bool NumberNode::isConstant() const
{
    return true;
}

bool VariableNode::isConstant() const
{
    return false;
}

bool BinaryOperatorNode::isConstant() const
{
    return left->isConstant() && right->isConstant();
}

std::unique_ptr<ExprNode> NumberNode::simplified(const VariableStorage &) const
{
    return std::make_unique<NumberNode>(value);
}

std::unique_ptr<ExprNode> VariableNode::simplified(const VariableStorage &Storage) const
{
    return std::make_unique<VariableNode>(name);
}

std::unique_ptr<ExprNode> BinaryOperatorNode::simplified(const VariableStorage &Storage) const
{
    auto newLeft = left->simplified(Storage);
    auto newRight = right->simplified(Storage);

    auto node = std::make_unique<BinaryOperatorNode>(op, std::move(newLeft), std::move(newRight));

    if (node->isConstant())
    {
        int result = node->evaluate(Storage);
        return std::make_unique<NumberNode>(result);
    }

    return node;
}

void ExpressionTree::simplify()
{
    if (root)
    {
        root = root->simplified(Storage);
    }
}
int ExpressionTree::priority(const std::string &op) const
{
    if (op == "^")
        return 3;
    if (op == "*" || op == "/")
        return 2;
    if (op == "+" || op == "-")
        return 1;
    return 0;
}

bool ExpressionTree::infix_to_postfix(const std::vector<std::string> &tokens,
                                      std::vector<std::string> &postfix) const
{
    std::stack<std::string> operators;
    postfix.clear();

    for (const std::string &token : tokens)
    {
        if (token == "(")
        {
            operators.push(token);
        }
        else if (token == ")")
        {
            bool found = false;

            while (!operators.empty())
            {
                if (operators.top() == "(")
                {
                    found = true;
                    operators.pop();
                    break;
                }
                postfix.push_back(operators.top());
                operators.pop();
            }
            if (!found)
            {
                return false;
            }
        }
        else if (isOperator(token))
        {
            while (!operators.empty() && operators.top() != "(" &&
                   priority(operators.top()) >= priority(token))
            {
                postfix.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
        else
        {
            postfix.push_back(token);
        }
    }

    while (!operators.empty())
    {
        if (operators.top() == "(")
        {
            return false;
        }
        postfix.push_back(operators.top());
        operators.pop();
    }
    return true;
}

bool ExpressionTree::build_from_infix(const std::vector<std::string> &tokens)
{
    std::vector<std::string> postfix;

    if (!infix_to_postfix(tokens, postfix))
    {
        return false;
    }

    return build_expression_tree(postfix);
}

void ExpressionTree::clear()
{
    root = nullptr;
}