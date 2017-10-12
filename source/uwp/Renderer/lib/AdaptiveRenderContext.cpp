#include "pch.h"

#include "AdaptiveRenderContext.h"
#include "AdaptiveHostConfig.h"
#include "AdaptiveElementRendererRegistration.h"
#include "Util.h"

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::AdaptiveCards::Uwp;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::UI::Xaml;

namespace AdaptiveCards { namespace Uwp
{
    HRESULT AdaptiveRenderContext::RuntimeClassInitialize() noexcept
    {
        return S_OK;
    }

    HRESULT AdaptiveRenderContext::RuntimeClassInitialize(
        ABI::AdaptiveCards::Uwp::IAdaptiveHostConfig* hostConfig,
        ABI::AdaptiveCards::Uwp::IAdaptiveElementRendererRegistration* elementRendererRegistration,
        ABI::AdaptiveCards::Uwp::IAdaptiveActionRendererRegistration* actionRendererRegistration) noexcept try
    {
        m_hostConfig.Attach(hostConfig);
        m_elementRendererRegistration.Attach(elementRendererRegistration);
        m_actionRendererRegistration.Attach(actionRendererRegistration);
        return S_OK;
    } CATCH_RETURN;

    _Use_decl_annotations_
    HRESULT AdaptiveRenderContext::get_HostConfig(IAdaptiveHostConfig** value)
    {
        return m_hostConfig.CopyTo(value);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveRenderContext::get_ElementRenderers(IAdaptiveElementRendererRegistration** value)
    {
        return m_elementRendererRegistration.CopyTo(value);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveRenderContext::get_ActionInvoker(IAdaptiveActionInvoker** value)
    {
        return m_actionInvoker.CopyTo(value);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveRenderContext::AddInputItem(IAdaptiveCardElement* cardElement, ABI::Windows::UI::Xaml::IUIElement* uiElement)
    {
        ComPtr<IAdaptiveCardElement> localCardElement(cardElement);
        ComPtr<IAdaptiveInputElement> inputElement;
        THROW_IF_FAILED(localCardElement.As(&inputElement));

        ComPtr<IUIElement> localUiElement(uiElement);

        InputItem item(inputElement.Get(), localUiElement.Get());

        auto inputItems = m_renderResult->GetInputItems();

        if (inputItems != nullptr)
        {
            inputItems->push_back(item);
        }
        else
        {
            // Add to Errors
        }

        return S_OK;
    }
}}
