//  KwasarEngine is an SDK that can help you speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 22.10.24.
//

#include "KwasarEngine/resources/textures/TextureLoader.h"

#include "KwasarEngine/resources/textures/Texture2DImageFormat.h"
#include "KwasarEngine/resources/textures/Texture2DType.h"
#include "KwasarEngine/resources/textures/TextureResource.h"
#include "KwasarEngine/utils/ReportMessage.h"

namespace ke {
/*template<typename PixelType>
std::vector<PixelType> get_data(const uint32_t pWidth, const uint32_t pHeight, std::istream &pStream) {
	std::vector<PixelType> pixelsOut;
	pixelsOut.resize(pWidth * pHeight);
	png::image<PixelType> img(pStream);
	auto &pixelBuffer = img.get_pixbuf();
	for (uint32_t x = 0; x < pHeight; ++x) {
		memcpy(&pixelsOut[static_cast<size_t>(x) * pWidth], pixelBuffer.get_row(pHeight - x - 1).data(),
			   static_cast<uint32_t>(pWidth * sizeof(PixelType)));
	}
	return pixelsOut;
}*/

std::shared_ptr<IResource> TextureLoader::createResource() { return TextureResource::create(); }

ReportMessagePtr TextureLoader::load(IResourceLoadExecutor* /*pLoadExecutor*/,
									 std::shared_ptr<std::istream> & /*pStream*/,
									 const std::shared_ptr<IResource> & /*pResource*/) {
	/*auto resource = std::dynamic_pointer_cast<TextureResource>(pResource);

	try {
		if (pStream->get() == std::istream::traits_type::eof()) return nullptr;
		pStream->unget();
	} catch (...) { return nullptr; }
	try {
		auto pos = pStream->tellg();
		png::reader reader(*pStream);
		reader.read_info();
		auto colorType = reader.get_color_type();
		auto width = reader.get_width();
		auto height = reader.get_height();
		pStream->seekg(pos);
		switch (colorType) {

			case png::color_type_gray:
				resource->setData(get_data<png::gray_pixel>(width, height, *pStream).data(), width, height,
								  Texture2DImageFormat::RED, Texture2DType::UNSIGNED_BYTE);
				break;
			case png::color_type_rgb:
				resource->setData(get_data<png::rgb_pixel>(width, height, *pStream).data(), width, height,
								  Texture2DImageFormat::RGB, Texture2DType::UNSIGNED_BYTE);
				break;
			case png::color_type_rgb_alpha:
				resource->setData(get_data<png::rgba_pixel>(width, height, *pStream).data(), width, height,
								  Texture2DImageFormat::RGBA, Texture2DType::UNSIGNED_BYTE);
				break;
			case png::color_type_gray_alpha:
				resource->setData(get_data<png::ga_pixel>(width, height, *pStream).data(), width, height,
								  Texture2DImageFormat::RG, Texture2DType::UNSIGNED_BYTE);
				break;
			default:;
		}
	} catch (...) {
		auto msg = ReportMessage::create();
		msg->setTitle("Failed to load texture resource");
		msg->setMessage("Exception thrown while parsing");
		return msg;
	}*/
	return nullptr;
}
} // namespace ke
