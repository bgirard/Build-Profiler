export DYLD_FORCE_FLAT_NAMESPACE=
export DYLD_INSERT_LIBRARIES=$(PWD)/bin/libmtrace.dylib
$(PWD)/bin/simple_test
