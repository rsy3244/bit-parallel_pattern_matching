#pragma once

#include <cstddef>
#include <string>
#include <optional>

class Edge {
public:
	std::size_t src, dst;
	std::optional<char> label;

	Edge() = default;
	Edge(std::size_t src_, std::size_t dst_, std::optional<char> label_): src(src_), dst(dst_), label(label_) {}
};

class TNFA_imp {
	const std::string m_pattern;
	std::vector<std::vector<Edge>> edges;

public:
	TNFA_imp(const string &pattern);
};
