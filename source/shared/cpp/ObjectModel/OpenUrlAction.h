#pragma once

#include "pch.h"
#include "BaseActionElement.h"
#include "Enums.h"
#include "ActionParserRegistration.h"

namespace AdaptiveCards
{
class OpenUrlAction : public BaseActionElement
{
public:
    OpenUrlAction();

    virtual std::string Serialize();
    virtual Json::Value SerializeToJsonValue();

    std::string GetUrl() const;
    void SetUrl(const std::string value);
    
private:
    std::string m_url;
};

class OpenUrlActionParser : public IActionElementParser
{
    std::shared_ptr<BaseActionElement> Deserialize(
        std::shared_ptr<ElementParserRegistration> elementParserRegistration,
        std::shared_ptr<ActionParserRegistration> actionParserRegistration, 
        const Json::Value& value);

    std::shared_ptr<BaseActionElement> DeserializeFromString(
        std::shared_ptr<ElementParserRegistration> elementParserRegistration,
        std::shared_ptr<ActionParserRegistration> actionParserRegistration, 
        const std::string& jsonString);
};
}