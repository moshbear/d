// Debug level: structures, prototypes and macros.
//
// The following macros are defined:
// 	D_eval(L, ...)    - conditionally evaluate <...> for D_level L and currently-scoped D_identifier
// 	D_print(L, o, s)  - conditionally print `s` to `o` for D_level L and currently-scoped D_identifier
// 	D_cprint(L, o, s) - as above, but a copy is made of the string argument for scoping reasons
// 	D_push_id(ID) - bring the most-local D_identifier with local part ID into scope
// 	Defining NDEBUG converts these into empty statements.
// The following D_level (debug level) enumerations are defined:
// 	D_silent - print nothing (errors which may break the algorithm greviously should printed);
// 			output prefix is "XX"
// 	D_err	 - print errors (and the above); output prefix is "EE"
// 	D_warn	 - print warnings (and the above); output prefix is "WW"
// 	D_info	 - print informatives (and the above); output prefix is "II"
// 	D_trace	 - print tracing information (and the above); output prefix is "TT"
// 	(*)	 - Undefined level; output prefix is "??"
// The following structs are defined:
// 	D_id_list - A refinement-based identifier list that permits for fuzzy matching
// 		of D_level by scope inheritance.
//		Use D_push_id(ID) for instantiation to make chaining painless.
// The following functions are defined:
// 	D_set(D_id_list const&, D_level)  - set the debug level for some scope-qualified D_id_list
// 	D_get(D_id_list const&)		  - get the current debug level for some ...
// 	D_ok(D_id_list const&, D_level)   - query whether the passed debug level is active for some ...
// 	D_xprint(D_id_list const& id, D_level d, std::ostream& o, std::string const& s)
// 			- print message `s` to `o` from scope `id` with debug level `d`
// 	D_cxprint(<as above, but std::string for fourth arg>)
//			- copy `s` and perform as above
//	D_set_from_string({char const *, std::string const &}) - set D_level scope tree based on input
//		string
//		Format: 
//			Let s_I be a scope token and T=S_1(:S_i)* be a scope list
//			Then T=D(,T=D)* denotes a list of D_levels D to set for scope-lists T
//			A missing T= implies global D.
//			In the event of duplicates, the last value holds.
//	D_set_from_args(int argc, char const* const* argv, char* const d_str)
//			- Scan [argv[0], ... argv[argc-1]] for d_str and forward to D_set_from_string.
//	D_{set,get,ok} are thread-safe.
//
// 2014.12.30 moshbear Bugfixes for v2
// 2014.12.27 moshbear v2
// 2014.12.09 moshbear v1
// Licenced under WTFPL.
#ifndef D_HH
#define D_HH

#include <string>
#include <iosfwd>

enum D_level {
	D_silent,
	D_err,
	D_warn,
	D_info,
	D_trace,
};

// To allow refinable scoping of D_level, we introduce the concept of an identifier list.
// The most recent identifier is up first, and `this->up` is called until the parent with
// `this->up == nullptr` is found to construct our chain.
// When D_get is called, the list is traversed to parent to get the scope identifiers and
// matching is done by recursive-best-match.
struct D_id_list {
	char const* id;
	D_id_list const* up;

	constexpr D_id_list()
	: id(""), up(nullptr)
	{ }
	constexpr D_id_list(char const* _id, D_id_list const* _up)
	: id(_id), up(_up)
	{ }
};

static D_id_list D_identifier;

struct D_context {
	D_id_list const* id;
	D_level level;
	constexpr D_context()
	: id(&::D_identifier), level(D_silent)
	{ }
	constexpr D_context(D_id_list const& _id, D_level const _lv)
	: id(&_id), level(_lv)
	{ }
};
struct D_scan_tag { };


void D_set(D_context const&);
D_level D_get(D_id_list const& id);
bool D_ok(D_context const&);
void D_xprint(D_context const&, std::ostream& o, std::string const& s);
inline void D_xcprint(D_context const& d, std::ostream& o, std::string s) {
	D_xprint(d, o, s);
}
void D_set_from_string(std::string const&);
void D_set_from_args(int argc, char const* const* argv, char const* d_str);

std::istream& operator>> (std::istream&, D_scan_tag&);
std::ostream& operator<< (std::ostream&, D_id_list const&);
std::ostream& operator<< (std::ostream&, D_level const);
std::ostream& operator<< (std::ostream&, D_context const&);

#define D_CAT3X(X,Y,Z) X##Y##Z
#define D_CAT3(X,Y,Z) D_CAT3X(X,Y,Z)
#ifndef NDEBUG
#define D_eval(L, ...) if (D_ok(D_add_context(L))) do { __VA_ARGS__; } while(0)
#define D_print(L, o, s) if (D_ok(D_add_context(L))) do { D_xprint(D_add_context(L), o, s); } while(0)
#define D_cprint(L, o, s) if (D_ok(D_add_context(L))) do { D_xcprint(D_add_context(L), o, s); } while(0)
// Warning: this macro can't be quasi-hygeinic by means of do{...}while(0) as the { } will effect scope loss
#define D_push_id(ID) \
	static constexpr D_id_list const* D_CAT3(SAVE,ID,__LINE__) = &D_identifier; \
	static constexpr D_id_list D_identifier(#ID, D_CAT3(SAVE,ID,__LINE__))
#define D_add_context(D_level) D_context{ D_identifier, D_level }
#else
#define D_eval(...)
#define D_print(...)
#define D_cprint(...)
#define D_push_id(...)
#define D_add_context(...)
#endif

#endif
