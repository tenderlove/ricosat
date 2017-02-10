#include <ruby.h>
#include <picosat.h>

VALUE cRicoSat;

void Init_ricosat() {
    cRicoSat = rb_define_class("RicoSat", rb_cObject);
    rb_define_const(cRicoSat, "UNKNOWN", INT2NUM(PICOSAT_UNKNOWN));
    rb_define_const(cRicoSat, "SATISFIABLE", INT2NUM(PICOSAT_SATISFIABLE));
    rb_define_const(cRicoSat, "UNSATISFIABLE", INT2NUM(PICOSAT_UNSATISFIABLE));
}

/* vim: set sws=4 noet: */
