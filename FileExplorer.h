#pragma once
#include "Frame.h"
#include <vector>
#include <string>

class FileExplorer final : public IFrame
{
public:
	FileExplorer();

private:

	void Draw(const RenderArea& renderArea) final;


	std::vector<std::string> m_CurrentFiles;
};

