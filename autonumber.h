#pragma once

class AutoNumber
{
public: // assign an automatic number
	static int getNext()
	{
		static int i = 0;
		i++;
		return i;
	}

protected:
	int	auto_id;

public: // operators
	AutoNumber()
	{
		auto_id = getNext();
	}

	AutoNumber& operator=(const AutoNumber&)
	{
	}
};

class AutoNumberIdentifier
{
private: // the tracker
	int _auto_number_id;

public: // automatic number
	AutoNumberIdentifier()
	{
		_auto_number_id = AutoNumber::getNext();
	}

	AutoNumberIdentifier(const AutoNumberIdentifier& R)
	{
		_auto_number_id = AutoNumber::getNext();
	}

	AutoNumberIdentifier& operator=(const AutoNumberIdentifier& R)
	{
		return *this;
	}

public: // automatic id
	int getAutoNumber()
	{
		return _auto_number_id;
	}
};