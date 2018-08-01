#pragma once
#include "Date.h"
#include "Hashtag.h"

namespace WordsFactory
{
	class Basic_WordFactory
	{
	public:
		virtual ~Basic_WordFactory() = default;

		virtual Word * create_word(const String & value) const;
	};

	class Concrete_WordFactory : public Basic_WordFactory
	{
	public:
		Word * create_word(const String & value) const override;
	};
};

