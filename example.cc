#include <iostream>
#include <d.hh>
	
namespace foo {
	D_push_id(foo);
	
	void f() {
		D_push_id(f);	
		D_print(D_err, std::cerr, "foo.f err");
		D_print(D_trace, std::cerr, "foo.f trace");
	}
	void g() {
		D_print(D_warn, std::cerr, "foo warn");
		D_print(D_info, std::cerr, "foo info");
	}
}
	
void h1() {
	std::cerr << "Hello from h1\n";
}
void h2() {
	D_print(D_err, std::cerr, "<global> err");
	D_eval(D_info, h1());
}
void h3() {
	D_push_id(h3);
	D_eval(D_warn, std::cerr << D_add_context(D_warn) << ' ' << "h3 warn" << '\n');
	D_print(D_trace, std::cerr, "h3 trace");
}

int main(int argc, char** argv) {
	D_set_from_args(argc - 1, argv + 1, "-d");
	foo::f();
	foo::g();
	h2();
	h3();
}

