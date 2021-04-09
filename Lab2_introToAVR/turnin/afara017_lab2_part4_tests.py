# Array of tests to run (in order)
# Each test contains
#   description - 
#   steps - A list of steps to perform, each step can have
#       inputs - A list of tuples for the inputs to apply at that step
#       *time - The time (in ms) to wait before continuing to the next step 
#           and before checking expected values for this step. The time should be a multiple of
#           the period of the system
#       *iterations - The number of clock ticks to wait (periods)
#       expected - The expected value at the end of this step (after the "time" has elapsed.) 
#           If this value is incorrect the test will fail early before completing.
#       * only one of these should be used
#   expected - The expected output (as a list of tuples) at the end of this test
# An example set of tests is shown below. It is important to note that these tests are not "unit tests" in 
# that they are not ran in isolation but in the order shown and the state of the device is not reset or 
# altered in between executions (unless preconditions are used).
tests = [ {'description': 'PINA: 10, PINB: 20, PINC: 30 => PORTD: 0xF0',
    'steps': [ {'inputs': [('PINA',10), ('PINB', 20), ('PINC', 30)], 'iterations': 5 } ],
    'expected': [('PORTD',0xF0)],
    },
    {'description': 'PINA: 90, PINB: 15, PINC: 5 => PORTD: 0xDE',
    'steps': [ {'inputs': [('PINA',90), ('PINB', 15), ('PINC', 5)], 'iterations': 5 } ],
    'expected': [('PORTD',0xDE)],
    },
    {'description': 'PINA: 0, PINB: 0, PINC: 200 => PORTD: 0xC9',
    'steps': [ {'inputs': [('PINA',0), ('PINB', 0), ('PINC', 200)], 'iterations': 5 } ],
    'expected': [('PORTD',0xC9)],
    },
    {'description': 'PINA: 0, PINB: 35, PINC: 85 => PORTD: 0x1E',
    'steps': [ {'inputs': [('PINA',0), ('PINB', 35), ('PINC', 85)], 'iterations': 5 } ],
    'expected': [('PORTC',0x1E)],
    },
    ]

# Optionally you can add a set of "watch" variables these need to be global or static and may need
# to be scoped at the function level (for static variables) if there are naming conflicts. The 
# variables listed here will display everytime you hit (and stop at) a breakpoint
#watch = ['PORTB']

