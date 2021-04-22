#!/usr/bin/env python3

import time
import sys
import random
import traceback
from os.path import *
import base64
import struct
import signal
import click



TYPES = ['input', 'not', 'and', 'or', 'xor', 'nand', 'nor', 'alt', 'ext0', 'ext1']
NON_INPUT_TYPES = TYPES[:]
NON_INPUT_TYPES.remove('input')
TYPES_NUM = len(TYPES)
MAX_CHILDREN_NUM = 256
LEVELS_NUM = 100


with open(join(dirname(__file__), 'flag')) as f:
    FLAG = f.read()

gDebug = False


@click.group()
def cli():
    pass


@cli.command()
def sh():
    import IPython; IPython.embed(colors='neutral')


def alarm_handler(signum, frame):
    if gDebug:
        log('timeout on answer')
    abort()


@cli.command()
@click.option('--debug', is_flag=True)
def doit(debug):
    global gDebug
    if debug:
        gDebug = True

    signal.signal(signal.SIGALRM, alarm_handler)

    send(b'RICK')  # healthcheck banner

    for level in range(1, LEVELS_NUM+1):
        if gDebug:
            log(f'Sending level {level}')

        f = gen_formula_for_level(level)
        payload = f.to_payload()

        if gDebug:
            log(f.to_pretty())
            log(f'sending payload of len: {len(payload)}')
            log(f'payload size: {f.root.payload_size()}')

        send(payload)

        signal.alarm(15)
        user_values = input().strip()
        signal.alarm(0)

        user_values = xor(user_values.encode('utf-8'), b'RICK').decode('utf-8')

        inputs_num = f.get_inputs_num()

        if len(user_values) != inputs_num:
            if gDebug:
                log(f'ERROR: len does not match. Expected {inputs_num}, but got an input of size {len(user_values)}: {user_values}')
            abort()

        try:
            if gDebug: log(f'user_values: {user_values}')
            assert set(user_values) <= {'0', '1'}
            user_values = list(map(bool, map(int, user_values)))
            vals = f.create_vals(user_values)
            res = f.eval(vals)
        except:
            if gDebug:
                log('ERROR: problems when processing user input')
                log(traceback.format_exc())
            abort()

        if not res:
            if gDebug: log('ERROR: res is not true')
            abort()

    # return the flag
    flag = b'RICK' + struct.pack('<I', len(FLAG)) + xor(FLAG.encode('utf-8'), b'RICK')
    send(flag)


@cli.command()
def test_formulas():
    for level in range(1, LEVELS_NUM+1):
        print(f'trying with level {level}')
        f = gen_formula_for_level(level)
        f.check()
        print(f.to_pretty())
    print('OK')


def gen_formula_for_level(level):
    if level == 1:
        f = Formula()
        f.root = f.create_node('and', 0)
        n = f.create_node('input', 1)
        f.root.add_child(n)
        f.inputs = [n]
    elif level == 2:
        f = Formula()
        f.root = f.create_node('or', 0)
        n1 = f.create_node('input', 1)
        n2 = f.create_node('input', 1)
        f.root.add_child(n1)
        f.root.add_child(n2)
        f.inputs = [n1, n2]
    elif level == 3:
        f = Formula()
        f.root = f.create_node('and', 0)
        n1 = f.create_node('input', 1)
        n2 = f.create_node('input', 1)
        f.root.add_child(n1)
        f.root.add_child(n2)
        f.inputs = [n1, n2]
    elif level == 4:
        f = Formula()
        f.root = f.create_node('or', 0)
        n1 = f.create_node('input', 1)
        n2 = f.create_node('input', 1)
        n3 = f.create_node('input', 1)
        n4 = f.create_node('input', 1)
        f.root.add_child(n1)
        f.root.add_child(n2)
        f.root.add_child(n3)
        f.root.add_child(n4)
        f.inputs = [n1, n2, n3, n4]
    elif level == 5:
        f = Formula()
        f.root = f.create_node('and', 0)
        n1 = f.create_node('input', 1)
        n2 = f.create_node('input', 1)
        n3 = f.create_node('input', 1)
        n4 = f.create_node('input', 1)
        f.root.add_child(n1)
        f.root.add_child(n2)
        f.root.add_child(n3)
        f.root.add_child(n4)
        f.inputs = [n1, n2, n3, n4]
    elif level == 6:
        f = Formula()
        f.root = f.create_node('or', 0)
        n1 = f.create_node('or', 1)
        n2 = f.create_node('or', 1)
        n1i = f.create_node('input', 2)
        n2i = f.create_node('input', 2)
        f.root.add_child(n1)
        f.root.add_child(n2)
        n1.add_child(n1i)
        n2.add_child(n2i)
        f.inputs = [n1i, n2i]
    elif level == 7:
        f = Formula()
        f.root = f.create_node('and', 0)
        n1 = f.create_node('and', 1)
        n2 = f.create_node('or', 1)
        n1i = f.create_node('input', 2)
        n2i = f.create_node('input', 2)
        f.root.add_child(n1)
        f.root.add_child(n2)
        n1.add_child(n1i)
        n2.add_child(n2i)
        f.inputs = [n1i, n2i]
    elif level == 8:
        f = Formula()
        f.root = f.create_node('xor', 0)
        n1 = f.create_node('not', 1)
        n2 = f.create_node('not', 1)
        n1i = f.create_node('input', 2)
        n2i = f.create_node('input', 2)
        f.root.add_child(n1)
        f.root.add_child(n2)
        n1.add_child(n1i)
        n2.add_child(n2i)
        f.inputs = [n1i, n2i]
    elif level == 9:
        f = Formula()
        f.root = f.create_node('and', 0)
        n1 = f.create_node('or', 1)
        n2 = f.create_node('or', 1)
        n1i = f.create_node('input', 2)
        n2i = f.create_node('input', 2)
        n3i = f.create_node('input', 2)
        n4i = f.create_node('input', 2)
        f.root.add_child(n1)
        f.root.add_child(n2)
        n1.add_child(n1i)
        n1.add_child(n2i)
        n2.add_child(n3i)
        n2.add_child(n4i)
        f.inputs = [n1i, n2i, n3i, n4i]
    elif level == 10:
        f = Formula()
        f.root = f.create_node('and', 0)
        n1 = f.create_node('xor', 1)
        n2 = f.create_node('xor', 1)
        n1i = f.create_node('input', 2)
        n2i = f.create_node('input', 2)
        n3i = f.create_node('input', 2)
        n4i = f.create_node('input', 2)
        f.root.add_child(n1)
        f.root.add_child(n2)
        n1.add_child(n1i)
        n1.add_child(n2i)
        n2.add_child(n3i)
        n2.add_child(n4i)
        f.inputs = [n1i, n2i, n3i, n4i]
    elif 11 <= level < 20:
        f = gen_formula_and_check(2, 4, 2, 3)
    elif 20 <= level < 30:
        f = gen_formula_and_check(2, 3, 3, 4)
    elif 30 <= level < 40:
        f = gen_formula_and_check(2, 4, 4, 5)
    elif 40 <= level < 50:
        f = gen_formula_and_check(2, 4, 5, 5)
    elif 50 <= level < 60:
        f = gen_formula_and_check(2, 4, 5, 6)
    elif 60 <= level < LEVELS_NUM:
        f = gen_formula_and_check(3, 4, 6, 6)
    else:
        f = gen_formula_and_check(4, 4, 7, 7)

    f.check()
    return f


def gen_formula_and_check(min_c, max_c, min_d, max_d):
    for _ in range(100):  # try at most n times
        f = gen_formula(min_c=min_c, max_c=max_c, min_d=min_d, max_d=max_d)
        inputs_ids = f.get_inputs_ids()
        inputs_num = len(inputs_ids)

        # check that "all false" does not make the formula true
        values = [False] * inputs_num
        vals = f.create_vals(values)
        e = f.eval(vals)
        if e: continue

        # check with n random attempts
        true_count = 0
        for _ in range(100):
            values = [ random.choice([True, False]) for _ in range(inputs_num) ]
            vals = f.create_vals(values)
            e = f.eval(vals)
            if e:
                true_count += 1

            if true_count > 5:
                continue

        # if we arrive here, it means the formula is not trivially satisfied
        break
    return f


def gen_formula(min_c, max_c, min_d, max_d):
    f = Formula()
    f.root = f.create_node()
    f.inputs = [f.root]
    depth = 1
    target_depth = random.randrange(min_d, max_d+1)
    while depth <= target_depth:
        old_input_nodes = f.inputs[:]
        f.inputs = []
        for input_node in old_input_nodes:
            input_node.type = random.choice(NON_INPUT_TYPES)
            if input_node.type == 'not':
                target_children_num = 1
            else:
                target_children_num = random.randrange(min_c, max_c+1)
            for _ in range(target_children_num):
                n = f.create_node(depth=input_node.depth+1)
                input_node.add_child(n)
                f.inputs.append(n)

        depth += 1

    return f


class Formula():

    def __init__(self):
        Node.next_id = 1
        self.nodes = {}
        self.root = None
        self.inputs = []

    def create_node(self, type_='input', depth=0):
        assert type_ in TYPES
        n = Node(type_=type_, depth=depth)
        self.nodes[n.id] = n
        return n

    def check(self):
        xx = self._get_inputs()
        assert self.inputs == xx
        for n in self.nodes.values():
            n.check()

    def get_inputs_ids(self):
        return sorted([ n.id for n in self.inputs ])

    def get_inputs_num(self):
        return len(self.inputs)

    # this is not strictly necessary, it's just for testing
    def _get_inputs(self):
        return [ n for n in self.nodes.values() if n.type == 'input' ]

    def create_vals(self, values):
        ids = self.get_inputs_ids()
        assert len(ids) == len(values)
        vals = {}
        for node_id, value in zip(ids, values):
            vals[node_id] = bool(value)
        return vals

    def eval(self, inputs):
        return self.root.eval(inputs)

    def to_pretty(self):
        return self.root.to_pretty()

    def to_nums(self):
        return self.root.to_nums()

    def to_payload(self):
        nums = self.to_nums()
        out = b''
        out += struct.pack('<I', len(nums))
        for n in nums:
            out += struct.pack('<I', n)
        return out


class Node():
    next_id = 1

    def __init__(self, type_='input', depth=0):
        self.id = Node.next_id
        Node.next_id += 1
        self.type = type_
        self.children = []
        self.depth = depth

    def add_child(self, node):
        self.children.append(node)

    def check(self):
        '''Check basic consistency rules.'''
        assert self.type in TYPES
        if self.type in ['input']:
            assert len(self.children) == 0
        elif self.type in ['or', 'and', 'xor', 'nand', 'nor', 'alt', 'ext0', 'ext1']:
            assert len(self.children) >= 1
        elif self.type in ['not']:
            assert len(self.children) == 1
        else:
            assert False

    def eval(self, vals):
        assert self.type in TYPES
        if self.type == 'input':
            b = vals[self.id]
            assert type(b) == bool
            return b
        elif self.type == 'not':
            return not self.children[0].eval(vals)
        elif self.type == 'and':
            for c in self.children:
                if not c.eval(vals):
                    return False
            return True
        elif self.type == 'or':
            for c in self.children:
                if c.eval(vals):
                    return True
            return False
        elif self.type == 'xor':
            es = []
            for c in self.children:
                 es.append(1 if (c.eval(vals) is True) else 0)
            acc = 0
            for e in es:
                acc ^= e
            if acc == 1:
                return True
            else:
                return False
        elif self.type == 'nand':
            and_eval = True
            for c in self.children:
                if not c.eval(vals):
                    and_eval = False
                    break
            return (not and_eval)
        elif self.type == 'nor':
            or_eval = False
            for c in self.children:
                if c.eval(vals):
                    or_eval = True
                    break
            return (not or_eval)
        elif self.type == 'alt':
            last_eval = self.children[0].eval(vals)
            for c in self.children[1:]:
                curr_eval = c.eval(vals)
                if curr_eval == last_eval:
                    return False
                last_eval = curr_eval
            return True
        elif self.type == 'ext0':
            # externals are false
            first_eval = self.children[0].eval(vals)
            if first_eval is True:
                return False
            if len(self.children) == 1:
                return True
            last_eval = self.children[-1].eval(vals)
            if last_eval is False:
                return True
            else:
                return False
        elif self.type == 'ext1':
            # externals are true
            first_eval = self.children[0].eval(vals)
            if first_eval is False:
                return False
            if len(self.children) == 1:
                return True
            last_eval = self.children[-1].eval(vals)
            if last_eval is True:
                return True
            else:
                return False
        raise Exception('unknown type')

    def to_pretty(self):
        assert self.type in TYPES
        out = ''
        out += f'{" "*self.depth*4}Node #{self.id}: '
        out += self.type.upper()
        out += f' (depth:{self.depth}) (children #: {len(self.children)})'
        out += '\n'
        for c in self.children:
            out += c.to_pretty()
        return out

    def to_nums(self):
        assert self.type in TYPES
        nums = []
        nums.append(Node.type_to_num(self.type))
        if self.type != 'input':
            nums.append(Node.children_num_to_num(len(self.children)))
            for c in self.children:
                nn = c.to_nums()
                nums.extend(nn)
        return nums

    def payload_size(self):
        if self.type == 'input':
            return 4
        size = 8
        for c in self.children:
            size += c.payload_size()
        return size

    @staticmethod
    def type_to_num(type_):
        type_idx = TYPES.index(type_)
        n = (random.randrange(2**32) // TYPES_NUM) * TYPES_NUM + type_idx
        assert (n % TYPES_NUM) == type_idx
        return n

    @staticmethod
    def children_num_to_num(children_num):
        assert children_num < MAX_CHILDREN_NUM
        n = (random.randrange(2**32) // MAX_CHILDREN_NUM) * MAX_CHILDREN_NUM + children_num
        assert (n % MAX_CHILDREN_NUM) == children_num
        return n

    def __str__(self):
        if self.children:
            return f'{self.id:02d}-{self.type}(' + ','.join(map(str, self.children)) + ')'
        else:
            return f'{self.id:02d}-{self.type}'

    def __repr__(self):
        return str(self)


def send(payload):
    assert type(payload) == bytes
    sys.stdout.buffer.write(payload) 
    sys.stdout.flush()


def log(msg):
    sys.stderr.write(msg + '\n')
    sys.stderr.flush()


def abort():
    msg = 'KO'
    log(msg)
    sys.stdout.write(msg)
    sys.stdout.flush()
    sys.exit(1)


def xor(s1, s2):
    # avoid using pwntools to reduce spawning time
    assert type(s1) == bytes
    assert type(s2) == bytes
    out = b''
    for i in range(len(s1)):
        out += bytes([s1[i] ^ s2[i%len(s2)]])
    return out


if __name__ == '__main__':
    cli()
