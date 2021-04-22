#!/usr/bin/env python3

'''
Generate .cpp and .h starting from the models.
'''

import random
import os
from os.path import *
import click



chall_dir = join(dirname(__file__), 'src')
textures_dir = join(chall_dir, 'textures')
textures_codes_header_path = join(chall_dir, 'TexturesCodes.h')
custom_h_path = join(chall_dir, 'CustomObjects.h')
res_path = join(chall_dir, 'res.dat')


CODE_LEN = 16
PATTERN_LEN = 64


textures = [
    ('DOOR', 'wooddoor.bmp'),
    ('FLOOR', 'tile.bmp'),
    ('BRICK', 'brick01.bmp'),
    ('ROOF', 'tegola.bmp'),
    ('OOO', 'ooo.bmp'),
    ('RICK', 'rickmod.bmp'),
]


@click.group()
def cli():
    pass


def text2code():
    random.seed(31337)
    t2c = {}
    for tt in textures:
        t2c[tt] = str(random.randrange(10*(CODE_LEN-1), 10**(CODE_LEN)))
    return t2c
t2c = text2code()


def get_pattern():
    return bytes([ random.randrange(256) for _ in range(PATTERN_LEN) ])


class Model:
    def get_cpp(self):
        pts = self.pts
        polys = self.polys
        n2idx = {}
        for idx, (n, _) in enumerate(sorted(pts.items())):
            n2idx[n] = idx
        with_texture = False
        for poly, color in polys:
            if not color.startswith('0x'):
                with_texture = True
                break
        out = ''
        out += f'class {self.class_name} : public Object {{' + '\n'
        out += f'    public:\n'
        out += f'    {self.class_name} () {{' + '\n'
        out += f'        vector<Point> vs;\n'
        out += f'        Polygon *p;\n'
        out += f'        Point v;\n'
        if with_texture:
            out += f'        Point textureCoord;\n'
            out += f'        vector <Point> textureCoord_v;\n'
        for pt_name, coords in sorted(pts.items()):
            out += f'        v.setCoord({coords[0]}, {coords[1]}, {coords[2]});\n'
            out += f'        vs.push_back(v);\n'
        for poly, color in polys:
            out += f'        p = new Polygon();\n'
            if with_texture:
                out += f'        textureCoord_v.clear();\n'
            for pt_name in poly:
                pt_idx = n2idx[pt_name]
                out += f'        p->addVertex(vs[{pt_idx}]);\n'
            if color.startswith('0x'):
                out += f'        p->setColor("{color}");\n'
            else:
                assert with_texture
                elems = color.split()
                if elems[0][0] == 'c':
                    out += f'        p->setTextureCode({elems[0][1:]});\n'
                    out += f'        // FIXME\n'
                else:
                    out += f'        p->setTextureName("{elems[0]}");\n'
                for idx in range((len(elems)-1)//2):
                    out += f'        textureCoord.setCoord({elems[2*idx+1]}, {elems[2*idx+2]}, 0);\n'
                    out += f'        textureCoord_v.push_back(textureCoord);\n'
                out += f'        p->setTextureCoords(textureCoord_v);\n'
                out += f'        p->setColor("0xffffff");\n'
            out += f'        if (!p->areNormalsComputed()) p->computeNormals();\n'
            out += f'        addPolygon(p);\n'
        out += '    }\n'
        out += '};\n'
        return out


class Church(Model):
    def __init__(self):
        self.class_name = 'ChurchObject'

        self.pts = {
            'A': (-30, 0, -30),
            'B': (-30, 0, 30),
            'C': (-30, 70, 30),
            'D': (-30, 100, 0),
            'E': (-30, 70, -30),
            'A1': (30, 0, -30),
            'B1': (30, 0, 30),
            'C1': (30, 70, 30),
            'D1': (30, 100, 0),
            'E1': (30, 70, -30),
            'F1': (-30, 30, -30),
            'F4': (-30, 30, 30),
            'H1': (-30, 0, -10),
            'H2': (-30, 0, 10),
            'H3': (-30, 30, 10),
            'H4': (-30, 30, -10),
            'G1': (-30, 50, -30),
            'G2': (-30, 50, -10),
            'G3': (-30, 50, 10),
            'G4': (-30, 50, 30),
            'L1': (-30, 70, -10),
            'L2': (-30, 70, 10),
        }

        self.polys = [
            (['A', 'F1', 'H4', 'H1'], 'cTEXTURE_BRICK 0 0 0 3 2 3 2 0'),
            (['H2', 'H3', 'F4', 'B'], 'cTEXTURE_BRICK 0 0 0 3 2 3 2 0'),
            (['F1', 'F4', 'G4', 'G1'], 'cTEXTURE_BRICK 0 0 6 0 6 2 0 2'),
            (['G1', 'G2', 'L1', 'E'], 'cTEXTURE_BRICK 0 0 2 0 2 2 0 2'),
            (['G2', 'G3', 'L2', 'L1'], 'cTEXTURE_OOO 0 0 1 0 1 1 0 1'),
            (['G3', 'G4', 'C', 'L2'], 'cTEXTURE_BRICK 0 0 2 0 2 2 0 2'),
            (['E', 'C', 'D'], 'cTEXTURE_BRICK 0 0 6 0 3 3'),
            (['A1', 'B1', 'C1', 'D1', 'E1'], 'cTEXTURE_BRICK 0 0 6 0 6 7 3 10 0 7'),
            (['B', 'B1', 'C1', 'C'], 'cTEXTURE_BRICK 0 0 6 0 7 6 0 7'),
            (['A', 'A1', 'E1', 'E'], 'cTEXTURE_BRICK 0 0 6 0 6 7 0 7'),
            (['E', 'D', 'D1', 'E1'], 'cTEXTURE_ROOF 0 0 0 8.4 12 8.4 12 0'),
            (['C', 'D', 'D1', 'C1'], 'cTEXTURE_ROOF 0 0 0 8.4 12 8.4 12 0'),
        ]


class Rick(Model):
    def __init__(self):
        self.class_name = 'RickObject'

        self.pts = {
            'R1': (28, 0, -30),
            'R2': (28, 0, 30),
            'R3': (28, 70, 30),
            'R4': (28, 70, -30),
        }

        self.polys = [
            (['R1', 'R2', 'R3', 'R4'], 'cTEXTURE_RICK 0 0 1 0 1 1 0 1'),
        ]


class ChurchDoor(Model):
    def __init__(self):
        self.class_name = 'ChurchDoorObject'

        self.pts = {
            'H1': (-30.5, 0, -10.5),
            'H2': (-30.5, 0, 10.5),
            'H3': (-30.5, 30, 10.5),
            'H4': (-30.5, 30, -10.5),
        }

        self.polys = [
            (['H1', 'H2', 'H3', 'H4'], 'cTEXTURE_DOOR 0 0 1 0 1 1 0 1'),
        ]


class Plane(Model):
    def __init__(self):
        self.class_name = 'PlaneObject'
        self.pts = {
            'A': (0, -100, -100),
            'B': (0, -100, 100),
            'C': (0, 100, 100),
            'D': (0, 100, -100),
            'E': (0, 200, 200),
        }
        self.polys = [
            ('ABCD', 'cTEXTURE_RICK 0 0 1 0 1 1 0 1'),
            #  ('ABCD', 'textures/rickmod.bmp 0 0 1 0 1 1 0 1'),
            ('CDE', '0x00ff00'),
        ]


class Box(Model):
    def __init__(self):
        self.class_name = 'BoxObject'
        self.pts = {
            'A': (-0.5, -0.5, -0.5),
            'B': (0.5, -0.5, -0.5),
            'C': (0.5, 0.5, -0.5),
            'D': (-0.5, 0.5, -0.5),
            'E': (-0.5, -0.5, 0.5),
            'F': (0.5, -0.5, 0.5),
            'G': (0.5, 0.5, 0.5),
            'H': (-0.5, 0.5, 0.5),
        }
        self.polys = [
            ('ABCD', '0xffffff'),
            ('ABFE', '0xffffff'),
            ('BCGF', '0xffffff'),
            ('CDHG', '0xffffff'),
            ('ADHE', '0xffffff'),
            ('EFGH', '0xffffff'),
        ]


@cli.command()
def gen_objects_cpp():
    models = [
        Box(),
        Church(),
        ChurchDoor(),
        #  Plane(),
        Rick(),
    ]

    cpp = ''
    cpp += '''#ifndef _CUSTOMOBJECT_H_
    #define _CUSTOMOBJECT_H_

    '''

    cpp += '#include "Object.h"\n'
    cpp += '#include "TexturesCodes.h"\n\n'
    for m in models:
        cpp += m.get_cpp() + '\n'
    cpp += '\n#endif\n'

    with open(custom_h_path, 'w') as f:
        f.write(cpp)
    print(f'Generated {custom_h_path}')


@cli.command()
def gen_texture_headers():
    out = ''
    for tinfo in textures:
        code = t2c[tinfo]
        tname = tinfo[0]
        out += f'#define TEXTURE_{tname} "{code}"\n'

    with open(textures_codes_header_path, 'w') as f:
        f.write(out)
    print(f'Generated {textures_codes_header_path}')


@cli.command()
def pack_resources():
    from pwn import xor, p32

    out = b''
    for tinfo in textures:
        code = t2c[tinfo]
        t_path = join(textures_dir, tinfo[1])
        with open(t_path, 'rb') as f:
            tpayload = f.read()
        tpattern = get_pattern()

        print(f'gen resource {code}, len: {len(tpayload)}')

        tpayload = p32(len(tpayload)) + tpayload

        code = code.encode('utf-8')
        assert(len(code) == CODE_LEN)
        out += b'X' + code + b'X'
        out += tpattern
        out += xor(tpayload, tpattern)

    with open(res_path, 'wb') as f:
        f.write(out)
    print(f'Generated {res_path}')

    rick_path = join(chall_dir, 'rick')
    cmd = f'objcopy {rick_path} --add-section res={res_path}'
    print(f'Executing CMD: {cmd}')
    os.system(cmd)


if __name__ == '__main__':
    cli()
