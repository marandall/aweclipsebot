#pragma once
#include <vector>

class FileReader
{
private:
	std::vector<String>	lines;

public: // operators for accessing indexes
	String operator[](size_t i) const { return lines[i]; }
	size_t size() const { return lines.size(); }
	bool is_ok;

public: // load from the file
	bool Load(const char* FilePath)
	{
		lines.reserve(5000);

		FILE* fptr = NULL;
		errno_t err = fopen_s(&fptr, FilePath, "r");
		if (err != 0)
			return false;
		char import_arr[512];
		CStringA lineText;
		while (fgets(import_arr, 512, fptr)) {
			lineText = import_arr;
			lineText.TrimRight();
			lines.push_back(lineText);
		}
		fclose(fptr);
		return true;
	}

	FileReader(String FilePath)
	{
		is_ok = Load(FilePath);
	}

	bool IsOK()
	{
		return is_ok;
	}

	//
	// read a binary file
	//
	vector<BYTE> ReadBinaryFile(String File)
	{
		// read the information
		BYTE			buffer[1024 * 5];
		size_t			len = 0;
		vector<BYTE>	vbyte;
		FILE*			fptr = 0;

		// attempt to open the fil
		errno_t err = fopen_s(&fptr, File, "rb");
		if (err != 0) 
			return vector<BYTE>();

		// load each byte
		while ((len = fread(buffer, 1, sizeof buffer, fptr)) != 0)
			for (size_t i = 0; i < len; i++)
				vbyte.push_back(buffer[i]);
		
		// close the file
		fclose(fptr);
		return vbyte;
	}
};

class FileWriter
{
private:
	FILE*	m_fptr;
	errno_t m_errno;

public:
	FileWriter(String Path, char* method = "w")
	{
		m_fptr = NULL;
		m_errno = fopen_s(&m_fptr, Path, method);
	}

	~FileWriter()
	{
		if (m_fptr)
			fclose(m_fptr);
	}

	void WriteLine(String Line)
	{
		fprintf_s(m_fptr, "%s\n", (const char*)Line);
	}

	void write(String Line)
	{
		fprintf_s(m_fptr, "%s", (const char*)Line);
	}

	bool IsOK()
	{
		return m_fptr != 0;
	}

	errno_t getError()
	{
		return m_errno;
	}

	static errno_t WriteBinaryFile(String Path, String Text)
	{
		FileWriter t(Path);
		if (!t.IsOK()) return t.getError();
		t.write(Text);
		return t.getError();
	}

	static errno_t SWriteLine(String Path, String Line)
	{
		FileWriter wr(Path, "a");
		if (wr.IsOK())
		{
			wr.WriteLine(Line);
		}

		return 0;
	}
};