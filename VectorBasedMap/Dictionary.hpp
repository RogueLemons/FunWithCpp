#pragma once
#include <vector>

namespace Charis {

	namespace Private {

		template<typename Key, typename Value>
		struct KeyValuePair {
			Key key{};
			Value value{};
		};

		/// <summary>
		/// Uses a vector to keep track of keys and their related values.
		/// </summary>
		template<typename Key, typename Value>
		class Dictionary {
		public:
			/// <summary>
			/// Adds a new key-value pair if the given key does not already exist.
			/// </summary>
			/// <param name="key">Key used to look up the value.</param>
			/// <param name="value">Value connected to the key.</param>
			/// <param name="overwrite">If true and the key already exists, its old value will be overwritten.</param>
			void Add(const Key& key, const Value& value, bool overwrite = true);

			/// <summary>
			/// Adds a new key-value pair WITHOUT looking if it already exists. Faster than normal Add but requires coder to verify no duplicate keys are added.
			/// </summary>
			/// <param name="key">Key used to look up the value.</param>
			/// <param name="value">Value connected to the key.</param>
			void QuickAdd(const Key& key, const Value& value);

			/// <summary>
			/// Looks up the value for a given key.
			/// </summary>
			/// <param name="key">Key used to look up the value.</param>
			/// <returns>Returns a pointer to the value if it exists. If no pair matches the key then returns nullptr.</returns>
			Value* At(const Key& key);

			/// <returns>Returns a vector containing all key-value pairs.</returns>
			const std::vector<KeyValuePair<Key, Value>>& List();

			/// <returns>Returns the number of key-value pairs.</returns>
			size_t Size();
		private:
			std::vector<KeyValuePair<Key, Value>> m_Pairs{};
		};

		template<typename Key, typename Value>
		inline void Dictionary<Key, Value>::Add(const Key& key, const Value& value, bool overwrite)
		{
			Value* loopupValue = At(key);
			if (loopupValue == nullptr) {
				m_Pairs.push_back({ key, value });
				auto hej = m_Pairs.size();
				return;
			}
		}

		template<typename Key, typename Value>
		inline void Dictionary<Key, Value>::QuickAdd(const Key& key, const Value& value)
		{
			m_Pairs.push_back({ key, value });
		}

		template<typename Key, typename Value>
		inline Value* Dictionary<Key, Value>::At(const Key& key)
		{
			for (auto& pair : m_Pairs) {
				if (pair.key == key)
					return &pair.value;
			}
			return nullptr;
		}

		template<typename Key, typename Value>
		inline const std::vector<KeyValuePair<Key, Value>>& Dictionary<Key, Value>::List()
		{
			return m_Pairs;
		}

		template<typename Key, typename Value>
		inline size_t Dictionary<Key, Value>::Size()
		{
			return m_Pairs.size();
		}

	}
}