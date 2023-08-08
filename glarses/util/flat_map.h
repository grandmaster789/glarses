#ifndef GLARSES_UTIL_FLAT_MAP_H
#define GLARSES_UTIL_FLAT_MAP_H

#include <vector>
#include <utility>
#include <iosfwd>

namespace glarses {
	// Linear mapping, separate key and value storage; should be very fast for small-ish key/value stores
	// 
	// [NOTE] key must implement < and == operators (or spaceship); value should be at least moveable
	// [NOTE] for larger amounts of keys, a hash map is probably better
	// [NOTE] only trivial noexcept specifications are kept; more are possible but would need to be conditional for full correctness
	// [NOTE] possibly we could use pmr::vector instead to allow for reasonable integration with allocators
	template <
		typename t_Key,
		typename t_Value
	>
	class FlatMap {
	public:
		// Full value semantics (default construct + destruct + copy + move)

		      t_Value* operator[](const t_Key& key)       noexcept; // returns nullptr if not found
		const t_Value* operator[](const t_Key& key) const noexcept; // returns nullptr if not found

		std::pair<t_Key, t_Value> at(size_t index) const; // will throw if index is out of bounds

		void assign(const t_Key& key, const t_Value& value) noexcept;
		void assign(const t_Key& key, t_Value&& value) noexcept;

		[[nodiscard]] bool insert(const t_Key& key, const t_Value&  value) noexcept; // returns false if it would overwrite an entry
		[[nodiscard]] bool insert(const t_Key& key,       t_Value&& value) noexcept; // returns false if it would overwrite an entry

		bool contains(const t_Key& key) const noexcept;
		void erase(const t_Key& key);
		void clear() noexcept;

		size_t size() const noexcept;
		bool is_empty() const noexcept;

		const std::vector<t_Key>&   get_keys()   const noexcept;
		const std::vector<t_Value>& get_values() const noexcept;

		template <typename t_Callback> // void(const t_Key&, t_Value&)
		void foreach(t_Callback fn) const;

		friend std::ostream& operator << (std::ostream& os, const FlatMap& fm);

	private:
		std::vector<t_Key>   m_Keys;
		std::vector<t_Value> m_Values;
	};
}

#include "flat_map.inl"

#endif