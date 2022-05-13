#pragma once
#include "RTTI.h"
#include "Factory.h"
#include <cstdint>

namespace UnitsTestsLibraryDesktop
{
	class Foo : public FieaGameEngine::RTTI
	{
	
		RTTI_DECLARATIONS(Foo, RTTI)

	public:
		Foo();
		Foo(std::int32_t data);
		Foo(const Foo& other);
		Foo& operator=(const Foo& other);
		Foo(Foo&& rhs) noexcept;
		Foo& operator=(Foo&& rhs) noexcept;
		virtual ~Foo();

		int& Data();
		const int& Data() const;

		bool operator==(const Foo& rhs) const;
		bool operator!=(const Foo& rhs) const;

		std::string ToString() const override;
	private:
		std::int32_t* _data;
	};

	ConcreteFactory(Foo, FieaGameEngine::RTTI)

	/*class FooFactory : public FieaGameEngine::Factory<FieaGameEngine::RTTI>
	{
	public:
		FooFactory()
		{
			Add(*this);
		}
		~FooFactory()
		{
			Remove(*this);
		}
		const std::string& ClassName() const override
		{
			return _className;
		}

		gsl::owner<FieaGameEngine::RTTI*> Create() const override
		{
			return new Foo;
		}

	private:
		const std::string _className{ "Foo" };
	};*/
}