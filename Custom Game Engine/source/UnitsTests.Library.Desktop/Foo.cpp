#include "pch.h"
#include "Foo.h"

using namespace std;

namespace UnitsTestsLibraryDesktop
{
	RTTI_DEFINITIONS(Foo)

	Foo::Foo() :
		_data(new int32_t(0))
	{
	}

	Foo::Foo(int32_t data) :
		_data(new int32_t(data))
	{

	}

	Foo::Foo(const Foo& other) :
		_data(new int32_t(*other._data))
	{

	}

	Foo& Foo::operator=(const Foo& other)
	{
		if (this != &other)
		{
			*_data = *other._data;
		}

		return *this;
	}

	Foo::Foo(Foo&& rhs) noexcept :
		_data(rhs._data)
	{
		rhs._data = nullptr;
	}

	Foo& Foo::operator=(Foo&& rhs) noexcept
	{
		if (this != &rhs)
		{
			delete _data;
			_data = rhs._data;
			rhs._data = nullptr;
		}

		return *this;
	}

	Foo::~Foo()
	{
		delete _data;
	}

	int32_t& Foo::Data()
	{
		return *_data;
	}

	const int32_t& Foo::Data() const
	{
		return *_data;
	}

	bool Foo::operator==(const Foo& rhs) const
	{
		if (_data == rhs._data)
		{
			return true;
		}

		return (*_data == *rhs._data);
	}

	bool Foo::operator!=(const Foo& rhs) const
	{
		return !operator==(rhs);
	}

	std::string Foo::ToString() const
	{
		return std::to_string(*_data);
	}
}