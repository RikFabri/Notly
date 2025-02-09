#include "FileExplorer.h"
#include <filesystem>
#include <iostream>
#include "glad/glad.h"
#include "FontRenderer.h"

namespace fs = std::filesystem;

FileExplorer::FileExplorer()
{
	const auto currentPath = fs::current_path();

	for (const auto& directory : fs::directory_iterator(currentPath))
	{
		m_CurrentFiles.emplace_back(directory.path().filename().string());
	}
}

void FileExplorer::Draw(const RenderArea& renderArea)
{
	// Todo: this needs to be calculated
	constexpr auto fileRenderHeight = 35;

	RenderArea r = renderArea;
	r.height = fileRenderHeight;


	for (const auto& entry : m_CurrentFiles)
	{
		FontRenderer::Get().DrawText(entry, r);
		r.y += fileRenderHeight;
	}
}
