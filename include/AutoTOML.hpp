#pragma once

#include <toml++/toml.h>

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace AutoTOML
{
	using namespace std::literals;

	using boolean_t = bool;
	using float_t = double;
	using integer_t = std::int64_t;
	using string_t = toml::string;

	class ISetting
	{
	public:
		ISetting() = delete;
		ISetting(const ISetting&) = delete;
		ISetting(ISetting&&) = delete;

		ISetting(string_t a_group, string_t a_key) :
			_group(std::move(a_group)),
			_key(std::move(a_key))
		{
			auto& settings = get_settings();
			settings.push_back(this);
		}

		virtual ~ISetting() = 0 {}

		ISetting& operator=(const ISetting&) = delete;
		ISetting& operator=(ISetting&&) = delete;

		[[nodiscard]] static std::vector<ISetting*>& get_settings() noexcept
		{
			static std::vector<ISetting*> settings;
			return settings;
		}

		[[nodiscard]] constexpr const string_t& group() const noexcept { return _group; }
		[[nodiscard]] constexpr const string_t& key() const noexcept { return _key; }
		void load(const toml::table& a_table) { do_load(a_table); }
		[[nodiscard]] toml::node_type type() const noexcept { return do_type(); }

	protected:
		virtual void do_load(const toml::table& a_table) = 0;
		[[nodiscard]] virtual toml::node_type do_type() const noexcept = 0;

	private:
		string_t _group;
		string_t _key;
	};

	namespace detail
	{
		template <
			class T,
			toml::node_type E>
		class tSetting :
			public ISetting
		{
		private:
			using super = ISetting;

		public:
			using value_type = T;
			using reference = value_type&;
			using const_reference = const value_type&;
			using pointer = value_type*;
			using const_pointer = const value_type*;

			tSetting() = delete;
			tSetting(const tSetting&) = delete;
			tSetting(tSetting&&) = delete;

			tSetting(string_t a_group, string_t a_key, value_type a_value) :
				super(std::move(a_group), std::move(a_key)),
				_value(std::move(a_value))
			{}

			~tSetting() = default;

			tSetting& operator=(const tSetting&) = delete;
			tSetting& operator=(tSetting&&) = delete;

			[[nodiscard]] constexpr reference operator*() noexcept { return get(); }
			[[nodiscard]] constexpr const_reference operator*() const noexcept { return get(); }

			template <
				class U = value_type,
				std::enable_if_t<
					std::is_class_v<U>,
					int> = 0>
			[[nodiscard]] constexpr pointer operator->() noexcept
			{
				return std::addressof(get());
			}

			template <
				class U = value_type,
				std::enable_if_t<
					std::is_class_v<U>,
					int> = 0>
			[[nodiscard]] constexpr const_pointer operator->() const noexcept
			{
				return std::addressof(get());
			}

			[[nodiscard]] constexpr reference get() noexcept { return _value; }
			[[nodiscard]] constexpr const_reference get() const noexcept { return _value; }

		protected:
			void do_load(const toml::table& a_table) override
			{
				const auto& node = a_table[group()][key()];
				const auto val = node.as<value_type>();
				if (val) {
					_value = val->get();
				} else {
					string_t err;
					err += '[';
					err += group();
					err += "] "sv;
					err += key();
					err += ": value is not "sv;
					if (!node) {
						err += "found"sv;
					} else if (!val) {
						err += "of expected type"sv;
					} else {
						err += "<UNKNOWN ERROR>"sv;
					}

					throw std::runtime_error(err);
				}
			}

			[[nodiscard]] toml::node_type do_type() const noexcept override { return E; }

		private:
			value_type _value;
		};
	}

	using bSetting = detail::tSetting<boolean_t, toml::node_type::boolean>;
	using fSetting = detail::tSetting<float_t, toml::node_type::floating_point>;
	using iSetting = detail::tSetting<integer_t, toml::node_type::integer>;
	using sSetting = detail::tSetting<string_t, toml::node_type::string>;
}
