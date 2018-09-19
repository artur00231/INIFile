#ifndef PROPERTYITERATORS_H
#define PROPERTYITERATORS_H

#include "Section.h"
#include <cstddef>
#include <utility>

namespace ini
{

	class PropertyIterator
	{
	public:
		using difference_type = ptrdiff_t;
		using value_type = Property;
		using pointer = Property * ;
		using reference = Property & ;
		using iterator_category = std::forward_iterator_tag;

		PropertyIterator() = default;
		PropertyIterator(Section & section) : current{ section.propertes.begin() }, end{ section.propertes.end() }, state{ State::FULL } {};
		PropertyIterator(const PropertyIterator&) = default;
		PropertyIterator& operator=(const PropertyIterator&) = default;
		PropertyIterator(PropertyIterator&&) = default;
		PropertyIterator& operator=(PropertyIterator&&) = default;

		reference operator*() const {
			if (state == State::EMPTY || current == end) throw std::out_of_range{ "PropertyIterator out of range" };
			return current->second;
		};

		pointer operator->() const {
			if (state == State::EMPTY || current == end) throw std::out_of_range{ "PropertyIterator out of range" };
			return &(current->second);
		};

		PropertyIterator& operator++() {
			if (state == State::EMPTY) throw std::out_of_range{ "PropertyIterator out of range" };
			if (++current == end) state = State::EMPTY;
			return *this;
		};

		PropertyIterator operator++(int) {
			auto x{ *this };
			if (state == State::EMPTY) throw std::out_of_range{ "PropertyIterator out of range" };
			if (++current == end) state = State::EMPTY;
			return x;
		};

	private:
		enum class State { EMPTY, FULL };

		friend bool operator==(const PropertyIterator& it_1, const PropertyIterator& it_2);
		friend bool operator!=(const PropertyIterator& it_1, const PropertyIterator& it_2);

		std::unordered_map<std::string, Property>::iterator current, end;
		State state{ State::EMPTY };
	};

	inline bool ini::operator==(const PropertyIterator & it_1, const PropertyIterator & it_2)
	{
		if (it_1.state == PropertyIterator::State::EMPTY && it_2.state == PropertyIterator::State::EMPTY)
		{
			return true;
		}

		if (it_1.state != PropertyIterator::State::EMPTY && it_2.state != PropertyIterator::State::EMPTY)
		{
			return (it_1.current == it_2.current);
		}

		if (it_1.state == PropertyIterator::State::EMPTY)
		{
			return (it_2.current == it_2.end);
		}

		return (it_1.current == it_1.end);
	}

	inline bool operator!=(const PropertyIterator & it_1, const PropertyIterator & it_2)
	{
		return !(it_1 == it_2);
	}

	class const_PropertyIterator
	{
	public:
		using difference_type = ptrdiff_t;
		using value_type = const Property;
		using pointer = const Property *;
		using reference = const Property &;
		using iterator_category = std::forward_iterator_tag;

		const_PropertyIterator() = default;
		const_PropertyIterator(const Section & section) : current{ section.propertes.cbegin() }, end{ section.propertes.cend() }, state{ State::FULL } {};
		const_PropertyIterator(const const_PropertyIterator&) = default;
		const_PropertyIterator& operator=(const const_PropertyIterator&) = default;
		const_PropertyIterator(const_PropertyIterator&&) = default;
		const_PropertyIterator& operator=(const_PropertyIterator&&) = default;

		reference operator*() const {
			if (state == State::EMPTY || current == end) throw std::out_of_range{ "const_PropertyIterator out of range" };
			return current->second;
		};

		pointer operator->() const {
			if (state == State::EMPTY || current == end) throw std::out_of_range{ "const_PropertyIterator out of range" };
			return &(current->second);
		};

		const_PropertyIterator& operator++() {
			if (state == State::EMPTY) throw std::out_of_range{ "const_PropertyIterator out of range" };
			if (++current == end) state = State::EMPTY;
			return *this;
		};

		const_PropertyIterator operator++(int) {
			auto x{ *this };
			if (state == State::EMPTY) throw std::out_of_range{ "const_PropertyIterator out of range" };
			if (++current == end) state = State::EMPTY;
			return x;
		};

	private:
		enum class State { EMPTY, FULL };

		friend bool operator==(const const_PropertyIterator& it_1, const const_PropertyIterator& it_2);
		friend bool operator!=(const const_PropertyIterator& it_1, const const_PropertyIterator& it_2);

		std::unordered_map<std::string, Property>::const_iterator current, end;
		State state{ State::EMPTY };
	};

	inline bool ini::operator==(const const_PropertyIterator & it_1, const const_PropertyIterator & it_2)
	{
		if (it_1.state == const_PropertyIterator::State::EMPTY && it_2.state == const_PropertyIterator::State::EMPTY)
		{
			return true;
		}

		if (it_1.state != const_PropertyIterator::State::EMPTY && it_2.state != const_PropertyIterator::State::EMPTY)
		{
			return (it_1.current == it_2.current);
		}

		if (it_1.state == const_PropertyIterator::State::EMPTY)
		{
			return (it_2.current == it_2.end);
		}

		return (it_1.current == it_1.end);
	}

	inline bool operator!=(const const_PropertyIterator & it_1, const const_PropertyIterator & it_2)
	{
		return !(it_1 == it_2);
	}
}



#endif // !PROPERTYITERATORS_H