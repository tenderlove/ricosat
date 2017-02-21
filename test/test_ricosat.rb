require 'minitest/autorun'
require 'tempfile'
require 'ricosat'

class TestRicosat < MiniTest::Test
  def test_sanity
    assert RicoSAT.new
  end

  def test_measure_all_calls
    assert RicoSAT.new.measure_all_calls
  end

  def test_set_verbosity
    sat = RicoSAT.new
    sat.verbosity = 2
  end

  def test_int_max_var
    sat = RicoSAT.new
    assert_equal 1, sat.inc_max_var
    assert_equal 2, sat.inc_max_var
    assert_equal 3, sat.inc_max_var
  end

  def test_enable_trace
    sat = RicoSAT.new
    assert sat.enable_trace_generation
    sat.add(1); sat.add(0)
    assert_raises do
      sat.write_extended_trace $stdout
    end
    sat.add(-1); sat.add(0)
    sat.solve(-1)
    f = Tempfile.new('trace')
    assert sat.write_extended_trace File.open(f.path, 'w')
    f.unlink
  end

  def test_added_original_clauses
    sat = RicoSAT.new
    sat.add(1); sat.add(0)
    assert_equal 1, sat.added_original_clauses
    sat.add(1); sat.add(0)
    assert_equal 2, sat.added_original_clauses
  end

  def test_add_returns_current_clause
    sat = RicoSAT.new
    assert_equal 0, sat.add(1)
    assert_equal 0, sat.add(0)
    assert_equal 1, sat.add(1)
    assert_equal 1, sat.add(0)
  end

  # A -> [B, C]
  # B -> [D, =1]
  # C -> [D, =2]
  def test_diamond
    sat = RicoSAT.new
    sat.enable_trace_generation
    sat.add(1); sat.add(0)               # A -> [B & C]
    sat.add(2); sat.add(-1); sat.add(0)  # B | !A
    sat.add(3); sat.add(-1); sat.add(0)  # C | !A
    sat.add(4); sat.add(-2); sat.add(0)  # D | !B
    sat.add(4); sat.add(-3); sat.add(0)  # D | !C
    sat.add(-4); sat.add(5); sat.add(0)  # Dv1 | !D
    sat.add(-4); sat.add(-6); sat.add(0)  # Dv1 | !D
    sat.add(-4); sat.add(6); sat.add(0)  # Dv2 | !D
    sat.add(-4); sat.add(-5); sat.add(0)  # Dv2 | !D
    assert_equal RicoSAT::UNSATISFIABLE, sat.solve(-1)
  end

  def test_extended_trace
    sat = RicoSAT.new
    sat.enable_trace_generation
    sat.add(1); sat.add(0)
    sat.add(2); sat.add(0)
    sat.add(-1); sat.add(2); sat.add(0)
    sat.add(-1); sat.add(2); sat.add(0)
    sat.add(-2); sat.add(-1); sat.add(0)
    assert_equal RicoSAT::UNSATISFIABLE, sat.solve(-1)
    # find core clauses that couldn't be solved
    x = sat.added_original_clauses.times.find_all do |i|
      sat.coreclause(i) == 1
    end
    assert_equal [0, 1, 4], x
    # sat.write_extended_trace $stdout
  end

  def test_deref_fail
    sat = RicoSAT.new
    sat.assume 1
    sat.assume 2
    sat.add(-1); sat.add(2); sat.add(0)
    sat.add(-2); sat.add(0)
    assert_equal RicoSAT::UNSATISFIABLE, sat.solve(-1)
    assert sat.failed_assumption 1
    assert_equal [1], sat.failed_assumptions

    assert_raises do
      sat.deref 1
    end
  end

  def test_deref_success
    sat = RicoSAT.new
    sat.add(1);  sat.add(0)
    sat.add(-1); sat.add(2); sat.add(0)
    assert_equal RicoSAT::SATISFIABLE, sat.solve(-1)

    assert_equal [true, true], [1, 2].map { |i| sat.deref i }
  end

  def test_solve
    sat = RicoSAT.new
    sat.add(1);  sat.add(0)
    sat.add(-1); sat.add(2); sat.add(0)
    assert_equal RicoSAT::SATISFIABLE, sat.solve(-1)
  end

  def test_add
    sat = RicoSAT.new
    #sat.add(-2); sat.add(0)
    sat.add(-1); sat.add(-3); sat.add(0)
    sat.add(1); sat.add(2); sat.add(0)
    sat.add(2); sat.add(3); sat.add(0)
    case sat.solve(-1)
    when RicoSAT::SATISFIABLE
      assert sat.deref(2)
      refute sat.deref(-2)
      assert sat.deref(-3)
      assert sat.deref(-1)
    when RicoSAT::UNSATISFIABLE
      flunk "should be satisfiable"
    when RicoSAT::UNKNOWN
      flunk "should be satisfiable"
    end
  end
end
