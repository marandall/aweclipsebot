#pragma once

namespace Validators
{
	class Integer
	{
	private:
		bool	m_success;
		int		m_result;

	public:
		Integer(const char* _Input)
		{
			char* end_char;
			int result = strtol(_Input, &end_char, 0);
			if (errno == ERANGE) {
				m_success = false;
			} else {
				if (*end_char == '\0') {
					m_success = true;
					m_result = result;
				} else {
					m_success = false;
				}
			}
		}

		bool Valid()  const	   { return m_success; }
		int Result() const    { return m_result;  }
	};

	class Double
	{
	private:
		bool	m_success;
		double	m_result;

	public:
		Double(const char* _Input)
		{
			char* end_char;
			double result = strtod(_Input, &end_char);
			if (errno == ERANGE) {
				m_success = false;
			} else {
				if (*end_char == '\0') {
					m_success = true;
					m_result = result;
				} else {
					m_success = false;
				}
			}
		}

		bool Valid()  const	   { return m_success; }
		double Result() const    { return m_result;  }
	};
}