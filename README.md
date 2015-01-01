#d
A lightweight debug printing library for C++11 applications.

This library provides variably-scoped variably-verbose tracing.

## Synopsis
In application code, use `D_push_id` to register the scope with the D system.
Runtime cost is negligible due to abundant use of constexpr. Likewise, use of `static`
avoids linker issues if two translation units `#include` the same file.

In `main()`, call `D_set_from_args` to register D_level scopes as needed. The command string is found
when option matches parameter `d_str`. In a designated input parser, call `D_set_from_string` as appropriate.
Each D_scope indicator takes the form of `(s1[.sI]*=)d`, where `d` is the specified D_level and the regex
to the left of the equals sign denotes scopes.
Each scope token is dot-delimited; each indicator token is comma-delimited.

After registration, three general options are available:
* `D_print`, with a specified `D_level`, output stream, and string.
* `D_eval`, using standard streams and pushing `D_add_context(D_level)` to the stream as needed.
* `D_eval`, with arbitrary code run conditionally.

The following D_levels are available

<table>
<tr><td>D_level</td><td>D_set_from_... token</td><td>Output pretty-print</td><td>Intent</td></tr>
<tr><td>D_silent</td><td>x</td><td>XX</td><td>Print/Evaluate nothing (fatal errors may be excepted)</td></tr>
<tr><td>D_err</td><td>e</td><td>EE</td><td>Print/Evaluate error, superceding `D_silent`</td></tr>
<tr><td>D_warn</td><td>w</td><td>WW</td><td>Print/Evaluate warning, superceding `D_err`</td></tr>
<tr><td>D_info</td><td>i</td><td>II</td><td>Print/Evaluate informative, superceding `D_warn`</td></tr>
<tr><td>D_trace</td><td>t</td><td>TT</td><td>Print/Evaluate trace, superceding `D_info`</td></tr>
<tr><td>?</td><td>?</td><td>??</td><td>Undefined D_level</td></tr>
</table>

## Synopsis 2: advanced usage
Advanced functions ("external primitives"):

`D_cprint` - like `D_print`, but the string is explicitly copied

`D_get` - Query the expected `D_level` based on the passed `D_id_list` by searching the scope tree from the root for the largest D_level

`D_set` - Set the `D_level` for the passed `D_list`, creating parent scoping nodes as needed and setting their `D_level`

`D_ok` - Check the value of `D_get` against `D_level`

`D_xprint` - Prints given string to given output stream, prepending a pretty-printing of `D_level` and `D_id_List`

`D_cxprint` - Like `D_xprint` but the string is explicitly copied

## Example

See example.cc

## Caveats
* Structure of `D_id_list` is left undescribed intentionally. Its values are useless to the user, anyways
  and there's not much robustness against intentionally bad input.
* `D_*` is reserved for future implementations.

## Todo
* Caching for `D_get` internals (might help with potential lock-free)

## Licensing
WTFPL v2. See `COPYING` or (http://www.wtfpl.net/about/).
