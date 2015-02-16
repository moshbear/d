#include <iostream>
#include <d.hh>
	
namespace foo {
	D_push_id(foo);
	
	void f() {
		D_push_id(f);	
		D_print(D_err, D_out, "foo.f err");
		D_print(D_trace, D_out, "foo.f trace");
	}
	void g() {
		D_print(D_warn, D_out, "foo warn");
		D_print(D_info, D_out, "foo info");
	}
}
	
void h1() {
	D_out << "Hello from h1\n";
}
void h2() {
	D_print(D_err, D_out, "<global> err");
	D_eval(D_info, h1());
}
void h3() {
	D_push_id(h3);
	D_eval(D_warn, D_out << D_add_context(D_warn) << ' ' << "h3 warn" << '\n');
	D_print(D_trace, D_out, "h3 trace");
}

int main(int argc, char** argv) {
	D_set_from_args(argc - 1, argv + 1, "-d");
	foo::f();
	foo::g();
	h2();
	h3();
}

