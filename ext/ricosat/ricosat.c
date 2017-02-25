#include <ruby.h>
#include <ruby/io.h>
#include <picosat.h>

VALUE cRicoSat;

static const rb_data_type_t RicoSatType = {
    "RicoStat",
    { 0, (void (*)(void *))picosat_reset, 0 },
    0, 0, RUBY_TYPED_FREE_IMMEDIATELY
};

static VALUE allocate(VALUE klass) {
  return TypedData_Wrap_Struct(klass, &RicoSatType, picosat_init());
}

static VALUE measure_all_calls(VALUE self) {
    PicoSAT * sat;

    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    picosat_measure_all_calls(sat);
    return self;
}

static VALUE set_verbosity(VALUE self, VALUE verbosity) {
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    picosat_set_verbosity(sat, NUM2INT(verbosity));
    return self;
}

static VALUE set_more_important_lit(VALUE self, VALUE lit) {
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    picosat_set_more_important_lit(sat, NUM2INT(lit));
    return self;
}

static VALUE set_less_important_lit(VALUE self, VALUE lit) {
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    picosat_set_less_important_lit(sat, NUM2INT(lit));
    return self;
}

static VALUE set_default_phase(VALUE self, VALUE lit, VALUE val) {
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    picosat_set_default_phase_lit(sat, NUM2INT(lit), NUM2INT(val));
    return self;
}

static VALUE inc_max_var(VALUE self) {
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    return INT2NUM(picosat_inc_max_var(sat));
}

static VALUE add(VALUE self, VALUE lit) {
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    return INT2NUM(picosat_add(sat, NUM2INT(lit)));
}

static VALUE solve(VALUE self, VALUE limit) {
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    return INT2NUM(picosat_sat(sat, NUM2INT(limit)));
}

static VALUE deref(VALUE self, VALUE lit) {
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    switch(picosat_deref(sat, NUM2INT(lit))) {
	case 1:
	    return Qtrue;
	case -1:
	    return Qfalse;
	case 0:
	    return Qnil;
    }
    return Qnil;
}

static VALUE failed_assumptions(VALUE self) {
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);

    VALUE failed_list = rb_ary_new();
    const int * failed = picosat_failed_assumptions(sat);
    while(*failed) {
	rb_ary_push(failed_list, INT2NUM(*failed));
	failed++;
    }
    return failed_list;
}

static VALUE failed_assumption(VALUE self, VALUE lit) {
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);

    if (picosat_failed_assumption(sat, NUM2INT(lit))) {
	return Qtrue;
    } else {
	return Qfalse;
    }
}

static VALUE assume(VALUE self, VALUE lit) {
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    picosat_assume(sat, NUM2INT(lit));
    return self;
}

static VALUE variables(VALUE self) {
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    return INT2NUM(picosat_variables(sat));
}

static VALUE enable_trace_generation(VALUE self)
{
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    return INT2NUM(picosat_enable_trace_generation(sat));
}

static VALUE write_extended_trace(VALUE self, VALUE io)
{
    PicoSAT * sat;
    rb_io_t * out;

    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    GetOpenFile(io, out);
    picosat_write_extended_trace(sat, fdopen(out->fd, "w"));
    return self;
}

static VALUE corelit(VALUE self, VALUE lit)
{
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    return INT2NUM(picosat_corelit(sat, NUM2INT(lit)));
}

static VALUE coreclause(VALUE self, VALUE i)
{
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    return INT2NUM(picosat_coreclause(sat, NUM2INT(i)));
}

static VALUE added_original_clauses(VALUE self)
{
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    return INT2NUM(picosat_added_original_clauses(sat));
}

static VALUE set_global_default_phase(VALUE self, VALUE val)
{
    PicoSAT * sat;
    TypedData_Get_Struct(self, PicoSAT, &RicoSatType, sat);
    picosat_set_global_default_phase(sat, NUM2INT(val));
    return self;
}

void error_cb(const char *m) {
    rb_raise(rb_eRuntimeError, "%s", m);
}

void Init_ricosat() {
    cRicoSat = rb_define_class("RicoSAT", rb_cObject);
    rb_define_alloc_func(cRicoSat, allocate);
    picosat_set_error_handler(error_cb);

    rb_define_const(cRicoSat, "UNKNOWN", INT2NUM(PICOSAT_UNKNOWN));
    rb_define_const(cRicoSat, "SATISFIABLE", INT2NUM(PICOSAT_SATISFIABLE));
    rb_define_const(cRicoSat, "UNSATISFIABLE", INT2NUM(PICOSAT_UNSATISFIABLE));
    rb_define_method(cRicoSat, "measure_all_calls", measure_all_calls, 0);
    rb_define_method(cRicoSat, "verbosity=", set_verbosity, 1);
    rb_define_method(cRicoSat, "more_important", set_more_important_lit, 1);
    rb_define_method(cRicoSat, "less_important", set_less_important_lit, 1);
    rb_define_method(cRicoSat, "set_default_phase", set_default_phase, 2);
    rb_define_method(cRicoSat, "inc_max_var", inc_max_var, 0);
    rb_define_method(cRicoSat, "add", add, 1);
    rb_define_method(cRicoSat, "solve", solve, 1);
    rb_define_method(cRicoSat, "deref", deref, 1);
    rb_define_method(cRicoSat, "failed_assumptions", failed_assumptions, 0);
    rb_define_method(cRicoSat, "assume", assume, 1);
    rb_define_method(cRicoSat, "variables", variables, 0);
    rb_define_method(cRicoSat, "failed_assumption", failed_assumption, 1);
    rb_define_method(cRicoSat, "enable_trace_generation", enable_trace_generation, 0);
    rb_define_method(cRicoSat, "write_extended_trace", write_extended_trace, 1);
    rb_define_method(cRicoSat, "corelit", corelit, 1);
    rb_define_method(cRicoSat, "coreclause", coreclause, 1);
    rb_define_method(cRicoSat, "added_original_clauses", added_original_clauses, 0);
    rb_define_method(cRicoSat, "global_default_phase=", set_global_default_phase, 1);
}

/* vim: set sws=4 noet: */
