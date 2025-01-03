#include "Asset.h"

#include "Renderer/Texture.h"
#include "Renderer/Model.h"
#include "Renderer/Material.h"
#include "Renderer/Shader.h"
#include "Renderer/Font.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace Core
{
	using namespace Gfx;

	static std::string RemoveFileExtension(const std::string& filename) 
	{
		size_t lastDot = filename.find_last_of('.');
		if (lastDot == std::string::npos) 
		{
			return filename; // No extension found
		}
		return filename.substr(0, lastDot);
	}

	void AssetRegistry::SerializeMetadata(const AssetMetadata& metadata)
	{
		std::string path = RemoveFileExtension(metadata.Path) + ".assetinfo";

		// Normalize the path
		std::filesystem::path filePath(path);
		filePath = filePath.make_preferred(); // This normalizes path separators based on the OS.

		// Ensure the parent directory exists
		std::filesystem::create_directories(filePath.parent_path());

		YAML::Emitter out;
		out << YAML::BeginMap;
		{
			out << YAML::Key << "ID" << YAML::Value << metadata.ID;
			out << YAML::Key << "Name" << YAML::Value << metadata.Name;
			out << YAML::Key << "Path" << YAML::Value << metadata.Path;
			out << YAML::Key << "Type" << YAML::Value << metadata.Type;

			if (!metadata.Dependencies.empty())
			{
				for (auto& dependency : metadata.Dependencies)
				{
					out << YAML::Key << dependency.first << YAML::Value << dependency.second;
				}
			}
		}
		out << YAML::EndMap;

		// Write to the file
		std::ofstream fout(filePath);
		fout << out.c_str();
		fout.close();
	}

	void AssetRegistry::Serialize()
	{
		for (auto& texture : m_Textures)
			SerializeMetadata(texture.first);

		for (auto& model : m_Models)
			SerializeMetadata(model.first);

		for (auto& material : m_Materials)
			SerializeMetadata(material.first);

		for (auto& shader : m_Shaders)
			SerializeMetadata(shader.first);

		for (auto& font : m_Fonts)
			SerializeMetadata(font.first);
	}

	AssetMetadata AssetRegistry::DeserializeMetadata(const std::string& filename)
	{
		std::ifstream fin(filename);
		if (!fin.is_open()) 
		{
			LOG_ERROR("Couldn't load " + filename + " metadata file");
			return AssetMetadata{};
		}
		YAML::Node metadataNode = YAML::Load(fin);
		fin.close();

		AssetMetadata metadata;
		metadata.ID   =	metadataNode["ID"].as<uint64_t>();
		metadata.Name = metadataNode["Name"].as<std::string>();
		metadata.Path = metadataNode["Path"].as<std::string>();
		metadata.Type = metadataNode["Type"].as<std::string>();

		if (metadataNode["Dependencies"].IsMap())
		{
			for (auto node : metadataNode["Dependencies"])
			{
				metadata.Dependencies[node.first.as<std::string>()] = node.second.as<uint64_t>();
			}
		}

		// Now add the metadata to its corresponding asset storage and pair it with placeholder
		if (metadata.Type == "Texture")
			m_Textures[metadata] = CreateRef<Texture>(metadata, TextureSpecifiation{});
		else if (metadata.Type == "Model")
			m_Models[metadata] = CreateRef<Model>(metadata);
		else if (metadata.Type == "Material")
			m_Materials[metadata] = CreateRef<Material>(metadata);
		else if (metadata.Type == "Shader")
			m_Shaders[metadata] = CreateRef<Shader>(metadata);
		else if (metadata.Type == "Font")
			m_Fonts[metadata] = CreateRef<Font>(metadata);
		else
			LOG_ERROR("Unkown asset type: " + metadata.Type);

		return metadata;
	}

	void AssetRegistry::Deserialize()
	{
		auto paths = Scan();
		std::vector<std::filesystem::path> processedAssets; // Assets that already come with .assetinfo
		
		for (std::filesystem::path path : paths)
		{
			if (path.extension() == ".assetinfo")
			{
				DeserializeMetadata(path.string());
				processedAssets.push_back(path);
			}
		}

		for (std::filesystem::path path : paths)
		{
			std::filesystem::path possibleAssetMetadataPath = path;

			if (std::find(processedAssets.begin(), processedAssets.end(), possibleAssetMetadataPath.replace_extension(".assetinfo")) == processedAssets.end())
			// The asset has no corresponding .assetinfo metadata
			// We have to generate it
			{
				AssetMetadata metadata;
				metadata.Path = path.string();
				metadata.Name = path.stem().string();
				metadata.ID = UUID();

				std::string extension = path.extension().string();

				if (extension == ".png" || extension == ".jpg")
				{
					metadata.Type = "Texture";
					m_Textures[metadata] = CreateRef<Texture>(metadata, TextureSpecifiation{});
				}
				else if (extension == ".obj" || extension == ".fbx")
				{
					metadata.Type = "Model";
					m_Models[metadata] = CreateRef<Model>(metadata);
				}
				else if (extension == ".glsl")
				{
					metadata.Type = "Shader";
					m_Shaders[metadata] = CreateRef<Shader>(metadata);
				}
				else if (extension == ".ttf")
				{
					metadata.Type = "Font";
					m_Fonts[metadata] = CreateRef<Font>(metadata);
				}
				else if (extension == ".material") // Shared material
				{
					metadata.Type = "Material";
					m_Materials[metadata] = CreateRef<Material>();
				}
			}
		}
	}

	std::vector<std::filesystem::path> AssetRegistry::Scan() const
	{
		std::vector<std::filesystem::path> paths;

		std::function<void(const std::string&)> ScanDirectories = [&](const std::string& directory)
		{
			for (const auto& entry : std::filesystem::directory_iterator(directory))
			{
				if (entry.is_directory())
				{
					ScanDirectories(entry.path().string());
				}
				paths.push_back(entry.path());
			}
		};
		ScanDirectories(m_AssetDirectory);

		return paths;
	}

	void AssetRegistry::Free()
	{
		m_Textures.Free();
		m_Models.Free();
		m_Materials.Free();
		m_Shaders.Free();
		m_Fonts.Free();
	}

	// Texture
	template <>
	UUID AssetRegistry::Track<Texture>(Ref<Texture> texture)
	{
		return m_Textures.Add(texture->GetMetadata().ID, texture);
	}

	template <>
	Ref<Texture> AssetRegistry::Get<Texture>(const AssetMetadata& metadata)
	{
		Ref<Texture> texture = m_Textures.Get(metadata);
		if (!texture->IsLoaded()) texture->Load();
		return texture;
	}
	template <>
	Ref<Texture> AssetRegistry::Get<Texture>(UUID ID)
	{
		Ref<Texture> texture = m_Textures.Get(ID);
		if (!texture->IsLoaded()) texture->Load();
		return texture;
	}
	template <>
	Ref<Texture> AssetRegistry::Get<Texture>(const std::string& path)
	{
		Ref<Texture> texture = m_Textures.Get(path);
		if (!texture->IsLoaded()) texture->Load();
		return texture;
	}

	template <>
	bool AssetRegistry::Exists<Texture>(const AssetMetadata& metadata)
	{
		return m_Textures.Exists(metadata);
	}

	// Model
	template <>
	UUID AssetRegistry::Track<Model>(Ref<Model> model)
	{
		return m_Models.Add(model->GetMetadata().ID, model);
	}

	template <>
	Ref<Model> AssetRegistry::Get<Model>(const AssetMetadata& metadata)
	{
		Ref<Model> model = m_Models.Get(metadata);
		if (!model->IsLoaded()) model->Load();
		return model;
	}
	template <>
	Ref<Model> AssetRegistry::Get<Model>(UUID ID)
	{
		Ref<Model> model = m_Models.Get(ID);
		if (!model->IsLoaded()) model->Load();
		return model;
	}
	template <>
	Ref<Model> AssetRegistry::Get<Model>(const std::string& path)
	{
		Ref<Model> model = m_Models.Get(path);
		if (!model->IsLoaded()) model->Load();
		return model;
	}

	template <>
	bool AssetRegistry::Exists<Model>(const AssetMetadata& metadata)
	{
		return m_Models.Exists(metadata);
	}

	// Material
	template <>
	UUID AssetRegistry::Track<Material>(Ref<Material> material)
	{
		return m_Materials.Add(material->ID, material);
	}

	template <>
	Ref<Material> AssetRegistry::Get<Material>(const AssetMetadata& metadata)
	{
		Ref<Material> material = m_Materials.Get(metadata);
		if (!material->IsLoaded()) material->Deserialize();
		return material;
	}
	template <>
	Ref<Material> AssetRegistry::Get<Material>(UUID ID)
	{
		Ref<Material> material = m_Materials.Get(ID);
		if (!material->IsLoaded()) material->Deserialize();
		return material;
	}
	template <>
	Ref<Material> AssetRegistry::Get<Material>(const std::string& path)
	{
		Ref<Material> material = m_Materials.Get(path);
		if (!material->IsLoaded()) material->Deserialize();
		return material;
	}

	template <>
	bool AssetRegistry::Exists<Material>(const AssetMetadata& metadata)
	{
		return m_Materials.Exists(metadata);
	}

	// Shader
	template <>
	UUID AssetRegistry::Track<Shader>(Ref<Shader> shader)
	{
		return m_Shaders.Add(shader->GetMetadata().ID, shader);
	}

	template <>
	Ref<Shader> AssetRegistry::Get<Shader>(const AssetMetadata& metadata)
	{
		Ref<Shader> shader = m_Shaders.Get(metadata);
		if (!shader->IsLoaded()) shader->Load();
		return shader;
	}
	template <>
	Ref<Shader> AssetRegistry::Get<Shader>(UUID ID)
	{
		Ref<Shader> shader = m_Shaders.Get(ID);
		if (!shader->IsLoaded()) shader->Load();
		return shader;
	}
	template <>
	Ref<Shader> AssetRegistry::Get<Shader>(const std::string& path)
	{
		Ref<Shader> shader = m_Shaders.Get(path);
		if (!shader->IsLoaded()) shader->Load();
		return shader;
	}

	template <>
	bool AssetRegistry::Exists<Shader>(const AssetMetadata& metadata)
	{
		return m_Shaders.Exists(metadata);
	}

	// Font
	template <>
	UUID AssetRegistry::Track<Font>(Ref<Font> font)
	{
		return m_Fonts.Add(font->GetMetadata().ID, font);
	}

	template <>
	Ref<Font> AssetRegistry::Get<Font>(const AssetMetadata& metadata)
	{
		Ref<Font> font = m_Fonts.Get(metadata);
		if (!font->IsLoaded()) font->Load();
		return font;
	}
	template <>
	Ref<Font> AssetRegistry::Get<Font>(UUID ID)
	{
		Ref<Font> font = m_Fonts.Get(ID);
		if (!font->IsLoaded()) font->Load();
		return font;
	}
	template <>
	Ref<Font> AssetRegistry::Get<Font>(const std::string& path)
	{
		Ref<Font> font = m_Fonts.Get(path);
		if (!font->IsLoaded()) font->Load();
		return font;
	}

	template <>
	bool AssetRegistry::Exists<Font>(const AssetMetadata& metadata)
	{
		return m_Fonts.Exists(metadata);
	}

}