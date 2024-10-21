#!/usr/bin/env python3

iota_counter=0
def iota(reset=False):
    global iota_counter
    if reset:
        iota_counter = 0
    result = iota_counter
    iota_counter += 1
    return result

OP_PUSH=iota(True)
OP_PLUS=iota()
OP_DUMP=iota()
COUNT_OPS=iota()

def push(x):
    return (OP_PUSH, x)

def plus():
    return (OP_PUSH, )

def dump():
    return (OP_DUMP, )

def simulate_program(program):
    assert False, "Not implemented"

def compile_program(program):
    assert False, "Not implemented"

program=[push(34), push(35), plus(), dump()]

simulate_program(program)