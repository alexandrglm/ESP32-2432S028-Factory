import struct

def extract_part( partition_table ):

    with open( partition_table, 'rb' ) as f:

        data = f.read()


    for i in range(0, len(data), 32):

        entry = data[i:i+32]

        if entry == b'\xFF' * 32:

            break


        type_, subtype = entry[0], entry[1]

        offset, size = struct.unpack("<II", entry[4:12])

        label = entry[12:28].split(b'\x00', 1)[0].decode(errors='ignore')


        print(f'{label:<12} Type: {type_:02X}\n SubType: {subtype:02X}\n Offset: 0x{offset:05X}\n Size: {size//1024} KB')


if __name__ = "__main__":

    partition_table = input( 'PATH for partition table bin??? ->' )
    extract_part(parse_partition_table)
