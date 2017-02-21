# ricosat

* https://github.com/tenderlove/ricosat

## DESCRIPTION:

RicoSAT is a wrapper around [PicoSAT](http://fmv.jku.at/picosat/).  It lets
you use the PicoSAT solver from Ruby!

## FEATURES/PROBLEMS:

* It's a SAT solver!  Not SAT like the tests, but the [Boolean satisfiability problem](https://en.wikipedia.org/wiki/Boolean_satisfiability_problem)

## SYNOPSIS:

Solve the problem (A and NOT B):

```ruby
A = 1
B = 2

sat = RicoSAT.new
sat.add(A); sat.add(0)  # Rules must always end with 0
sat.add(-B); sat.add(0) # Negative means "not"

case sat.solve(-1)
when RicoSAT::SATISFIABLE
  sat.variables.times.each do |i|
    p((i + 1) => sat.deref(i + 1))
  end
when RicoSAT::UNSATISFIABLE
  raise "idk"
else
end
```

A is true, and B is false.

Solve the problem (A and NOT A):

```ruby
A = 1

sat = RicoSAT.new
sat.enable_trace_generation # Get access to SAT proofs
sat.add(A); sat.add(0)  # Rules must always end with 0
sat.add(-A); sat.add(0) # Negative means "not"

case sat.solve(-1)
when RicoSAT::SATISFIABLE
  raise "should not work"
when RicoSAT::UNSATISFIABLE
  sat.added_original_clauses.times do |i|
    if sat.coreclause(i) == 1
      p [:clause, i, sat.corelit(1)]
    end
  end
  sat.write_extended_trace $stdout
else
end
```

It's not solvable!

## INSTALL:

* gem install ricosat

## LICENSE:

(The MIT License)

Copyright (c) 2017 Aaron Patterson

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
