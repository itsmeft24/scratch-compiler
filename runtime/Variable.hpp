#pragma once
#include <string>
#include <optional>
namespace scratch {
	class Variable
	{
	private:
		enum {
			None,
			Integer,
			Double,
			String,
		} m_tag;
		union {
			std::string* m_string;
			long long m_integer;
			double m_double;
		};
		std::optional<double> into_number() const;
	public:
		Variable();
		Variable(const Variable&);
		Variable(long long);
		Variable(int);
		Variable(double);
		Variable(const std::string&);
		~Variable();

		Variable& operator=(const Variable&);
		bool operator==(const Variable&);
	};
};

static_assert(sizeof(long long) == sizeof(double));
