#pragma once
#include "fseq_impl.hpp"

namespace zh {

// fseq is a functor that applies unary functors to it in sequence
// Note: cannot declare fseq as alias because deduction guides are lost.
template <class... Functors>
struct fseq : private detail::fseq_impl<Functors..., void> {
private:
	using base = detail::fseq_impl<Functors..., void>;

public:
	fseq() = default;

	template <class... UFunctors>
	explicit fseq(UFunctors... functors) :
		base(functors...) {
	}

	using base::size;
	using base::operator();
};

// Deduction guide: contruct from lambas
template<class... UFunctors>
fseq(UFunctors...)->fseq<UFunctors...>;

} // namespace zh