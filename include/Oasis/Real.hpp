//
// Created by Matthew McCall on 11/8/22.
//

#ifndef OASIS_REAL_HPP
#define OASIS_REAL_HPP

#include "Utility/LeafExpressionNode.hpp"

namespace oa {

    /**
     * Represents a Real number
     */
    class Real final : public LeafExpressionNode<Real> {
    public:
        Real() = default;
        Real(const Real &other);

        explicit Real(double val);

        [[nodiscard]] std::unique_ptr<oa::Expression> copy() const override;
        std::unique_ptr<oa::Expression> copyWithoutChildren() const override;

        [[nodiscard]] std::unique_ptr<Expression> evaluate() const override;

        [[nodiscard]] double getVal() const;

        bool equals(const Expression &other) const override;

        OA_EXPRESSION_TYPE(REAL)
        OA_EXPRESSION_CATEGORIES(EXPRESSION_CATEGORY_VALUE)

    private:
        double _val {};
    };

    OA_DECLARE_FACTORY(RealFactory, Real)

}// namespace oa

#endif//OASIS_REAL_HPP
