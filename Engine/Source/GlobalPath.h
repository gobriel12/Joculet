#pragma once

namespace C9Engine
{
	class GlobalPath
	{
		GlobalPath(int argc, char** argv);
	public:
		static GlobalPath* Instance(int argc = 0, char** argv = nullptr);

		static const char* ms_MainWindowTitle;
		static const int ms_Weight, ms_Height;

		const char* m_EXE_PATH;
	private:
		static GlobalPath* ms_Instance;
	};
}
