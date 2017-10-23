#include "pch.h"
#include "AdaptiveActionParserRegistration.h"
#include "CustomActionWrapper.h"
#include "Util.h"

using namespace Microsoft::WRL;
using namespace ABI::AdaptiveCards::Uwp;
using namespace ABI::Windows::UI;

namespace AdaptiveCards { namespace Uwp
{
    AdaptiveActionParserRegistration::AdaptiveActionParserRegistration()
    {
    }

    HRESULT AdaptiveActionParserRegistration::RuntimeClassInitialize() noexcept try
    {
        m_sharedParserRegistration = std::make_shared<ActionParserRegistration>();
        m_registration = std::make_shared<RegistrationMap>();

        return S_OK;
    } CATCH_RETURN;

    _Use_decl_annotations_
    HRESULT AdaptiveActionParserRegistration::Set(HSTRING type, IAdaptiveActionParser* Parser)
    {
        std::string typeString = HStringToUTF8(type);

        ComPtr<IAdaptiveActionParser> localParser(Parser);
        (*m_registration)[typeString] = localParser;

        m_sharedParserRegistration->AddParser(typeString, std::make_shared<SharedModelActionParser>(this));

        return S_OK;
    }

    _Use_decl_annotations_
    HRESULT AdaptiveActionParserRegistration::Get(HSTRING type, IAdaptiveActionParser** result)
    {
        *result = nullptr;

        RegistrationMap::iterator found = m_registration->find(HStringToUTF8(type));
        if (found != m_registration->end())
        {
            *result = found->second.Get();
        }
        return S_OK;
    }

    _Use_decl_annotations_
    HRESULT AdaptiveActionParserRegistration::Remove(_In_ HSTRING type)
    {
        std::string typeString = HStringToUTF8(type);

        m_registration->erase(typeString);
        m_sharedParserRegistration->RemoveParser(typeString);
        return S_OK;
    }

    std::shared_ptr<ActionParserRegistration> AdaptiveActionParserRegistration::GetSharedParserRegistration()
    {
        return m_sharedParserRegistration;
    }

    std::shared_ptr<BaseActionElement> SharedModelActionParser::Deserialize(
        std::shared_ptr<AdaptiveCards::ElementParserRegistration>,
        std::shared_ptr<AdaptiveCards::ActionParserRegistration>,
        const Json::Value& value)
    {
        std::string type = ParseUtil::GetTypeAsString(value);

        HSTRING typeAsHstring;
        THROW_IF_FAILED(UTF8ToHString(type, &typeAsHstring));

        ComPtr<IAdaptiveActionParser> parser;
        THROW_IF_FAILED(m_parserRegistration->Get(typeAsHstring, &parser));

        ComPtr<ABI::Windows::Data::Json::IJsonObject>jsonObject;
        THROW_IF_FAILED(JsonCppToJsonObject(value, &jsonObject));

        ComPtr<IAdaptiveActionElement> actionElement;
        THROW_IF_FAILED(parser->FromJson(jsonObject.Get(), &actionElement));

        std::shared_ptr<CustomActionWrapper> actionWrapper = std::make_shared<CustomActionWrapper>(actionElement.Get());
        return actionWrapper;
    }
}}
