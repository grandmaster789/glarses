#pragma once

#include "flat_map.h"
#include "algorithm.h"

#include <ostream>
#include <iostream>

namespace util {
	template <typename K, typename V>
	V* FlatMap<K, V>::operator[](const K& key) noexcept {
		auto it = find(m_Keys, key);

		if (it == std::end(m_Keys))
			return nullptr;
		else {
			const auto index = std::distance(std::cbegin(m_Keys), it);

			return &m_Values[index];
		}
	}

	template <typename K, typename V>
	const V* FlatMap<K, V>::operator[](const K& key) const noexcept {
		auto it = find(m_Keys, key);

		if (it == std::end(m_Keys))
			return nullptr;
		else {
			const auto index = std::distance(std::begin(m_Keys), it);
			return &m_Values[index];
		}
	}

	template <typename K, typename V>
	std::pair<K, V> FlatMap<K, V>::at(size_t index) const {
		return std::make_pair(
			m_Keys[index],
			m_Values[index]
		);
	}

	template <typename K, typename V>
	void FlatMap<K, V>::assign(
		const K& key,
		const V& value
	) noexcept {
		auto it = find(m_Keys, key);

		if (it == std::end(m_Keys)) {
			// not found, new entry for both
			m_Keys.push_back(key);
			m_Values.push_back(value);
		}
		else {
			// key found, update entry
			auto idx = std::distance(std::cbegin(m_Keys), it);
			m_Values[idx] = value;
		}
	}

	template <typename K, typename V>
	void  FlatMap<K, V>::assign(
		const K& key,
		V&& value
	) noexcept {
		auto it = find(m_Keys, key);

		if (it == std::end(m_Keys)) {
			// not found, new entry for both
			m_Keys.push_back(key);
			m_Values.push_back(std::forward<V>(value));
		}
		else {
			// key found, update entry
			auto idx = std::distance(std::cbegin(m_Keys), it);
			m_Values[idx] = std::forward<V>(value);
		}
	}

	template <typename K, typename V>
	[[nodiscard]] bool FlatMap<K, V>::insert(
		const K& key,
		const V& value
	) noexcept {
		auto it = find(m_Keys, key);

		if (it == std::end(m_Keys)) {
			m_Keys.push_back(key);
			m_Values.push_back(value);

			return true;
		}
		else
			return false;
	}

	template <typename K, typename V>
	[[nodiscard]] bool FlatMap<K, V>::insert(
		const K& key,
		V&& value
	) noexcept {
		auto it = find(m_Keys, key);

		if (it == std::end(m_Keys)) {
			m_Keys.push_back(key);
			m_Values.push_back(std::forward<V>(value));
		}
		else
			return false;
	}

	template <typename K, typename V>
	bool FlatMap<K, V>::contains(const K& key) const noexcept {
		return util::contains(m_Keys, key);
	}

	template <typename K, typename V>
	void FlatMap<K, V>::erase(const K& key) {
		auto it = find(m_Keys, key);

		if (it == std::end(m_Keys))
			std::cout << "Key not found\n";
		else {
			auto idx = std::distance(std::cbegin(m_Keys), it);
			m_Keys.erase(it);
			m_Values.erase(std::begin(m_Values) + idx);
		}
	}

	template <typename K, typename V>
	void FlatMap<K, V>::clear() noexcept {
		m_Keys.clear();
		m_Values.clear();
	}

	template <typename K, typename V>
	size_t FlatMap<K, V>::size() const noexcept {
		return m_Keys.size();
	}

	template <typename K, typename V>
	bool FlatMap<K, V>::is_empty() const noexcept {
		return m_Keys.empty();
	}

	template <typename K, typename V>
	const std::vector<K>& FlatMap<K, V>::get_keys() const noexcept {
		return m_Keys;
	}

	template <typename K, typename V>
	const std::vector<V>& FlatMap<K, V>::get_values() const noexcept {
		return m_Values;
	}

	template <typename K, typename V>
	template <typename Fn>
	void FlatMap<K, V>::foreach(Fn callback) const {
		for (size_t i = 0; i < m_Keys.size(); ++i)
			callback(m_Keys[i], m_Values[i]);
	}

	template <typename K, typename V>
	std::ostream& operator << (std::ostream& os, const FlatMap<K, V>& fm) {
		fm.foreach([&](const K& key, const V& value) {
			os << key << ": " << value << "\n";
		});

		return os;
	}
}