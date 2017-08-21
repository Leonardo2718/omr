/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017, 2017
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 ******************************************************************************/

#ifndef METHOD_INFO_HPP
#define METHOD_INFO_HPP

#include "il/DataTypes.hpp"
#include "type_info.hpp"
#include "ilgen.hpp"

#include <vector>
#include <string>
#include <stdexcept>

namespace Tril {

/**
 * @brief Class for extracting infromation about a Tril method from its AST node
 */
class MethodInfo {
    public:

        /**
         * @brief Constructs a MethodInfo object from a Tril AST node
         * @param methodNode is the Tril AST node
         */
        explicit MethodInfo(const ASTNode* methodNode) : _methodNode{methodNode} {
            auto returnTypeArg = _methodNode->getArgByName("return");
            _returnType = Tril::TypeInfo::getTRDataTypes(returnTypeArg->getValue()->getString());

            auto argTypesArg = _methodNode->getArgByName("args");
            if (argTypesArg != nullptr) {
                auto typeValue = argTypesArg->getValue();
                while (typeValue != nullptr) {
                    _argTypes.push_back(Tril::TypeInfo::getTRDataTypes(typeValue->getString()));
                    typeValue = typeValue->next;
                }
            }

            auto nameArg = _methodNode->getArgByName("name");
            if (nameArg != nullptr) {
                _name = nameArg->getValue()->get<ASTValue::String_t>();
            }
        }

        /**
         * @brief Returns the name of the Tril method
         */
        const std::string& getName() const { return _name; }

        /**
         * @brief Retruns the AST node representing the Tril method
         */
        const ASTNode* getASTNode() const { return _methodNode; }

        /**
         * @brief Returns the AST representation of the method's body
         */
        const ASTNode* getBodyAST() const { return _methodNode->getChildren(); }

        /**
         * @brief Returns the return type of the method
         */
        TR::DataTypes getReturnType() const { return _returnType; }

        /**
         * @brief Returns a vector with the argument types of the method
         */
        const std::vector<TR::DataTypes>& getArgTypes() const { return _argTypes; }

        /**
         * @brief Returns the number of arguments the Tril method takes
         */
        std::size_t getArgCount() const { return _argTypes.size(); }

    private:
        const ASTNode* _methodNode;
        std::string _name;
        TR::DataTypes _returnType;
        std::vector<TR::DataTypes> _argTypes;
};

} // namespace Tril

#endif // METHOD_INFO_HPP
