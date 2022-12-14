//
// Created by Matthew McCall on 11/8/22.
//

#ifndef OASIS_BINARYEXPRESSIONNODE_HPP
#define OASIS_BINARYEXPRESSIONNODE_HPP

#include <cassert>

#include "Oasis/Expression.hpp"

#include "Oasis/Blank.hpp"
#include "Types.hpp"

namespace oa {

    struct BinaryEvaluateReturnType {
        std::unique_ptr<Expression> leftResult;
        std::unique_ptr<Expression> rightResult;
    };

    /**
     * Helper class for Expressions that have two children/operands
     */
    template<typename T>
    class BinaryExpressionNode : public Expression {
    public:
        BinaryExpressionNode();
        BinaryExpressionNode(std::unique_ptr<Expression> &&left, std::unique_ptr<Expression> &&right);

        [[nodiscard]] std::unique_ptr<oa::Expression> copy() const final;
        [[nodiscard]] std::unique_ptr<oa::Expression> copyWithoutChildren() const final;

        bool addChild(std::unique_ptr<Expression> &&expr) final;
        void forEachChild(std::function<void(const std::unique_ptr<Expression> &)> func) const final;
        void recurseForEachChild(std::function<void(const Expression &)> func) const override;

        bool equals(const Expression &other) const final;

        [[nodiscard]] const std::unique_ptr<Expression> &getLeft() const;
        [[nodiscard]] const std::unique_ptr<Expression> &getRight() const;

    protected:
        [[nodiscard]] BinaryEvaluateReturnType evaluateOperands() const;

        std::unique_ptr<Expression> _left, _right;
    };

    template<typename T>
    bool BinaryExpressionNode<T>::addChild(std::unique_ptr<Expression> &&expr) {

        if (_left->getType() == Expression::Type::BLANK || !_left) {
            _left = std::move(expr);
            return true;
        }

        if (_right->getType() == Expression::Type::BLANK || !_right) {
            _right = std::move(expr);
            return true;
        }

        return false;
    }

    template<typename T>
    BinaryEvaluateReturnType BinaryExpressionNode<T>::evaluateOperands() const {
        auto leftResult = _left->evaluate();
        auto rightResult = _right->evaluate();

        return { std::move(leftResult), std::move(rightResult) };
    }

    template<typename T>
    BinaryExpressionNode<T>::BinaryExpressionNode() : _left(BlankFactory()), _right(BlankFactory()) { }

    template<typename T>
    BinaryExpressionNode<T>::BinaryExpressionNode(std::unique_ptr<Expression> &&left, std::unique_ptr<Expression> &&right) : _left(std::move(left)), _right(std::move(right)) { }

    template<typename T>
    void BinaryExpressionNode<T>::forEachChild(std::function<void(const std::unique_ptr<Expression> &)> func) const {
        func(_left);
        func(_right);
    }

    template<typename T>
    void BinaryExpressionNode<T>::recurseForEachChild(std::function<void(const Expression &)> func) const {
        _left->recurseForEachChild(func);
        _right->recurseForEachChild(func);

        func(*this);
    }

    template<typename T>
    bool BinaryExpressionNode<T>::equals(const Expression &other) const {

        auto result = evaluate();
        auto otherResult = evaluate();

        if (
                (result->getCategories() & EXPRESSION_CATEGORY_BINARY_OPERANDS) &&
                (otherResult->getCategories() & EXPRESSION_CATEGORY_BINARY_OPERANDS)) {
            if (other.getType() != this->getType()) {
                return false;
            }

            const auto &binaryResult = dynamic_cast<const BinaryExpressionNode &>(*result);
            const auto &binaryOther = dynamic_cast<const BinaryExpressionNode &>(*otherResult);

            auto [leftResult, rightResult] = binaryResult.evaluateOperands();
            auto [otherLeftResult, otherRightResult] = binaryOther.evaluateOperands();

            return (*leftResult == *otherLeftResult) && (*rightResult == *otherRightResult);
        }

        return *result == *otherResult;
    }

    template<typename T>
    const std::unique_ptr<Expression> &BinaryExpressionNode<T>::getLeft() const {
        return _left;
    }

    template<typename T>
    const std::unique_ptr<Expression> &BinaryExpressionNode<T>::getRight() const {
        return _right;
    }

    template<typename T>
    std::unique_ptr<oa::Expression> BinaryExpressionNode<T>::copy() const {
        return std::make_unique<T>(_left->copy(), _right->copy());
    }
    template<typename T>
    std::unique_ptr<oa::Expression> BinaryExpressionNode<T>::copyWithoutChildren() const {
        return std::make_unique<T>();
    }

}// namespace oa

#endif//OASIS_BINARYEXPRESSIONNODE_HPP
