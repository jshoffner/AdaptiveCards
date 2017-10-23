#pragma once

#include "pch.h"
#include "Enums.h"
#include "BaseCardElement.h"
#include "Column.h"
#include "ElementParserRegistration.h"

namespace AdaptiveCards
{
class ColumnSet : public BaseCardElement
{
friend class ColumnSetParser;
public:
    ColumnSet();
    ColumnSet(std::vector<std::shared_ptr<Column>>& columns);

    virtual std::string Serialize();
    Json::Value SerializeToJsonValue();

    std::vector<std::shared_ptr<Column>>& GetColumns();
    const std::vector<std::shared_ptr<Column>>& GetColumns() const;

    std::shared_ptr<BaseActionElement> GetSelectAction() const;
    void SetSelectAction(const std::shared_ptr<BaseActionElement> action);

private:
    static const std::unordered_map<CardElementType, std::function<std::shared_ptr<Column>(const Json::Value&)>, EnumHash> ColumnParser;
    std::vector<std::shared_ptr<Column>> m_columns;
    std::shared_ptr<BaseActionElement> m_selectAction;
};

class ColumnSetParser : public IBaseCardElementParser
{
public:
    std::shared_ptr<BaseCardElement> Deserialize(
        std::shared_ptr<ElementParserRegistration> elementParserRegistration,
        std::shared_ptr<ActionParserRegistration> actionParserRegistration, 
        const Json::Value& root);

    std::shared_ptr<BaseCardElement> DeserializeFromString(
        std::shared_ptr<ElementParserRegistration> elementParserRegistration,
        std::shared_ptr<ActionParserRegistration> actionParserRegistration, 
        const std::string& jsonString);
};
}