//==============================================================================
//
//  OvenMediaEngine
//
//  Created by Hyunjun Jang
//  Copyright (c) 2020 AirenSoft. All rights reserved.
//
//==============================================================================
#pragma once

#include <base/mediarouter/media_type.h>

#include "publisher.h"

namespace cfg
{
	namespace vhost
	{
		namespace app
		{
			namespace pub
			{
				// <Compatibility><Required><MediaType>Video</MediaType>...</Required></Compatibility>
				// Names the media types whose codecs go into the OVT2 `ovt.required` list.
				// Structured children rather than one text field,
				// so that a typo fails startup instead of silently lifting the policy.
				struct OvtRequired : public Item
				{
				protected:
					std::vector<ov::String> _media_type_list = {"Video", "Audio"};

				public:
					CFG_DECLARE_CONST_REF_GETTER_OF(GetMediaTypeList, _media_type_list)

				protected:
					void MakeList() override
					{
						Register<Optional>("MediaType", &_media_type_list, nullptr,
										   [=]() -> std::shared_ptr<ConfigError> {
											   for (const auto &name : _media_type_list)
											   {
												   auto media_type = ::cmn::GetMediaTypeByName(name);
												   if ((media_type.has_value() == false) || (*media_type == ::cmn::MediaType::Unknown))
												   {
													   return CreateConfigErrorPtr("Unknown media type in <Compatibility><Required>: %s", name.CStr());
												   }
											   }
											   return nullptr;
										   });
					}
				};

				struct OvtCompatibility : public Item
				{
				protected:
					OvtRequired _required;

				public:
					CFG_DECLARE_CONST_REF_GETTER_OF(GetRequired, _required)

				protected:
					void MakeList() override
					{
						Register<Optional>("Required", &_required);
					}
				};

				struct OvtPublisher : public Publisher
				{
					PublisherType GetType() const override
					{
						return PublisherType::Ovt;
					}

					CFG_DECLARE_CONST_REF_GETTER_OF(GetCompatibility, _compatibility)

				protected:
					void MakeList() override
					{
						Publisher::MakeList();

						Register<Optional>("Compatibility", &_compatibility);
					}

					OvtCompatibility _compatibility;
				};
			}  // namespace pub
		}  // namespace app
	}  // namespace vhost
}  // namespace cfg