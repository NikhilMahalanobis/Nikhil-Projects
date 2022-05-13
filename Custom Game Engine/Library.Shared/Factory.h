#pragma once

#include "HashMap.h"
#include <gsl/gsl>

namespace FieaGameEngine
{
	template<typename T> class Factory
	{
	public:
		/// <summary>
		/// Compiler supplied Defaulted Constructor for Factory;
		/// </summary>
		Factory() = default;

		/// <summary>
		/// Compiler supplied virtual Defaulted Destructor for Factory
		/// </summary>
		virtual ~Factory() = default;

		/// <summary>
		/// Pure Virtual ClassName method for Factory
		/// </summary>
		/// <returns> Returns a const reference to a std::string </returns>
		virtual const std::string& ClassName() const = 0;

		/// <summary>
		/// Pure Virtual Create method for Factory
		/// </summary>
		/// <returns> Returns a gls::owner of a pointer of templated type T </returns>
		virtual gsl::owner<T*> Create() const = 0;

		/// <summary>
		/// Static Find method for Factory
		/// </summary>
		/// <param name="className"> Takes a const reference to a std::string </param>
		/// <returns> Returns a const pointer to a const Factory of templated type T </returns>
		static const Factory<T>* const Find(const std::string& className);

		/// <summary>
		/// Static Create method for Factory 
		/// </summary>
		/// <param name="className"> Takes a const reference to a std::string </param>
		/// <returns> Returns a gsl::owner of a pointer of templated type T </returns>
		static gsl::owner<T*> Create(const std::string className);

	protected:
		/// <summary>
		/// Static Add method for Factory 
		/// </summary>
		/// <param name="factory"> Takes a const reference to a Factory of templated type T </param>
		static void Add(const Factory<T>& factory);
		
		/// <summary>
		/// Static Remove method for Factory 
		/// </summary>
		/// <param name="factory"> Takes a const reference to a Factory of templated type T </param>
		static void Remove(const Factory<T>& factory);
	private:
		static HashMap<std::string, const Factory<T>* const> _factoryTable;
	};

#define ConcreteFactory(ConcreteProductType, AbstractProductType)																							\
		class ConcreteProductType##Factory : public FieaGameEngine::Factory<AbstractProductType>															\
		{																																					\
			public:																																			\
				ConcreteProductType##Factory() { Add(*this); }																								\
				~ConcreteProductType##Factory() { Remove(*this); }																							\
				const std::string& ClassName() const override { return _className; }																		\
				gsl::owner<AbstractProductType*> Create() const override { return new ConcreteProductType; }												\
		private:																																			\
			const std::string _className{ #ConcreteProductType };																							\
		};
}

#include "Factory.inl"