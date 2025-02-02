/************************************************************************************//*!
\file          AssetExtensions.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          September 6, 2021
\brief         This file contains utility for mapping string extensions to asset type

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once
#include <unordered_map>

#include "Engine/Asset/AssetTypes.h"

namespace engine{


	inline static std::unordered_map<std::string, AssetType> s_assetExtensionMap =
	{
		{ ".png", AssetType::Texture },
		{ ".jpg", AssetType::Texture },
		{ ".jpeg", AssetType::Texture }
	};
}
