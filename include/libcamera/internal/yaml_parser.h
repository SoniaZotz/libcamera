/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2022, Google Inc.
 *
 * yaml_parser.h - libcamera YAML parsing helper
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include <libcamera/base/class.h>

#include <libcamera/geometry.h>

namespace libcamera {

class File;
class YamlParserContext;

class YamlObject
{
public:
	YamlObject();
	~YamlObject();

	bool isValue() const
	{
		return type_ == Type::Value;
	}
	bool isList() const
	{
		return type_ == Type::List;
	}
	bool isDictionary() const
	{
		return type_ == Type::Dictionary;
	}

	std::size_t size() const;

#ifndef __DOXYGEN__
	template<typename T,
		 typename std::enable_if_t<
			 std::is_same_v<bool, T> ||
			 std::is_same_v<double, T> ||
			 std::is_same_v<int32_t, T> ||
			 std::is_same_v<uint32_t, T> ||
			 std::is_same_v<std::string, T> ||
			 std::is_same_v<Size, T>> * = nullptr>
#else
	template<typename T>
#endif
	T get(const T &defaultValue, bool *ok = nullptr) const;

	const YamlObject &operator[](std::size_t index) const;

	bool contains(const std::string &key) const;
	const YamlObject &operator[](const std::string &key) const;
	std::vector<std::string> memberNames() const;

private:
	LIBCAMERA_DISABLE_COPY_AND_MOVE(YamlObject)

	friend class YamlParserContext;

	enum class Type {
		Dictionary,
		List,
		Value,
	};

	Type type_;

	std::string value_;
	std::vector<std::unique_ptr<YamlObject>> list_;
	std::map<const std::string, std::unique_ptr<YamlObject>> dictionary_;
};

class YamlParser final
{
public:
	static std::unique_ptr<YamlObject> parse(File &file);
};

} /* namespace libcamera */
