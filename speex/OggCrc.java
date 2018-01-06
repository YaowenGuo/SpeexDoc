package com.pandarow.chinese.util.speex;

/**
 * Created by yaowen on 2017/12/29.
 */

public class OggCrc {
    // TODO - implement java.util.zip.Checksum

    /**
     * CRC checksum lookup table
     */
    private static int[] crc_lookup;

    static {
        crc_lookup = new int[256];
        for (int i = 0; i < crc_lookup.length; i++) {
            int r = i << 24;
            for (int j = 0; j < 8; j++) {
                if ((r & 0x80000000) != 0) {
          /* The same as the ethernet generator polynomial, although we use an
          unreflected alg and an init/final of 0, not 0xffffffff */
                    r = (r << 1) ^ 0x04c11db7;
                } else {
                    r <<= 1;
                }
            }
            crc_lookup[i] = (r & 0xffffffff);
        }
    }

    /**
     * Calculates the checksum on the given data, from the give offset and
     * for the given length, using the given initial value.
     * This allows on to calculate the checksum iteratively, by reinjecting the
     * last returned value as the initial value when the function is called for
     * the next data chunk.
     * The initial value should be 0 for the first iteration.
     * @param crc - the initial value
     * @param data - the data
     * @param offset - the offset at which to start calculating the checksum.
     * @param length - the length of data over which to calculate the checksum.
     * @return the checksum.
     */
    public static  int checksum(int crc,
                                final byte[] data,
                                int offset,
                                final int length) {
        int end = offset + length;
        for (; offset < end; offset++) {
            crc = (crc << 8) ^ crc_lookup[((crc >>> 24) & 0xff) ^ (data[offset] & 0xff)];
        }
        return crc;
    }
}
