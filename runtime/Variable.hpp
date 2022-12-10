#pragma once
#include <mutex>
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
		mutable std::mutex mutex;

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
		/*
		operator bool() const;
		operator std::string() const;
		operator long long() const;
		void change_by(const double);
		*/
	};
};

static_assert(sizeof(long long) == sizeof(double));
