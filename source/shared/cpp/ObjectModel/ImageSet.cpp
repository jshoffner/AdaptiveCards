#include "ImageSet.h"
#include "ParseUtil.h"
#include "Image.h"

using namespace AdaptiveCards;

ImageSet::ImageSet() : 
    BaseCardElement(CardElementType::ImageSet),
    m_imageSize(ImageSize::None)
{
}

ImageSet::ImageSet(
    Spacing spacing, 
    bool separation,
    std::vector<std::shared_ptr<Image>>& images) :
    BaseCardElement(CardElementType::ImageSet, spacing, separation),
    m_images(images),
    m_imageSize(ImageSize::None)
{
}

ImageSet::ImageSet(
    Spacing spacing, 
    bool separation) :
    BaseCardElement(CardElementType::ImageSet, spacing, separation),
    m_imageSize(ImageSize::None)
{
}

ImageSize ImageSet::GetImageSize() const
{
    return m_imageSize;
}

void ImageSet::SetImageSize(const ImageSize value)
{
    m_imageSize = value;
}

const std::vector<std::shared_ptr<Image>>& ImageSet::GetImages() const
{
    return m_images;
}

std::vector<std::shared_ptr<Image>>& ImageSet::GetImages()
{
    return m_images;
}

std::string ImageSet::Serialize()
{
    Json::FastWriter writer;
    return writer.write(SerializeToJsonValue());
}

Json::Value ImageSet::SerializeToJsonValue()
{
    Json::Value root = BaseCardElement::SerializeToJsonValue();

    ImageSize imageSize = GetImageSize();
    if (imageSize != ImageSize::None)
    {
        root[AdaptiveCardSchemaKeyToString(AdaptiveCardSchemaKey::ImageSize)] = ImageSizeToString(GetImageSize());
    }

    std::string itemsPropertyName = AdaptiveCardSchemaKeyToString(AdaptiveCardSchemaKey::Images);
    root[itemsPropertyName] = Json::Value(Json::arrayValue);
    for (const auto& image : GetImages())
    {
        root[itemsPropertyName].append(image->SerializeToJsonValue());
    }

    return root;
}

std::shared_ptr<BaseCardElement> ImageSetParser::Deserialize(
    std::shared_ptr<ElementParserRegistration> elementParserRegistration,
    std::shared_ptr<ActionParserRegistration> actionParserRegistration,
    const Json::Value& value)
{
    ParseUtil::ExpectTypeString(value, CardElementType::ImageSet);

    auto imageSet = BaseCardElement::Deserialize<ImageSet>(value);

    // Get ImageSize
    imageSet->m_imageSize = ParseUtil::GetEnumValue<ImageSize>(value, AdaptiveCardSchemaKey::ImageSize, ImageSize::None, ImageSizeFromString);

    // Parse Images
    auto images = ParseUtil::GetElementCollection(elementParserRegistration, actionParserRegistration, value, AdaptiveCardSchemaKey::Images, true);

    for (auto image : images)
    {
        imageSet->m_images.push_back(std::static_pointer_cast<Image>(image));
    }

    return imageSet;
}

std::shared_ptr<BaseCardElement> ImageSetParser::DeserializeFromString(
    std::shared_ptr<ElementParserRegistration> elementParserRegistration,
    std::shared_ptr<ActionParserRegistration> actionParserRegistration,
    const std::string& jsonString)
{
    return ImageSetParser::Deserialize(elementParserRegistration, actionParserRegistration, ParseUtil::GetJsonValueFromString(jsonString));
}
