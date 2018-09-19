#ifndef SECTIONTERATORS_H
#define SECTIONTERATORS_H

#include "Section.h"
#include <cstddef>
#include <utility>

namespace ini
{

	class SectionIterator
	{
	public:
		using difference_type = ptrdiff_t;
		using value_type = Section;
		using pointer = Section*;
		using reference = Section&;
		using iterator_category = std::forward_iterator_tag;

		SectionIterator() = default;
		SectionIterator(Section & section) : current{ section.sections.begin() }, end{ section.sections.end() }, state{ State::FULL } {};
		SectionIterator(const SectionIterator&) = default;
		SectionIterator& operator=(const SectionIterator&) = default;
		SectionIterator(SectionIterator&&) = default;
		SectionIterator& operator=(SectionIterator&&) = default;

		reference operator*() const {
			if (state == State::EMPTY || current == end) throw std::out_of_range{ "SectionIterator out of range" };
			return current->second;
		};

		pointer operator->() const {
			if (state == State::EMPTY || current == end) throw std::out_of_range{ "SectionIterator out of range" };
			return &(current->second);
		};

		SectionIterator& operator++() {
			if (state == State::EMPTY) throw std::out_of_range{ "SectionIterator out of range" };
			if (++current == end) state = State::EMPTY;
			return *this;
		};

		SectionIterator operator++(int) {
			auto x{ *this };
			if (state == State::EMPTY) throw std::out_of_range{ "SectionIterator out of range" };
			if (++current == end) state = State::EMPTY;
			return x;
		};

	private:
		enum class State { EMPTY, FULL };

		friend bool operator==(const SectionIterator& it_1, const SectionIterator& it_2);
		friend bool operator!=(const SectionIterator& it_1, const SectionIterator& it_2);

		std::unordered_map<std::string, Section>::iterator current, end;
		State state{ State::EMPTY };
	};

	inline bool ini::operator==(const SectionIterator & it_1, const SectionIterator & it_2)
	{
		if (it_1.state == SectionIterator::State::EMPTY && it_2.state == SectionIterator::State::EMPTY)
		{
			return true;
		}

		if (it_1.state != SectionIterator::State::EMPTY && it_2.state != SectionIterator::State::EMPTY)
		{
			return (it_1.current == it_2.current);
		}

		if (it_1.state == SectionIterator::State::EMPTY)
		{
			return (it_2.current == it_2.end);
		}

		return (it_1.current == it_1.end);
	}

	inline bool operator!=(const SectionIterator & it_1, const SectionIterator & it_2)
	{
		return !(it_1 == it_2);
	}

	class const_SectionIterator
	{
	public:
		using difference_type = ptrdiff_t;
		using value_type = const Section;
		using pointer = const Section * ;
		using reference = const Section & ;
		using iterator_category = std::forward_iterator_tag;

		const_SectionIterator() = default;
		const_SectionIterator(const Section & section) : current{ section.sections.cbegin() }, end{ section.sections.cend() }, state{ State::FULL } {};
		const_SectionIterator(const const_SectionIterator&) = default;
		const_SectionIterator& operator=(const const_SectionIterator&) = default;
		const_SectionIterator(const_SectionIterator&&) = default;
		const_SectionIterator& operator=(const_SectionIterator&&) = default;

		reference operator*() const {
			if (state == State::EMPTY || current == end) throw std::out_of_range{ "const_SectionIterator out of range" };
			return current->second;
		};

		pointer operator->() const {
			if (state == State::EMPTY || current == end) throw std::out_of_range{ "const_SectionIterator out of range" };
			return &(current->second);
		};

		const_SectionIterator& operator++() {
			if (state == State::EMPTY) throw std::out_of_range{ "const_SectionIterator out of range" };
			if (++current == end) state = State::EMPTY;
			return *this;
		};

		const_SectionIterator operator++(int) {
			auto x{ *this };
			if (state == State::EMPTY) throw std::out_of_range{ "const_SectionIterator out of range" };
			if (++current == end) state = State::EMPTY;
			return x;
		};

	private:
		enum class State { EMPTY, FULL };

		friend bool operator==(const const_SectionIterator& it_1, const const_SectionIterator& it_2);
		friend bool operator!=(const const_SectionIterator& it_1, const const_SectionIterator& it_2);

		std::unordered_map<std::string, Section>::const_iterator current, end;
		State state{ State::EMPTY };
	};

	inline bool ini::operator==(const const_SectionIterator & it_1, const const_SectionIterator & it_2)
	{
		if (it_1.state == const_SectionIterator::State::EMPTY && it_2.state == const_SectionIterator::State::EMPTY)
		{
			return true;
		}

		if (it_1.state != const_SectionIterator::State::EMPTY && it_2.state != const_SectionIterator::State::EMPTY)
		{
			return (it_1.current == it_2.current);
		}

		if (it_1.state == const_SectionIterator::State::EMPTY)
		{
			return (it_2.current == it_2.end);
		}

		return (it_1.current == it_1.end);
	}

	inline bool operator!=(const const_SectionIterator & it_1, const const_SectionIterator & it_2)
	{
		return !(it_1 == it_2);
	}
}



#endif // !SECTIONTERATORS_H