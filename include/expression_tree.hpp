#pragma once

#include "avl_map.hpp"
#include <string>
#include <vector>
#include <memory>


using VariableStorage = AVLTree<std::string,std::string>;

class ExprNode
{
public:
    virtual ~ExprNode() = default;

    virtual int evaluate(const VariableStorage&) const = 0;

    virtual std::string toInfix() const = 0;
    virtual std::string toPrefix() const = 0;
    virtual std::string toPostfix() const = 0;

    virtual int height() const = 0;
    virtual int operatorsCount() const = 0;

    virtual bool isConstant() const = 0;

    virtual std::unique_ptr <ExprNode> simplified(const VariableStorage&) const = 0;
};

class NumberNode : public ExprNode
{
private:
    int value;

public:
    explicit NumberNode(int value);
    int evaluate(const VariableStorage&) const override;

    std::string toInfix() const override;
    std::string toPrefix() const override;
    std::string toPostfix() const override;

    int height() const override;
    int operatorsCount() const override;
    bool isConstant()const override;

    std::unique_ptr <ExprNode> simplified(const VariableStorage&) const override;
};

class BinaryOperatorNode : public ExprNode
{
private:
    std::string op;
    std::unique_ptr <ExprNode> left;
    std::unique_ptr <ExprNode> right;

public:
    BinaryOperatorNode(const std::string&,
                       std::unique_ptr <ExprNode> left,
                       std::unique_ptr <ExprNode> right);

    int evaluate(const VariableStorage&) const override;

    std::string toInfix() const override;
    std::string toPrefix() const override;
    std::string toPostfix() const override;

    int height() const override;
    int operatorsCount() const override;
    bool isConstant() const override;

    std::unique_ptr <ExprNode> simplified(const VariableStorage&)const override;
};

class VariableNode : public ExprNode
{
private:
    std::string name;
public:
    explicit VariableNode(const std::string& name);

    int evaluate(const VariableStorage&) const override;

    std::string toInfix() const override;
    std::string toPrefix() const override;
    std::string toPostfix() const override;

    int height() const override;
    int operatorsCount() const override;
    bool isConstant() const override;

    std::unique_ptr <ExprNode> simplified(const VariableStorage&)const override;
};  

class ExpressionTree
{
private:
    std::unique_ptr <ExprNode> root;

    bool isOperator(const std::string &token) const;
    bool isNumber(const std::string &s) const;  
    bool infix_to_postfix(const std::vector<std::string> &tokens, std::vector<std::string> &postfix) const;
    int priority(const std::string& op)const;
    VariableStorage Storage ;

public:
    ExpressionTree();
    ~ExpressionTree()=default;
    ExpressionTree(const ExpressionTree &) = delete;
    ExpressionTree &operator=(const ExpressionTree &) = delete;
    std::string getexpr_postfix() const;
    std::string getexpr_prefix() const;
    std::string getexpr_infix() const;
    bool build_expression_tree(const std::vector<std::string> &v);
    void evaluate(int &result) const;
    bool build_from_infix(const std::vector<std::string> &tokens);
    int operators_count() const;
    int height() const;
    void simplify();
    void setVariable(const std::string &name, const std::string &value);
    void clear();
};