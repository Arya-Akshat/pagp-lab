import zlib, struct, random, sys
w = h = int(sys.argv[1]); out = sys.argv[2]
raw = b''.join(b'\x00' + bytes(random.randrange(256) for _ in range(w*3)) for _ in range(h))
def ch(t, d):
    c = struct.pack('>I', len(d)) + t + d
    return c + struct.pack('>I', zlib.crc32(t + d) & 0xffffffff)
open(out, 'wb').write(b'\x89PNG\r\n\x1a\n' + ch(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0)) + ch(b'IDAT', zlib.compress(raw)) + ch(b'IEND', b''))
